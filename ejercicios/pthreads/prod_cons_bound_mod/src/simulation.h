// Copyright 2023 <Dario Murillo Chaverri C15406>
#define _DEFAULT_SOURCE


#ifndef SIMULATION_H
#define SIMULATION_H


#include <assert.h>
#include <errno.h>
#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/random.h>
#include <unistd.h>


enum {
  ERR_NOMEM_SHARED = EXIT_FAILURE + 1,
  ERR_NOMEM_BUFFER,
  ERR_NO_ARGS,
  ERR_BUFFER_CAPACITY,
  ERR_ROUND_COUNT,
  ERR_MIN_PROD_DELAY,
  ERR_MAX_PROD_DELAY,
  ERR_MIN_CONS_DELAY,
  ERR_MAX_CONS_DELAY,
  ERR_CREATE_THREAD,
};


typedef struct simulation {
  size_t thread_count;
  size_t buffer_capacity;
  double* buffer;
  size_t rounds;
  useconds_t producer_min_delay;
  useconds_t producer_max_delay;
  useconds_t consumer_min_delay;
  useconds_t consumer_max_delay;
  sem_t can_produce;
  sem_t can_consume;
} simulation_t;

/**
 * @brief inicializa memoria
 * @return puntero a la memoria iniciada
*/
simulation_t* simulation_create();

/**
 * @brief subrutina principal que activa las demas
 * @param simulation puntero a la memoria con los datos
 * @param argc numero de argumentos
 * @param argv argumentos
 * @return codigo de error, 0 en caso exitoso
*/
int simulation_run(simulation_t* simulation, int argc, char* argv[]);


/**
 * @brief libera la memoria ocupada
 * @param simulation puntero a la memoria con los datos
*/
void simulation_destroy(simulation_t* simulation);

/**
 * @brief genera un numero pseudoaleatorio
 * @param min minimo numero a generar
 * @param max maximo numero a generar
 * @return numero random
 * 
*/
useconds_t random_between(useconds_t min, useconds_t max);

#endif
