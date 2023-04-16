// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef COMMON_H
#define COMMON_H


#include <stdlib.h>
#include <semaphore.h>
#include <unistd.h>
#include "queue.h"

enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_UNIT_COUNT,
  ERR_PRODUCER_COUNT,
  ERR_CONSUMER_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};

typedef struct simulation {
  size_t unit_count;
  size_t producer_count;
  size_t consumer_count;
  __useconds_t producer_min_delay;
  __useconds_t producer_max_delay;
  __useconds_t consumer_min_delay;
  __useconds_t consumer_max_delay;
  queue_t queue;

  pthread_mutex_t can_access_next_unit;  // mutex de la unidades
  size_t next_unit;
  sem_t can_consume;  // semaforo para consumir
  pthread_mutex_t can_access_consumed_count;  // mutex de los consumidores
  size_t consumed_count;  // cantidad de elementos consumidos
} simulation_t;

/**
 * @brief genera un numero pseudoaleatorio
 * @param min minimo numero a generar
 * @param max maximo numero a generar
 * @return numero pseudoaleatorio
 * 
*/
__useconds_t random_between(__useconds_t min, __useconds_t max);

#endif  // COMMON_H
