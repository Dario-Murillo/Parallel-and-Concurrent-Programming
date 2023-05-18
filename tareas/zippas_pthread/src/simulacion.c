// Copyright 2023 <Dario Murillo Chaverri C15406>


#define _MAX_INT 1844674407370955161

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>
#include <zip.h>
#include <math.h>
#include <stdint.h>
#include <sys/stat.h>
#include <dirent.h>
#include "simulacion.h"



/**
 * @brief lee el archivo introducido por la entrada estandar
 * y carga los valores en las variables respectivas
 * @param datos puntero de una variable tipo datos_t
 * @param input archivo introducido de la entrada estandar
 * @return codigo de error 
*/
int datos_analisis(datos_t* datos, FILE* input, int argc, char* argv[]);

/**
 * @brief codigo encargado de abrir un archivo zip encriptado
 * @param archivo dirrecion relativa del archivo a abrir
 * @param clave posible clave para abrir el archivo
 * @return booleano que indica si abrio el archivo o no
 * @details
 Adaptado de 
 https://www.geeksforgeeks.org/
 c-program-to-read-and-print-all-files-from-a-zip-file/
*/
bool datos_abrir_archivo(const char* archivo, const char* clave);

/**
 * @brief genera todas las posibles claves dado 
 * un alfabeto y un maximo de la clave
 * @param datos puntero de una variable tipo datos_t
 * @return void
*/
void* datos_generate_passw(void* data);

/**
 * @brief imprime las dirreciones de los zips y sus contrasenas
 * @param datos puntero de una variable tipo datos_t
 * @return void
 * 
*/
void datos_impresion(datos_t* datos);

/**
 * @brief principio de mapeo por bloque
 * @param thread_number numero de hilo
 * @param trabajo cantidad de trabajo a procesar
 * @param datos puntero de una variable tipo datos_t
 * @return carga inicial de trabajo
*/
int inicio(uint64_t thread_number, int trabajo, datos_t* datos);

/**
 * @brief final de mapeo por bloque
  * @param thread_number numero de hilo
 * @param trabajo cantidad de trabajo a procesar
 * @param datos puntero de una variable tipo datos_t
 * @return carga final de trabajo
*/
int final(uint64_t thread_number, int trabajo, datos_t* datos);

/**
 * @brief crea equipo de hilos con los respectivos datos
 * @param datos puntero de una variable tipo datos_t
 * @return codigo de error
*/
int create_threads(datos_t* datos);


/**
 * @brief elimina una carpeta y los archivos dentro de ella
 * @param carpeta nombre de la carpeta a eliminar
 * @return codigo de error
 * @details Adaptado de:
  https://stackoverflow.com/questions/2256945/
  removing-a-non-empty-directory-programmatically-in-c-or-c
 * 
*/
int borrar_carpeta(const char *carpeta);

/**
 * @brief selecciona un caracter random
 * @param index 
 * @return caracter random
*/
char random_char(int index);


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
  pthread_mutex_init(&datos->mutex, 0);
  datos->limite = 0;
}

void datos_destroy(datos_t* datos) {
  assert(datos);
  arreglo_destroy(&datos->alfabeto);
  arreglo_destroy(&datos->zips);
  arreglo_destroy(&datos->contrasenas);
  pthread_mutex_destroy(&datos->mutex);
  free(datos);
}


int borrar_carpeta(const char *carpeta) {
  DIR *d = opendir(carpeta);
  size_t path_len = strlen(carpeta);
  int r = -1;

  if (d) {
    struct dirent *p;
    r = 0;
    while (!r && (p=readdir(d))) {
      int r2 = -1;
      char *buf;
      size_t len;
      if (!strcmp(p->d_name, ".") || !strcmp(p->d_name, ".."))
          continue;
      len = path_len + strlen(p->d_name) + 2; 
      buf = malloc(len);
      if (buf) {
        struct stat statbuf;
        snprintf(buf, len, "%s/%s", carpeta, p->d_name);
        if (!stat(buf, &statbuf)) {
          if (S_ISDIR(statbuf.st_mode))
            r2 = remove_directory(buf);
          else
            r2 = unlink(buf);
        }
        free(buf);
      }
      r = r2;
    }
    closedir(d);
  }
  if (!r)
    r = rmdir(carpeta);
  return r;
}

int datos_run(datos_t* datos, FILE* input, int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  error = mkdir("tmp", 0777); 
  if (!error) {
    return EXIT_FAILURE;
  }
  int error = datos_analisis(datos, input, argc, argv);
  /// Iniciar barrera una vez que sabemos la cantidad de hilos
  pthread_barrier_init(&datos->barrera, NULL, datos->thread_count);
  create_threads(datos);


  borrar_carpeta("tmp");
  pthread_barrier_destroy(&datos->barrera);
  return error;
}

int datos_analisis(datos_t* datos, FILE* input, int argc, char* argv[]) {
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
  datos->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &datos->thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }
  return error;
}

char random_char(int index) {
  char charset[] = 
  "abcdefghijklmnopqrstuvwxyzABCDEFGHIJKLMNOPQRSTUVWXYZ0123456789";
  return charset[index];
}

int inicio(uint64_t thread_number, int trabajo, datos_t* datos) {
  int min = 0;
  if(thread_number <  trabajo % datos->thread_count) {
    min = thread_number;
  } else {
    min = trabajo % datos->thread_count;
  }
  return thread_number * (trabajo / datos->thread_count) + min;
}

int final(uint64_t thread_number, int trabajo, datos_t* datos) {
  return inicio(thread_number+1, trabajo, datos);
}

int create_threads(datos_t* datos) {
  int error = EXIT_SUCCESS;
  datos_privados_t* private_data = (datos_privados_t*)
    calloc(datos->thread_count, sizeof(datos_privados_t));

  srand(time(NULL));

  for (uint64_t thread_number = 0; thread_number < datos->thread_count
    ; thread_number++) {
    
    arreglo_innit(&private_data->archivos);
    for (uint64_t archivo_number = 0; archivo_number < datos->zips.total
      ; archivo_number++) {
      FILE *src, *dest;
      char buf[BUFSIZ];
      size_t n;
      char tmp_name[100];
      char str[17] = "";
      char aux[5] = ".zip";

      int index;
      for (int i = 0; i < 16; i++) {
        index = rand() % 62;
        str[i] = random_char(index);
      }

      strcpy(tmp_name, "tmp/");
      strcat(tmp_name, str);
      strcat(tmp_name, aux);

      int largo = strlen(tmp_name);
      tmp_name[largo] = '\0';

      src = fopen(datos->zips.array[archivo_number], "rb");
      dest = fopen(tmp_name, "wb");

      while ((n = fread(buf, 1, BUFSIZ, src)) > 0) {
        fwrite(buf, 1, n, dest);
      }

      arreglo_agregar(&private_data[thread_number].archivos, tmp_name);
      fclose(src);
      fclose(dest);
    }
  }

  for (uint64_t thread_number = 0; thread_number < datos->thread_count
      ; ++thread_number) {
    
    arreglo_innit(&private_data[thread_number].carga_inicio);
    arreglo_innit(&private_data[thread_number].carga_final);

    for (uint64_t largo = 1; largo <= datos->limite; largo++) {
      int trabajo = pow(strlen(datos->alfabeto.array[0]), largo);
      char in[100];
      sprintf(in, "%d", inicio(thread_number, trabajo, datos));
      arreglo_agregar(&private_data[thread_number].carga_inicio, in);

      char fi[100];
      sprintf(fi, "%d",  final(thread_number, trabajo, datos));
      arreglo_agregar(&private_data[thread_number].carga_final, fi);
    }

    private_data[thread_number].datos_compartidos = datos;
    error = pthread_create(&private_data[thread_number].thread
        , NULL
        , datos_generate_passw
        , &private_data[thread_number]);
    if (error == EXIT_SUCCESS) {
    } else {
      fprintf(stderr, "Error: could not create secondary thread\n");
      error = 21;
      break;
    }
  }

  for (uint64_t thread_number = 0; thread_number < datos->thread_count
    ; ++thread_number) {
      pthread_join(private_data[thread_number].thread, NULL);
      arreglo_destroy(&private_data[thread_number].carga_inicio);
      arreglo_destroy(&private_data[thread_number].carga_final);
      arreglo_destroy(&private_data[thread_number].archivos);
  }

  free(private_data);
  return EXIT_SUCCESS;
}


void* datos_generate_passw(void* data) {
  assert(data);
  datos_privados_t* private_data = (datos_privados_t*) data;
  datos_t* datos = private_data->datos_compartidos;
  /// primer ciclo recorre todos los archivos zips
  for (uint64_t ind = 0; ind < datos->zips.total; ind++) {
    pthread_mutex_lock(&datos->mutex);
    datos->encontroPass = false;
    datos->insercion = 0;
    pthread_mutex_unlock(&datos->mutex);
    /// segundo ciclo recorre todos los largos posibles de la clave
    for (uint64_t i = 1; i <= (size_t)datos->limite; i++) {
      char* pass_temp = calloc(i + 1, sizeof(char*));
      /// tercer ciclo recorre todas los caracteres del arreglo
      for (int j = atoi(private_data->carga_inicio.array[i-1]); 
          j < atoi(private_data->carga_final.array[i-1]); j++) {
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
        puts(pass_temp);
        bool retorno =
        datos_abrir_archivo(private_data->archivos.array[ind], pass_temp);

        if (retorno == true && private_data->datos_compartidos->insercion == 0) {
          /// si la clave fue correcta la agrega a un arreglo
          pthread_mutex_lock(&datos->mutex);
          arreglo_agregar(&datos->contrasenas, pass_temp);
          datos->encontroPass = true;
          datos->insercion++;
          pthread_mutex_unlock(&datos->mutex);
          break;
        }

        if (private_data->datos_compartidos->encontroPass == true) {
          break;
        }
      }
      free(pass_temp);
      if (private_data->datos_compartidos->encontroPass == true) {
        break;
      }
    }
    pthread_mutex_lock(&datos->mutex);
    if (private_data->datos_compartidos->encontroPass == false 
      && private_data->datos_compartidos->insercion  == 0) {
      arreglo_agregar(&datos->contrasenas, "\n");
      datos->insercion++;
      pthread_mutex_unlock(&datos->mutex);
    }
    pthread_mutex_unlock(&datos->mutex);
    pthread_barrier_wait(&datos->barrera);
  }
  return NULL;
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
