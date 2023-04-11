// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <consumer.h>
#include <simulation.h>

void* consume(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  simulation_t* simulation = (simulation_t*)data;
  for (size_t round = 0; round < simulation->rounds; ++round) {
    for (size_t index = 0; index < simulation->buffer_capacity; ++index) {
        // espera a que haya algo por consumir
        sem_wait(&simulation->can_consume);

      // extrae el valor del buffer
        double value = simulation->buffer[index];
        usleep(1000 * random_between(simulation->consumer_min_delay
            , simulation->consumer_max_delay));
        printf("\tConsumed %lg\n", value);

        // si logre consumir algo significa que
        // queda un espacio vacio disponible para consumir
        sem_post(&simulation->can_produce);
    }
  }

  return NULL;
}
