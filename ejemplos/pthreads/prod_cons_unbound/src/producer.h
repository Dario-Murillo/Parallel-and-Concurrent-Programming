// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef PRODUCER_H
#define PRODUCER_H

/**
 * @brief produce un producto y lo agrega a la cola
 * @param data recibe los datos necesarios
 * almacenados en las estructura encargada de la simulacion
*/
void* produce(void* data);

#endif  // PRODUCER_H
