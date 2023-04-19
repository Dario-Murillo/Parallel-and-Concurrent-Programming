// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#define _XOPEN_SOURCE 500
#include <stdio.h>
#include <stdbool.h>

#include "common.h"
#include "consumer.h"

void* consume(void* data) {
  private_data_t* private_data = (private_data_t*)data;
  simulation_t* simulation = private_data->shared;

  while (true) {
    // lock(can_access_consumed_count)
    pthread_mutex_lock(&simulation->can_access_consumed_count);
    if (simulation->consumed_count < simulation->unit_count) {
      // Reserve the next product to me
      ++simulation->consumed_count;
    } else {
      // unlock(can_access_consumed_count)
      pthread_mutex_unlock(&simulation->can_access_consumed_count);
      // break while
      break;
    }
    // unlock(can_access_consumed_count)
    pthread_mutex_unlock(&simulation->can_access_consumed_count);

    // wait(can_consume)
    sem_wait(&simulation->can_consume);

    size_t value = 0;
    queue_dequeue(&simulation->queue, &value);
    printf("\t Consumer %zu consumed %zu\n", private_data->thread_number,
    value);
    usleep(1000 * random_between(simulation->consumer_min_delay
      , simulation->consumer_max_delay));
  }

  return NULL;
}
