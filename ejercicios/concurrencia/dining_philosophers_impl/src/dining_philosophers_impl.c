// Copyright 2023 <Dario Murillo Chaverri C15406>
#define _DEFAULT_SOURCE

#include <unistd.h>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <stdbool.h>
#include <semaphore.h>

typedef struct shared_data {
  sem_t* chopsticks;
  useconds_t min_wait;
  useconds_t max_wait;
  int philospher_count;
} shared_data_t;

typedef struct private_data {
  int id;
  pthread_t philosopher;
  shared_data_t* shared;
} private_data_t;


/// @brief Procedimiento que llevan a cabo los filosofos
void* philosopher(void* data);

/// @brief Funcion que simula el pensar, imprime un mensaje
/// con el numero del filoso o hilo efectuando la accion
/// @param id identificador o numero del filosofo
void think(int id);

/// @brief Funcion que simula el comer, imprime un mensaje
/// con el numero del filoso o hilo efectuando la accion
/// @param id identificador o numero del filosofo
void eat(int id);

/// @brief Funcion para agarrar el palillo izquierdo
/// @param private_data puntero a memoria privada
void get_left_chopstick(private_data_t* private_data);

/// @brief Funcion para agarrar el palillo derecho
/// @param private_data puntero a memoria privada
void get_right_chopstick(private_data_t* private_data);

/// @brief Funcion para poner el palillo izquierdo en la mesa
/// @param private_data puntero a memoria privada
void put_left_chopstick(private_data_t* private_data);

/// @brief Funcion para poner el palillo derecho en la mesa
/// @param private_data puntero a memoria privada
void put_right_chopstick(private_data_t* private_data);

/// @brief Produce un numero aleatorio en el rango dado
/// @param min cota inferior
/// @param max cota superior
/// @return numero aleatorio
useconds_t random_between(useconds_t min, useconds_t max);

int main(int argc, char *argv[]) {
  if (argc == 4) {
    int philosopher_count = atoi(argv[1]);
    int min_wait = atoi(argv[2]);
    int max_wait = atoi(argv[3]);


    shared_data_t* shared_data =
      (shared_data_t*) calloc(1, sizeof(shared_data_t));
    shared_data->chopsticks = (sem_t*) calloc(philosopher_count, sizeof(sem_t));
    shared_data->philospher_count = philosopher_count;
    shared_data->min_wait = min_wait;
    shared_data->max_wait = max_wait;

    private_data_t* private_data = (private_data_t*)
      calloc(philosopher_count, sizeof(private_data_t));

    for (int i = 0; i < shared_data->philospher_count ; i++) {
      private_data[i].id = i;
      private_data[i].shared = shared_data;
      sem_init(&shared_data->chopsticks[i], 0, 1);
      pthread_create(&private_data[i].philosopher,
        0, philosopher, &private_data[i]);
    }

    for (int i = 0; i < shared_data->philospher_count ; i++) {
      pthread_join(private_data[i].philosopher, NULL);
      sem_destroy(&shared_data->chopsticks[i]);
    }

    free(private_data);
    free(shared_data->chopsticks);
    free(shared_data); 
  } else {
    printf("Usage\n 1. Philosopher Count\n 2. Max delay\n 3.Min delay\n");
  }
  return 0;
}

void* philosopher(void* data) {
  private_data_t* private_data = (private_data_t*) data;
  shared_data_t* shared_data = private_data->shared;
  while (true) {
    think(private_data->id);
    usleep(1000 *
      random_between(shared_data->min_wait, shared_data->max_wait));
    if (private_data->id % 2 == 0) {
      get_right_chopstick(private_data);
      get_left_chopstick(private_data);
    } else {
      get_left_chopstick(private_data);
      get_right_chopstick(private_data);
    }
    eat(private_data->id);
    usleep(1000 *
      random_between(shared_data->min_wait, shared_data->max_wait));
    put_left_chopstick(private_data);
    put_right_chopstick(private_data);
  }
  return NULL;
}

void think(int id) {
  printf("Philosopher %d is thinking\n", id);
}

void eat(int id) {
  printf("Philosopher %d is eating\n", id);
}

void get_left_chopstick(private_data_t* private_data) {
  shared_data_t* shared_data = private_data->shared;
  sem_wait(&shared_data->chopsticks
  [(private_data->id + 1) % shared_data->philospher_count]);
}

void get_right_chopstick(private_data_t* private_data) {
  shared_data_t* shared_data = private_data->shared;
  sem_wait(&shared_data->chopsticks[(private_data->id)]);
}

void put_left_chopstick(private_data_t* private_data) {
  shared_data_t* shared_data = private_data->shared;
  sem_post(&shared_data->chopsticks
  [(private_data->id + 1) % shared_data->philospher_count]);
}

void put_right_chopstick(private_data_t* private_data) {
  shared_data_t* shared_data = private_data->shared;
  sem_post(&shared_data->chopsticks[(private_data->id)]);
}

useconds_t random_between(useconds_t min, useconds_t max) {
  return min + (max > min ? (random() % (max - min)) : 0);
}
