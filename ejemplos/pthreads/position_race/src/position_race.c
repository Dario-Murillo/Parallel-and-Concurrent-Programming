// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#define _POSIX_C_SOURCE 199309L
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <unistd.h>

// thread_shared_data_t
// estructura de datos de memoria compartida
// en este caso el dato que comparten todos los hilos es el totaol de hilos
typedef struct shared_data {
  uint64_t position;
  // variable compartida la cual usa para determinar cuando puede
  // comenzar a trabajar un hilo
  pthread_mutex_t can_access_position;
  uint64_t thread_count;
} shared_data_t;


// thread_private_data_t
// estructura de datos de memoria privada
// en este caso el dato privado de cada hilo, es su respectivo numero
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;


void* race(void* data);
int create_threads(shared_data_t* shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }

  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));
  if (shared_data) {
    shared_data->position = 0;   // inicializar el conteo de hilos en 0

    // declaracion del mutex
    error = pthread_mutex_init(&shared_data->can_access_position, /*attr*/NULL);
    if (error == EXIT_SUCCESS) {
    shared_data->thread_count = thread_count;
    // estructura de datos solicitada para medir el tiempo de ejecucion
    struct timespec start_time, finish_time;
    clock_gettime(CLOCK_MONOTONIC, &start_time);  // el reloj toma el tiempo

    error = create_threads(shared_data);  // los hilos trabajan

    clock_gettime(CLOCK_MONOTONIC, &finish_time);  // el reloj toma el tiempo

    // determinamos cuanto duro en hacer el trabajo
    double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;

    printf("Execution time: %.9lfs\n", elapsed_time);
      // liberacion del mutex una vez los hilos hayan terminado sus trabajos
      pthread_mutex_destroy(&shared_data->can_access_position);
      free(shared_data);
    } else {
      fprintf(stderr, "Error: could not init mutex\n");
      return 13;
    }
  } else {
    fprintf(stderr, "Error: could not allocate shared data\n");
    return 12;
  }
  return error;
}  // end procedure

/**
 * @brief funciona como el hilo principal es el encargado de multiples acciones
 * desde crear hilos, haciendo uso de memoria dinamica, como una impresion, 
 * y luego la liberacion de la memoria usada
 * @param shared_data recibe del main cuantos hilos se van a crear segun
 * lo indica por el usuario, en caso de no recibir nada usa el numero
 * de nucleos
*/
int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*)
    malloc(shared_data->thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(shared_data->thread_count, sizeof(private_data_t));
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].shared_data = shared_data;
      // create_thread(greet, thread_number)
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, race
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , shared_data->thread_count);
    error = 22;
  }

  return error;
}

/**
 * @brief subrutina llamada por los hilos, indica:
 * total de hilos, numero de hilos, en que posicion llego
 * @param data
*/
void* race(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;

  // para poder bloquear la posicion de carrera ponemos un mutex
  // el mutex se encarga de crear un "candado" que permite que solo
  // un hilo entre a estos recursos a la vez
  pthread_mutex_lock(&shared_data->can_access_position);

  ++shared_data->position;

  uint64_t my_position = shared_data->position;
  // print "Hello from secondary thread"
  printf("Thread %" PRIu64 "/%" PRIu64 ": I arrived at position %" PRIu64 "\n"
    , private_data->thread_number, shared_data->thread_count, my_position);

  // hay que desbloquear el mutex para informarle al sistema operativo
  // que el hilo termino de hacer sus procesos y esta listo para que otro entre
  pthread_mutex_unlock(&shared_data->can_access_position);
  return NULL;
}  // end procedure