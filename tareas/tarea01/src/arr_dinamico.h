#ifndef ARR_DINAMICO_H
#define ARR_DINAMICO_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  size_t capacidad;
  size_t total;
  char* elementos;
} arr_dinamico_t;


void arreglo_imprimir(arr_dinamico_t* arr);
int arreglo_innit(arr_dinamico_t* arr);
void arreglo_destroy(arr_dinamico_t* arr);
int arreglo_agregar(arr_dinamico_t* arr, char elemento);

#endif 