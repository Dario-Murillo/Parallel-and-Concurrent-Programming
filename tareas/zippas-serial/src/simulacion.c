// Copyright 2023 <Dario Murillo Chaverri C15406>
#define  _POSIX_C_SOURCE 200809L
#define  _XOPEN_SOURCE 500L
#define _MAX_INT 1844674407370955161

#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <unistd.h>
#include <zip.h>
#include <math.h>
#include <stdint.h>
#include <pthread.h>
#include <ftw.h>
#include "common.h"
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
 * https://stackoverflow.com/questions/18792489/
 * how-to-create-a-temporary-directory-in-c-in-linux
*/
void datos_carpeta_crear();

void datos_carpeta_destroy();


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

int datos_run(datos_t* datos, FILE* input, int argc, char* argv[]) {
  int error = datos_analisis(datos, input, argc, argv);
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


int create_threads(datos_t* shared_data) {
  int error = EXIT_SUCCESS;

  datos_privados_t* private_data = (datos_privados_t*)
    calloc(shared_data->thread_count, sizeof(datos_privados_t));

  for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
      ; ++thread_number) {

    error = pthread_create(&private_data[thread_number].thread, NULL
      , datos_generate_passw
      , &private_data[thread_number]);
    if (error == EXIT_SUCCESS) {
    } else {
      fprintf(stderr, "Error: could not create secondary thread\n");
      error = 21;
      break;
    }
  }


  for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
    ; ++thread_number) {
      pthread_join(private_data[thread_number].thread, /*value_ptr*/ NULL);
  }

  free(private_data);
  return EXIT_SUCCESS;
}

static int
remove_callback(const char *pathname,
                __attribute__((unused)) const struct stat *sbuf,
                __attribute__((unused)) int type,
                __attribute__((unused)) struct FTW *ftwb) {
  return remove (pathname);
}

void datos_carpeta_crear() {
  /* Create the temporary directory */
  char template[] = "/tmp/tmpdir.XXXXXX";
  char *tmp_dirname = mkdtemp (template);

  if (tmp_dirname == NULL) {
    perror ("tempdir: error: Could not create tmp directory");
  }

  /* Change directory */
  if (chdir (tmp_dirname) == -1) {
    perror ("tempdir: error: ");
  }
  /* Delete the temporary directory */
  if (nftw (tmp_dirname, remove_callback, FOPEN_MAX,
            FTW_DEPTH | FTW_MOUNT | FTW_PHYS) == -1) {
    perror("tempdir: error: ");
  }
}


void repartir_carga(pthread_t thread, int total) {
  int div = total/thread;
}

void* datos_generate_passw(void* data) {
  assert(data);

  datos_privados_t* private_data = (datos_privados_t*) data;
  datos_t* datos = private_data->datos_compartidos;
  /// primer ciclo recorre todos los archivos zips
  for (size_t ind = 0; ind < datos->zips.total; ind++) {
    pthread_mutex_lock(&datos->mutex);
    datos->encontroPass = false;
    datos->insercion = 0;
    pthread_mutex_unlock(&datos->mutex);
    /// segundo ciclo recorre todos los largos posibles de la clave
    for (size_t i = 1; i <= (size_t)datos->limite; i++) {
      char* pass_temp = calloc(i + 1, sizeof(char*));
      // calcular las posibilidades


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

        if (retorno == true && datos->insercion == 0) {
          /// si la clave fue correcta la agrega a un arreglo
          pthread_mutex_lock(&datos->mutex);
          arreglo_agregar(&datos->contrasenas, pass_temp);
          datos->encontroPass = true;
          datos->insercion++;
          pthread_mutex_unlock(&datos->mutex);
          break;
        }
        pthread_mutex_unlock(&datos->mutex);

        if (datos->encontroPass == true) {
          break;
        }
      }
      free(pass_temp);
      if (datos->encontroPass == true) {
        break;
      }
    }
    if (datos->encontroPass == false) {
      arreglo_agregar(&datos->contrasenas, "\n");
    }
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

