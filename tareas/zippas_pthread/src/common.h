// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <pthread.h>
#include <stdbool.h>
#include "arr_dinamico.h"

#ifndef COMMON_H
#define COMMON_H

/**
 * @struct datos
 * @brief estructura encargada de recibir y manipular los datos del imput
 * 
 * @var datos::alfabeto
 * alfabeto guarda la primer linea de los lotes de prueba 
 * los caracteres con los que se va a trabajar
 * 
 * @var datos::limite
 * limite guarda la segunda linea de los lotes de prueba
 * el limite de largo de la contrasena
 * 
 * @var datos::zips
 * zips guarda la tercer lines de los lotes de prueba
 * las dirreciones de los archivos zips
 * 
 * @var datos::contrasenas  
 * contrasenas guarda las contrasenas que 
 * desencriptan los zips
 * 
*/
typedef struct datos {
  arr_dinamico_t alfabeto;
  uint64_t limite;
  arr_dinamico_t zips;
  arr_dinamico_t contrasenas;
  uint64_t thread_count;
  pthread_mutex_t mutex;
  bool encontroPass;
  int64_t insercion;
} datos_t;

/**
 * @struct datos_privados
 * @brief estructura de memoria privada para cada hilo
 * 
 * @var datos_privados::carga
 * cantidad de claves que pueden producir
 * 
 * @var datos_privados::thread
 * hilo de ejecuccion
 * 
 * @var datos_privados::datos_compartidos
 * puntero a la memoria compartida
 * 
*/
typedef struct datos_privados {
  int64_t carga;
  pthread_t thread;
  arr_dinamico_t carga_inicio;
  arr_dinamico_t carga_final;
  datos_t* datos_compartidos;
} datos_privados_t;

#endif  // COMMON_H
