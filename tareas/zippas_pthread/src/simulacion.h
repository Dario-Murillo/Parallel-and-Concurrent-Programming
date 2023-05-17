// Copyright 2023 <Dario Murillo Chaverri C15406>
#ifndef SIMULACION_H
#define SIMULACION_H

#include "common.h"
#include "arr_dinamico.h"

typedef struct datos datos_t;
/**
 * @brief reserva memoria para crear una variable de tipo datos_t
 * @return variable de tipo datos_t
*/
datos_t* datos_create();

/**
 * @brief inicia los miembros del struct datos
 * @param datos puntero de una variable tipo datos_t
 * @return void
*/
void datos_innit(datos_t* datos);

/**
 * @brief controlador principal de la simulacion
 * 
*/
int datos_run(datos_t* datos, FILE* input, int argc, char* argv[]); 


/**
 * @brief libera la memoria usada
 * @see datos_innit
*/
void datos_destroy(datos_t* datos);

#endif
