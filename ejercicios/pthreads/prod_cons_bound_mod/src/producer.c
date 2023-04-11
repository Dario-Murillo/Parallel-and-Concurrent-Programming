// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <producer.h>
#include <simulation.h>

void* produce(void* data) {
  // const private_data_t* private_data = (private_data_t*)data;
  simulation_t* simulation = (simulation_t*)data;
  size_t count = 0;
  // cantidad de rondas
  for (size_t round = 0; round < simulation->rounds; ++round) {
    // cantidad del buffer
    for (size_t index = 0; index < simulation->buffer_capacity; ++index) {
        // espere a que haya algo por producir
        sem_wait(&simulation->can_produce);

        // duerme por una cantidad pseudoaleatoria de microsegundos
        usleep(1000 * random_between(simulation->producer_min_delay
            , simulation->producer_max_delay));
        // agregar el numero al buffer
        simulation->buffer[index] = ++count;
        printf("Produced %lg\n", simulation->buffer[index]);

        // una vez producido se indica que hay datos que se pueden consumir
        sem_post(&simulation->can_consume);
    }
  }

  return NULL;
}
