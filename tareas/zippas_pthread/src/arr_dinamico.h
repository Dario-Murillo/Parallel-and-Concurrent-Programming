// Copyright 2023 <Dario Murillo Chaverri C15406>
#ifndef ARR_DINAMICO_H
#define ARR_DINAMICO_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

/**
 * @struct arr_dinamico
 * @brief estructura de memoria dinamica que funciona
 * como un arreglo para datos tipo char*
 * 
 * @var arr_dinamico::capacidad
 * capacidad actual de arreglo 
 * 
 * @var arr_dinamico::total
 * capacidad total del arreglo
 * 
 * @var arr_dinamico::array
 * array son los elementos de dicho arreglo
*/
typedef struct arr_dinamico {
  size_t capacidad;
  size_t total;
  char** array;
} arr_dinamico_t;



/**
 * @brief inicializa los valores del struct
 * @details reserva memoria inicial para el primer elemento
 * @param arr puntero a la variable de tipo arr_dinamico_t
 * @return retorna codigo de error 
*/
int arreglo_innit(arr_dinamico_t* arr);


/**
 * @brief libera los alojamientos de memoria 
 * @param arr puntero a la variable de tipo arr_dinamico_t
 * @return void
*/
void arreglo_destroy(arr_dinamico_t* arr);


/**
 * @brief agrega un nuevo elemento de tipo char* al arreglo
 * @param arr  puntero a la variable de tipo arr_dinamico_t
 * @param elemento cadena de caracteres que se agregara al arreglo
 * @return codigo de error 
*/
int arreglo_agregar(arr_dinamico_t* arr, const char* elemento);

#endif
