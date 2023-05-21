// Copyright 2023 <Dario Murillo Chaverri C15406>
#define _DEFAULT_SOURCE

#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>
#include <semaphore.h>
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
 * variable par indicar a los hilos si la  clave de un archivo fue encontrada
 * 
 * @var datos::abrir_archivo
 * mutex para la apertura de archivos zip
 * 
 * @var datos::escribir archivo
 * mutes para indexar el archivo zip
 * 
 * @var datos::insercion
 * contador de cuantas veces se ha introducido una clave al
 * arreglo, usada para evitar multiples inserciones de la 
 * misma clave
 * 
 * @var datos::barrera_limite
 * contador de hilos que han llegado a la barrera
 * 
 * @var datos::acceso
 * semaforo que bloque barrera_limite
 * 
 * @var datos::barrera
 * semaforo que cumple la funcionalidad de una barrera
 * 
 * @var datos::ultima_clave
 * variable para identificar la ultima clave posible
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
  pthread_mutex_t abrir_archivo;
  pthread_mutex_t escribir_archivo;
  int64_t insercion;
  uint64_t barrera_limite;
  sem_t acceso;
  sem_t barrera;
  char* ultima_clave;
} datos_t;

/**
 * @struct datos_privados
 * @brief estructura de memoria privada para cada hilo
 * 
 * @var datos_privados::thread
 * hilo de ejecuccion
 * 
 * @var datos_privados::archivos
 * arreglo de copias unicas de archivos zip
 * 
 * @var datos_privados::carga_inicio
 * arreglo que contiene el rango inicial de trabajo
 * 
 * @var datos_privados::carga_final
 * arreglo que contiene el rango final de trabajo
 * 
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
