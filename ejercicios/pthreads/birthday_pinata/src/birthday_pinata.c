// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct shared_data {
  pthread_mutex_t can_i_hit;
  int64_t hit_number;
  uint64_t thread_count;
} shared_data_t;

typedef struct private_data {
  uint64_t thread_number;
  shared_data_t* shared_data;
} private_data_t;

void* pinata(void* data);
int create_threads(shared_data_t* shared_data);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  uint64_t hit_number = 0;
  if (argc == 3) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1 &&
       (sscanf(argv[2], "%" SCNu64, &hit_number) == 1)) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }
  shared_data_t* shared_data = (shared_data_t*)calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = pthread_mutex_init(&shared_data->can_i_hit, /*attr*/NULL);
    if (error == EXIT_SUCCESS) {
    shared_data->thread_count = thread_count;
    shared_data->hit_number = hit_number;
    create_threads(shared_data);
    pthread_mutex_destroy(&shared_data->can_i_hit);
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
}

/**
 * @brief funciona como el hilo principal es el encargado de multiples acciones
 * desde crear hilos, haciendo uso de memoria dinamica, como una impresion, 
 * y luego la liberacion de la memoria usada
 * @param shared_data recibe del main cuantos hilos se van a crear segun
 * lo indica por el usuario, en caso de no recibir nada usa el numero
 * de nucleos y tambien recibe cuantos golpes recibe la pinata
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
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, pinata
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }

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
void* pinata(void* data) {
  assert(data);
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared_data;
  pthread_mutex_lock(&shared_data->can_i_hit);

  int64_t random_number = 0;
  if (shared_data->hit_number <= 0) {
  } else if (shared_data->hit_number > 0) {
    random_number = 5;
    int64_t sub = shared_data->hit_number - random_number;
    shared_data->hit_number =  sub;
    if (shared_data->hit_number <= 0) {
      printf("Thread " "%" PRIu64 "/" "%" PRIu64 ": " "%" PRIu64
      " hits, I broke the pinata \n",
      private_data->thread_number, shared_data->thread_count,
      random_number);
    } else {
      printf("Thread " "%" PRIu64 "/" "%" PRIu64 ": " "%" PRIu64 " hits \n",
      private_data->thread_number, shared_data->thread_count,
      random_number);
    }
  }

  pthread_mutex_unlock(&shared_data->can_i_hit);
  return NULL;
}
