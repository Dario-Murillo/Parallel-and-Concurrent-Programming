// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef SIMULATION_H
#define SIMULATION_H

typedef struct simulation simulation_t;  // opaque record
// equivalente a un registro "privado"

/**
 * @brief crea un registro de tipo simulation_t
 * e inicializa los datos
 * @return puntero al registro creado
*/
simulation_t* simulation_create();

/**
 * @brief se encarga de correr las subrutinas de la simulacion
 * @param simulation puntero al registro simulation
 * @param argc numero de argumentos
 * @param argv argumentos
 * @return codigo de error 0 en caso exitoso
*/
int simulation_run(simulation_t* simulation, int argc, char* argv[]);

/**
 * @brief libera la memoria ocupada por la simulacion
 * @param simulation puntero al registro simulation
*/
void simulation_destroy(simulation_t* simulation);

#endif  // SIMULATION_H
