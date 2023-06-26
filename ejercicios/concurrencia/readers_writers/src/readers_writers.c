// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <pthread.h>
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

typedef struct shared_data {
  sem_t can_access_medium;
  sem_t can_access_reading;
  int buffer;  // buffer para simular la escritura
  int reading_count;
} shared_data_t;

/// @brief Subrutina que efectua el equipo de hilos lectores
/// @param data memoria privada y compartida
void* reader(void* data);

/// @brief Subrutina que efectua el equipo de escritores
/// @param data memoria privada y compartida
void* writer(void* data);

int main() {
  char aux = '\0';
  shared_data_t* shared_data = (shared_data_t*)
    calloc(1, sizeof(shared_data_t));
  sem_init(&shared_data->can_access_medium, 0, 1);
  sem_init(&shared_data->can_access_reading, 0, 1);
  shared_data->buffer = 0;
  shared_data->reading_count = 0;

  // Leemos de la entrada estandar cuantos lectores y escritores queremos
  while (read(STDIN_FILENO, &aux, 1) > 0) {
    if (aux == 'R') {
      pthread_t thread;
      pthread_create(&thread, 0, reader, &shared_data[0]);
      pthread_join(thread, NULL);
    } else if (aux == 'W') {
      pthread_t thread;
      pthread_create(&thread, 0, writer, &shared_data[0]);
      pthread_join(thread, NULL);
    }
  }
  sem_destroy(&shared_data->can_access_medium);
  sem_destroy(&shared_data->can_access_reading);
  free(shared_data);
  return 0;
}

void* reader(void* data) {
  shared_data_t* shared_data = (shared_data_t*) data;
  sem_wait(&shared_data->can_access_reading);
  shared_data->reading_count = shared_data->reading_count + 1;
  if (shared_data->reading_count == 1) {
    sem_wait(&shared_data->can_access_medium);
  }
  sem_post(&shared_data->can_access_reading);
  printf("Number %d \n", shared_data->buffer);
  sem_wait(&shared_data->can_access_reading);
  shared_data->reading_count = shared_data->reading_count - 1;
  if (shared_data->reading_count == 0) {
    sem_post(&shared_data->can_access_medium);
  }
  sem_post(&shared_data->can_access_reading);
  return NULL;
}

void* writer(void* data) {
  shared_data_t* shared_data = (shared_data_t*) data;
  sem_wait(&shared_data->can_access_medium);
  shared_data->buffer++;
  sem_post(&shared_data->can_access_medium);
  return NULL;
}
