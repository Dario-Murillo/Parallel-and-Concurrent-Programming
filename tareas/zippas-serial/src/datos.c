// Copyright 2023 <Dario Murillo Chaverri C15406>

#define _MAX_INT 1844674407370955161
#include <datos.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <zip.h>
#include <math.h>
#include <stdint.h>


datos_t* datos_create() {
  datos_t* datos = (datos_t*)
  calloc(1 , sizeof(datos_t));
  return datos;
}


void datos_innit(datos_t* datos) {
  assert(datos);
  arreglo_innit(&datos->alfabeto);
  arreglo_innit(&datos->zips);
  arreglo_innit(&datos->contrasenas);
  datos->limite = 0;
}


void datos_destroy(datos_t* datos) {
  assert(datos);
  arreglo_destroy(&datos->alfabeto);
  arreglo_destroy(&datos->zips);
  arreglo_destroy(&datos->contrasenas);
  free(datos);
}


int datos_analisis(datos_t* datos, FILE* input) {
  assert(datos);
  assert(input);
  int error = EXIT_SUCCESS;
  if (input != NULL) {
      char str[120];
      int linea = 0;
      uint64_t value = 0;
      while (fgets(str, 120, input)) {
          if (linea == 0) {
              /// strcspn evita agregar cambios de linea
              str[strcspn(str, "\n")] = 0;
              arreglo_agregar(&datos->alfabeto, str);
          } else if (linea == 1 && linea < 2) {
              str[strcspn(str, "\n")] = 0;
              sscanf(str, "%"  SCNu64 , &value);
              /// revisa que no sea un numero muy grande
              if (value < _MAX_INT) {
                sscanf(str, "%"  SCNu64 , &datos->limite);
              } else {
                printf("%s\n", str);
                fprintf(stderr, "Numero demasiado grande\n");
                error = EXIT_FAILURE;
                break;
              }
          } else if (linea >= 3) {
              str[strcspn(str, "\n")] = 0;
              FILE* fp;
              /// revisar si es una dirrecion valida
              if ((fp = fopen(str, "r")) != NULL) {
                arreglo_agregar(&datos->zips, str);
                fclose(fp);
              } else {
                fprintf(stderr, "Dirrecion de archivo invalida\n");
                error = EXIT_FAILURE;
                break;
              }
          }
          linea++;
      }
  } else {
      error = EXIT_FAILURE;
      return error;
  }
  return error;
}


void datos_generate_passw(datos_t* datos) {
  assert(datos);
  /// primer ciclo recorre todos los archivos zips
  for (size_t ind = 0; ind < datos->zips.total; ind++) {
    bool encontro = false;
    int64_t insercion = 0;
    /// segundo ciclo recorre todos los largos posibles de la clave
    for (size_t i = 1; i <= (size_t)datos->limite; i++) {
        char* pass_temp = calloc(i + 1, sizeof(char*));
        /// tercer ciclo recorre todas los caracteres del arreglo
        for (size_t j = 0; j < pow(strlen(datos->alfabeto.array[0]), i); j++) {
            /// largo de alfabero = base
            int64_t base = strlen(datos->alfabeto.array[0]);
            int64_t numero = j;
            size_t cont = 0;
            if (numero == 0) {
                for (size_t k = 0; k < i; k++) {
                    pass_temp[k] = datos->alfabeto.array[0][0];
                }
            } else {
                /// ciclo para cambiar de base el numero
                while (numero > 0) {
                    int64_t div = numero / base;
                    int64_t residuo = numero % base;
                    pass_temp[cont] = datos->alfabeto.array[0][residuo];
                    numero = div;
                    cont++;
                }
            }
            pass_temp[i] = '\0';  /// agregar terminacion nula a la clave
            bool retorno =
            datos_abrir_archivo(datos->zips.array[ind], pass_temp);
            if (retorno == true && insercion == 0) {
                /// si la clave fue correcta la agrega a un arreglo
                arreglo_agregar(&datos->contrasenas, pass_temp);
                encontro = true;
                insercion++;
                break;
            }
        }
        free(pass_temp);
        if (encontro == true) {
            break;
        }
    }
    if (encontro == false) {
        arreglo_agregar(&datos->contrasenas, "\n");
    }
  }
}


bool datos_abrir_archivo(const char* archivo, const char* key) {
  int error = EXIT_SUCCESS;
  bool found_key = false;
  /// apertura de archivo zip
  zip_t* arch = NULL;
  arch = zip_open(archivo, 0, &error);

  /// estructura que almacena distintos datos
  struct zip_stat* finfo = NULL;
  finfo = calloc(256, sizeof(int));
  zip_stat_init(finfo);

  /// para los archvivos comprimidos dentro del zip
  zip_file_t* fd = NULL;

  /// buffer que almacenara el texto del archivo
  char* txt = NULL;
  zip_int64_t count = 0;

  while ((zip_stat_index(arch, count, 0, finfo)) == 0) {
    txt = calloc(finfo->size + 1, sizeof(char*));
    fd = zip_fopen_index_encrypted(arch, 0, ZIP_FL_ENC_GUESS, key);
    if (fd != NULL) {
      zip_fread(fd, txt, finfo->size);
      if (txt[0] == 'C') {
        found_key = true;
      }
      zip_fclose(fd);
    }
    free(txt);
    count++;
  }
  free(finfo);
  zip_close(arch);
  return found_key;
}

void datos_impresion(datos_t* datos) {
  assert(datos);
  FILE* output = stdout;
  char* linea = "\n";
  for (size_t i = 0; i < datos->zips.total; i++) {
    if (strcmp(datos->contrasenas.array[i], linea) == 0) {
      fprintf(output, "%s\n", datos->zips.array[i]);
    } else {
      fprintf(output, "%s %s\n", datos->zips.array[i]
      , datos->contrasenas.array[i]);
    }
  }
}

