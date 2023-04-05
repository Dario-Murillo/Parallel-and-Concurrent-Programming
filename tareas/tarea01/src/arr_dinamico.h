// Copyright 2023 <Dario Murillo Chaverri C15406>
#ifndef ARR_DINAMICO_H
#define ARR_DINAMICO_H

#include <stddef.h>
#include <stdlib.h>
#include <stdio.h>

typedef struct {
  size_t capacidad;
  size_t total;
  char** array;
} arr_dinamico_t;


void arreglo_imprimir(arr_dinamico_t* arr);
int arreglo_innit(arr_dinamico_t* arr);
void arreglo_destroy(arr_dinamico_t* arr);
int arreglo_agregar(arr_dinamico_t* arr, const char* elemento);

#endif
