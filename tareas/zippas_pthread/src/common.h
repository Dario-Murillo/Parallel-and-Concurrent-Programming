// Copyright 2023 <Dario Murillo Chaverri C15406>
#define _DEFAULT_SOURCE

#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>
#include "arr_dinamico.h"


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
 * @var datos::thread_count
 * cantidad de hilos que se usaran
 * 
 * @var datos::mutex
 * mutex para evitar condiciones de carrera a la hora de
 * modificar memoria compartida
 * 
 * @var datos::encontroPass
 * variable booleana encarga de indicar a los hilos si la 
 * clave de un archivo fue encontrada
 * 
 * @var datos::insercion
 * contador de cuantas veces se ha introducido una clave al
 * arreglo, usada para evitar multiples inserciones de la 
 * misma clave
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
  pthread_barrier_t barrera;
} datos_t;

/**
 * @struct datos_privados
 * @brief estructura de memoria privada para cada hilo
 * 
 * @var datos_privados::thread
 * hilo de ejecuccion
 * 
 * @var datos_privados::archivos
 * arreglo de las copias unicas sobre las cuales los
 * hilos van a trabajar
 * 
 * @var datos_privados::carga_inicio
 * arreglo que contiene el rango inicial sobre el cual
 * los hilos van a trabajar 
 * 
 * @var datos_privados::carga_final
 * arreglo que contiene el rango final sobre el cual
 * los hilos van a trabajar 
 * @var datos_privados::datos_compartidos
 * puntero a la memoria compartida
 * 
*/
typedef struct datos_privados {
  pthread_t thread;
  arr_dinamico_t archivos;
  arr_dinamico_t carga_inicio;
  arr_dinamico_t carga_final;
  datos_t* datos_compartidos;
} datos_privados_t;

#endif  // COMMON_H
