// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
#define _DEFAULT_SOURCE

#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <stdlib.h>
#include <sys/random.h>
#include <stdio.h>

#include "common.h"
#include "consumer.h"
#include "producer.h"
#include "simulation.h"

/**
 * @brief carga los datos obtenidos a la estructura de datos
 * @param simulation puntero al registro simulation
 * @param argc numero de argumentos
 * @param argv argumentos
 * @return codigo de error 0 en caso exitoso
*/
int analyze_arguments(simulation_t* simulation, int argc, char* argv[]);

/**
 * @brief crea y destruye los consumidores y productores
 * @param simulation puntero a la estructura de datos
 * @return codigo de error 0 en caso exitoso
*/
int create_consumers_producers(simulation_t* simulation);

/**
 * @brief crea un arreglo de memoria privada
 * @param cantidad cantidad de hilos
 * @param subroutine subrutina sobre la cual van a operar los hilos
 * @param data la informacion que se va a mandar con los hilos
 * @return devuelve un arreglo con la memoria privada
*/
private_data_t* create_threads(simulation_t* simulation,
size_t count, void*(*subroutine)(void*));

/**
 * @brief destruye los hilos y la memoria privada alojada
 * @param count numero de hilos
 * @param threads puntero a la memoria privada
 * @return codigo de error 0 en caso exitoso
*/
int join_threads(size_t count, private_data_t* threads);

simulation_t* simulation_create() {
  simulation_t* simulation = (simulation_t*) calloc(1, sizeof(simulation_t));
  if (simulation) {
    simulation->unit_count = 0;
    simulation->producer_count = 0;
    simulation->consumer_count = 0;
    simulation->producer_min_delay = 0;
    simulation->producer_max_delay = 0;
    simulation->consumer_min_delay = 0;
    simulation->consumer_max_delay = 0;
    queue_init(&simulation->queue);
    simulation->next_unit = 0;
    sem_init(&simulation->can_consume, /* pshared */ 0, /* value */ 0);
    pthread_mutex_init(&simulation->can_access_next_unit, /* attr */ NULL);
    pthread_mutex_init(&simulation->can_access_consumed_count, /* attr */ NULL);
    simulation->consumed_count = 0;
  }
  return simulation;
}

void simulation_destroy(simulation_t* simulation) {
  assert(simulation);
  pthread_mutex_destroy(&simulation->can_access_consumed_count);
  pthread_mutex_destroy(&simulation->can_access_next_unit);
  sem_destroy(&simulation->can_consume);
  queue_destroy(&simulation->queue);
  free(simulation);
}

int simulation_run(simulation_t* simulation, int argc, char* argv[]) {
  int error = analyze_arguments(simulation, argc, argv);
  if (error == EXIT_SUCCESS) {
    unsigned int seed = 0;
    getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
    srandom(seed);

    struct timespec start_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

    error = create_consumers_producers(simulation);

    struct timespec finish_time;
    clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

    double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
      (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
    printf("execution time: %.9lfs\n", elapsed);
  }
  return error;
}

int analyze_arguments(simulation_t* simulation, int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (argc == 8) {
    if (sscanf(argv[1], "%zu", &simulation->unit_count) != 1
      || simulation->unit_count == 0) {
        fprintf(stderr, "error: invalid unit count\n");
        error = ERR_UNIT_COUNT;
    } else if (sscanf(argv[2], "%zu", &simulation->producer_count) != 1
      || simulation->producer_count == 0) {
        fprintf(stderr, "error: invalid producer count\n");
        error = ERR_PRODUCER_COUNT;
    } else if (sscanf(argv[3], "%zu", &simulation->consumer_count) != 1
      || simulation->consumer_count == 0) {
        fprintf(stderr, "error: invalid consumer count\n");
        error = ERR_CONSUMER_COUNT;
    } else if (sscanf(argv[4], "%u", &simulation->producer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min producer delay\n");
        error = ERR_MIN_PROD_DELAY;
    } else if (sscanf(argv[5], "%u", &simulation->producer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max producer delay\n");
        error = ERR_MAX_PROD_DELAY;
    } else if (sscanf(argv[6], "%u", &simulation->consumer_min_delay) != 1) {
        fprintf(stderr, "error: invalid min consumer delay\n");
        error = ERR_MIN_CONS_DELAY;
    } else if (sscanf(argv[7], "%u", &simulation->consumer_max_delay) != 1) {
        fprintf(stderr, "error: invalid max consumer delay\n");
        error = ERR_MAX_CONS_DELAY;
    }
  } else {
    fprintf(stderr, "usage: unit_count producer_count"
      " consumer_count producer_min_delay producer_max_delay"
      " consumer_min_delay consumer_max_delay\n");
      error = ERR_NO_ARGS;
  }
  return error;
}

private_data_t* create_threads(simulation_t* simulation, size_t count,
void*(*subroutine)(void*)) {
  private_data_t* private_data =
  (private_data_t*) calloc(count, sizeof(private_data_t));
  if (private_data) {
    for (size_t index = 0; index < count; ++index) {
      private_data[index].thread_number = index;
      private_data[index].thread_count = count;
      private_data[index].shared = simulation;
      if (pthread_create(&private_data[index].thread, /*attr*/ NULL,
      subroutine, &private_data[index]) == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "error: could not create thread %zu\n", index);
        join_threads(index, private_data);
        return NULL;
      }
    }
  }
  return private_data;
}

int join_threads(size_t count, private_data_t* private_data) {
  int error = EXIT_SUCCESS;
  for (size_t index = 0; index < count; ++index) {
    // todo: sum could not be right
    error += pthread_join(private_data[index].thread, /*value_ptr*/ NULL);
  }
  free(private_data);
  return error;
}

int create_consumers_producers(simulation_t* simulation) {
  assert(simulation);
  int error = EXIT_SUCCESS;

  private_data_t* producers = create_threads(simulation,
  simulation->producer_count, produce);
  private_data_t* consumers = create_threads(simulation,
  simulation->consumer_count, consume);

  if (producers && consumers) {
    join_threads(producers->thread_count, producers);
    join_threads(consumers->thread_count, consumers);
  } else {
    fprintf(stderr, "error: could not create threads\n");
    error = ERR_CREATE_THREAD;
  }

  return error;
}
