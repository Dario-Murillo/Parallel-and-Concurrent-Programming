// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#define _XOPEN_SOURCE 500

#include <stdio.h>
#include "producer.h"
#include <common.h>

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  simulation_t* simulation = (simulation_t*)data;

  while (true) {

    size_t my_unit = 0;
    if (sem_trywait(&simulation->can_produce)) {
      
      sem_wait(&simulation->can_acces_unit);
      my_unit = ++simulation->next_unit;
      sem_post(&simulation->can_acces_unit);
      

      usleep(1000 * random_between(simulation->producer_min_delay
        , simulation->producer_max_delay));
      queue_enqueue(&simulation->queue, my_unit);
      printf("Produced %zu\n", my_unit);

      // signal(can_consume)
      sem_post(&simulation->can_consume);
      sem_post(&simulation->can_produce);
    } else {

    }
  }

  return NULL;
}
