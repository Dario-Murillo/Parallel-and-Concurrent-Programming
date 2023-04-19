// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef CONSUMER_H
#define CONSUMER_H

/**
 * @brief consume un numero producido por los productores
 * al quitarlo de la cola 
 * @param data recibe los datos necesarios
 * almacenados en las estructura encargada de la simulacion
*/
void* consume(void* data);

#endif  // CONSUMER_H
