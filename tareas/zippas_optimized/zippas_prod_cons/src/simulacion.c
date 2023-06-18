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
 * @brief lee el archivo introducido y los argumentos de
 * la entrada estandar y carga los valores en la estructura de datos
 * @param datos puntero de una variable tipo datos_t
 * @param input archivo introducido de la entrada estandar
 * @param argc cantidad de argumentos
 * @param argv argumentos
 * @return codigo de error 
*/
int datos_analisis(datos_t* datos, FILE* input, int argc, char* argv[]);

/**
 * @brief codigo encargado de abrir un archivo zip encriptado
 * @param archivo dirrecion relativa del archivo a abrir
 * @param clave posible clave para abrir el archivo
 * @param datos puntero de una variable de tipo datos_t
 * @return booleano que indica si abrio el archivo o no
 * @details
 Adaptado de 
 https://www.geeksforgeeks.org/
 c-program-to-read-and-print-all-files-from-a-zip-file/
*/
void* datos_abrir_archivo(void* data);

/**
 * @brief genera todas las posibles claves dado 
 * un alfabeto y un maximo de la clave, se reparte
 * la carga entre multiples hilos
 * @param data puntero de una variable tipo datos_t
 * @return void
*/
void* datos_generate_passw(datos_t* datos);

/**
 * @brief imprime las dirreciones de los zips y sus contrasenas
 * @param datos puntero de una variable tipo datos_t
 * @return void
 * 
*/
void datos_impresion(datos_t* datos);


/**
 * @brief crea equipo de hilos con los respectivos datos
 * @param datos puntero de una variable tipo datos_t
 * @return codigo de error
*/
int crear_hilos(datos_t* datos);


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
  queue_init(&datos->cola);
  pthread_mutex_init(&datos->cambiar_variable, NULL);
  pthread_mutex_init(&datos->abrir_archivo, NULL);
  sem_init(&datos->can_consume, 0, 0);
  datos->limite = 0;
  datos->contador_pass = 0;
}

void datos_destroy(datos_t* datos) {
  assert(datos);
  arreglo_destroy(&datos->alfabeto);
  arreglo_destroy(&datos->zips);
  arreglo_destroy(&datos->contrasenas);
  queue_destroy(&datos->cola);
  pthread_mutex_destroy(&datos->cambiar_variable);
  pthread_mutex_destroy(&datos->abrir_archivo);
  sem_destroy(&datos->can_consume);
  datos->limite = 0;
  datos->contador_pass = 0;
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
            r2 = borrar_carpeta(buf);
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

  // creacion de carpeta temporal
  error = mkdir("tmp", 0777);
  if (!error) {
  } else {
    error = EXIT_FAILURE;
    return error;
  }

  // analisis de datos
  error = datos_analisis(datos, input, argc, argv);
  // creacion de hilos y ejecuccion del programa
  if (error == EXIT_SUCCESS) {
    crear_hilos(datos);
  } else {
    printf("Error al analizar los datos");
    error = EXIT_FAILURE;
    return error;
  }

  datos_impresion(datos);
  borrar_carpeta("tmp");
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
        // strcspn evita agregar cambios de linea
        str[strcspn(str, "\n")] = 0;
        arreglo_agregar(&datos->alfabeto, str);
      } else if (linea == 1 && linea < 2) {
        str[strcspn(str, "\n")] = 0;
        sscanf(str, "%"  SCNu64 , &value);
        // revisa que no sea un numero muy grande
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
        // revisar si es una dirrecion valida
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
  // numero por default de hilos
  datos->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    // revisa si hay un argumento para indicar la cantidad de hilos
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

int crear_hilos(datos_t* datos) {
  int error = EXIT_SUCCESS;
  datos_privados_t* private_data = (datos_privados_t*)
    calloc(datos->thread_count, sizeof(datos_privados_t));


  unsigned int seed = time(NULL);
  // crea copias de cada archivo para cada hilo
  for (uint64_t thread_number = 0; thread_number < datos->thread_count
    ; thread_number++) {
    arreglo_innit(&private_data[thread_number].archivos);
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
        index = rand_r(&seed) % 62;
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
    private_data[thread_number].datos_compartidos = datos;
    error = pthread_create(&private_data[thread_number].thread
        , NULL
        , datos_abrir_archivo
        , &private_data[thread_number]);
  }

  datos_generate_passw(datos);


  for (uint64_t thread_number = 0; thread_number < datos->thread_count
    ; ++thread_number) {
      pthread_join(private_data[thread_number].thread, NULL);
      arreglo_destroy(&private_data[thread_number].archivos);
  }

  free(private_data);
  return error;
}


void* datos_generate_passw(datos_t* datos) {
  int64_t base = strlen(datos->alfabeto.array[0]);
  for (uint64_t i = 1; i <= datos->limite; i++) {
    char* pass_temp = calloc(i, sizeof(char*));
    for (int64_t j = 0; j < pow(base, i) ; j++) {
      int64_t numero = j;
      size_t cont = 0;
      if (numero == 0) {
        for (int64_t k = 0; k < (int64_t)i; k++) {
          pass_temp[k] = datos->alfabeto.array[0][0];
        }
      } else {
        // ciclo para cambiar de base el numero
        while (numero > 0) {
          int64_t div = numero / base;
          int64_t residuo = numero % base;
          pass_temp[cont] = datos->alfabeto.array[0][residuo];
          numero = div;
          cont++;
        }
      }
      pass_temp[i] = '\0';
      queue_enqueue(&datos->cola, pass_temp);
      sem_post(&datos->can_consume);

    }
    free(pass_temp);
  }
  for (int64_t i = 0; i < (int64_t)datos->thread_count; i++) {
    queue_enqueue(&datos->cola, "");
    sem_post(&datos->can_consume);
  }
  return NULL;
}


void* datos_abrir_archivo(void* data) {
  datos_privados_t* private_data = (datos_privados_t*) data;
  datos_t* datos = private_data->datos_compartidos;

  while (true) {
    sem_wait(&datos->can_consume);
    char* clave = calloc(100, sizeof(char*));
    queue_dequeue(&datos->cola, clave);

    if (strcmp(clave, "") == 0) {
      free(clave);
      break;
    }
    
    for (size_t i = 0; i < datos->zips.total; i++) {
      int error = EXIT_SUCCESS;

      /// apertura de archivo zip
      zip_t* arch = NULL;
      pthread_mutex_lock(&datos->abrir_archivo);
      arch = zip_open(private_data->archivos.array[i], 0, &error);
      pthread_mutex_unlock(&datos->abrir_archivo);

      /// estructura que almacena distintos datos
      struct zip_stat* finfo = NULL;
      finfo = calloc(256, sizeof(int));
      zip_stat_init(finfo);

      /// para los archvivos comprimidos dentro del zip
      zip_file_t* fd = NULL;

      /// buffer que almacenara el texto del archivo
      char* txt = NULL;

      zip_stat_index(arch, 0, 0, finfo);

      txt = calloc(finfo->size + 1, sizeof(char*));
      fd = zip_fopen_index_encrypted(arch, 0, ZIP_FL_ENC_GUESS, clave);
      if (fd != NULL) {
        zip_fread(fd, txt, finfo->size);
        if (strcmp(txt, "CI0117-23a") == 0) {
          pthread_mutex_lock(&datos->cambiar_variable);
          strcpy(datos->claves[i], clave);
          datos->contador_pass++;
          pthread_mutex_unlock(&datos->cambiar_variable);
        }
        zip_fclose(fd);
      }
      free(txt);
      free(finfo);
      zip_close(arch);
    }
    free(clave);
    pthread_mutex_lock(&datos->cambiar_variable);
    if (datos->contador_pass == datos->zips.total) {
      pthread_mutex_unlock(&datos->cambiar_variable);
      break;
    }
    pthread_mutex_unlock(&datos->cambiar_variable);
  }
  return NULL;
}

void datos_impresion(datos_t* datos) {
  assert(datos);
  FILE* output = stdout;
  for (size_t i = 0; i < datos->zips.total; i++) {
    fprintf(output, "%s %s\n", datos->zips.array[i]
    , datos->claves[i]);
  }
}
