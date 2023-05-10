// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#define _DEFAULT_SOURCE

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct shared_data {
  size_t team_count;  // numero de hilos
  useconds_t stage1_duration;  // delay de la primer parte de la carrera
  useconds_t stage2_duration;  // delay de la segunda parte de la carrera
  size_t position;  // posicion de los equipos al terminar la carrera
  pthread_barrier_t start_barrier;  // barreras
  sem_t* batons;  // arreglo de semaforos
  pthread_mutex_t finish_mutex;
  // mutex para la impresion al final del programa
} shared_data_t;

typedef struct private_data {
  size_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief crea los dos hilos de cada equipo
 * @param shared_data puntero a memoria compartida
 * @return codigo de error
*/
int create_threads(shared_data_t* shared_data);

/**
 * @brief analiza los argumentos de la linea de comandos
 * @param argc numero de argumentos
 * @param argv arreglo que contiene los argumentos
 * @param shared_data puntero a memoria compartida
 * @return codigo de error
*/
int analyze_arguments(int argc, char* argv[], shared_data_t* shared_data);

/**
 * @brief "primer parte de la carrera" 
 * la llevan acabo los primeros hilos de cada equipo
 * @param data memoria compartida
*/
void* start_race(void* data);

/**
 * @brief "segunda parte de la carrera" 
 * la llevan acabo los segundos hilos de cada equipo
 * y ademas los 3 primeros equipos en terminar lo imprimen en consola
 * @param data memoria compartida
*/
void* finish_race(void* data);

/**
 * @brief subrutina principal inicializa memoria, mutex, barreras, etc...
 * se encarga de la destruccion y liberacion de memoria y de analizar
 * el tiempo de ejecuccion del programa al llamar las subrutinas que 
 * resuelven el problema
*/
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;

  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));

  if (shared_data) {
    error = analyze_arguments(argc, argv, shared_data);
    if (error == EXIT_SUCCESS) {
      shared_data->position = 0;
      error = pthread_barrier_init(&shared_data->start_barrier,
        /*attr*/ NULL, /*count*/ shared_data->team_count);
      shared_data->batons = (sem_t*) calloc(shared_data->team_count
        , sizeof(sem_t));
      error += pthread_mutex_init(&shared_data->finish_mutex, /*attr*/ NULL);

      if (error == EXIT_SUCCESS && shared_data->batons) {
        // Inicia todos los semaforos en 0
        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_init(&shared_data->batons[index], /*pshared*/ 0, /*value*/ 0);
        }

        struct timespec start_time, finish_time;
        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

        error = create_threads(shared_data);

        clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);
        double elapsed_time = finish_time.tv_sec - start_time.tv_sec +
          (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
        printf("execution time: %.9lfs\n", elapsed_time);

        for (size_t index = 0; index < shared_data->team_count; ++index) {
          sem_destroy(&shared_data->batons[index]);
        }
        pthread_mutex_destroy(&shared_data->finish_mutex);
        free(shared_data->batons);
        pthread_barrier_destroy(&shared_data->start_barrier);
      } else {
        fprintf(stderr, "error: could not init mutex\n");
        error = 11;
      }
    }

    free(shared_data);
  } else {
    fprintf(stderr, "error: could not allocated shared memory\n");
    error = 12;
  }

  return error;
}

int analyze_arguments(int argc, char* argv[]
    , shared_data_t* shared_data) {
  if (argc == 4) {
    if ( sscanf(argv[1], "%zu", &shared_data->team_count) != 1
      || shared_data->team_count == 0 ) {
      fprintf(stderr, "invalid team count: %s\n", argv[1]);
      return 11;
    }

    if ( sscanf(argv[2], "%u", &shared_data->stage1_duration) != 1 ) {
      fprintf(stderr, "invalid stage 1 duration: %s\n", argv[2]);
      return 12;
    }

    if ( sscanf(argv[3], "%u", &shared_data->stage2_duration) != 1 ) {
      fprintf(stderr, "invalid stage 2 duration: %s\n", argv[3]);
      return 13;
    }
    return EXIT_SUCCESS;
  } else {
    fprintf(stderr, "usage: relay_race teams stage1duration stage2duration\n");
    return 10;
  }
}

int create_threads(shared_data_t* shared_data) {
  int error = EXIT_SUCCESS;

  // 2 hilos por cada equipo
  const size_t thread_count = 2 * shared_data->team_count;
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));

  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));

  if (threads && private_data) {
    // Orden original
    #if 0
    // Creacion de los primeros hilos de cada equipo
    for (size_t index = 0; index < shared_data->team_count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, start_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    // Creacion de los segundos hilos de cada equipo
    for (size_t index = shared_data->team_count; index < thread_count;
        ++index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, finish_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }
    #endif
    // Creacion de los primeros hilos de cada equipo
    for (size_t index = shared_data->team_count-1; (int)index >= 0; --index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, start_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    // Creacion de los segundos hilos de cada equipo
    for (size_t index = thread_count-1; index >= shared_data->team_count;
        --index) {
      private_data[index].thread_number = index;
      private_data[index].shared_data = shared_data;

      error = pthread_create(&threads[index], NULL, finish_race
        , &private_data[index]);

      if (error) {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        error = 21;
      }
    }

    for (size_t index = 0; index < thread_count; ++index) {
      pthread_join(threads[index], NULL);
    }

    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "error: could not allocate memory for %zu threads\n"
      , shared_data->team_count);
    error = 22;
  }

  return error;
}

void* start_race(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  const size_t rank = private_data->thread_number;
  const size_t team_number = rank;

  pthread_barrier_wait(&shared_data->start_barrier);
  usleep(1000 * shared_data->stage1_duration);
  sem_post(&shared_data->batons[team_number]);

  return NULL;
}

void* finish_race(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  shared_data_t* shared_data = private_data->shared_data;

  const size_t rank = private_data->thread_number;
  // se averigua el equipo al restar el total de equipos con el numero del hilo
  const size_t team_number = rank - shared_data->team_count;
  assert(team_number < shared_data->team_count);

  // wait(batons[team_number])
  sem_wait(&shared_data->batons[team_number]);
  usleep(1000 * shared_data->stage2_duration);

  pthread_mutex_lock(&shared_data->finish_mutex);
  const size_t our_position = ++shared_data->position;
  // if (our_position <= 3) {
    printf("Place %zu: team %zu\n", our_position, team_number);
  // }
  pthread_mutex_unlock(&shared_data->finish_mutex);

  return NULL;
}
