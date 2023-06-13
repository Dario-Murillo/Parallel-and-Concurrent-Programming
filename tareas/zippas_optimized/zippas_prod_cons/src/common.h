// Copyright 2023 <Dario Murillo Chaverri C15406>
#define _DEFAULT_SOURCE

#ifndef COMMON_H
#define COMMON_H

#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>
#include <semaphore.h>
#include "arr_dinamico.h"
#include "queue.h"

/**
 * @struct datos
 * @brief estructura encargada de recibir y manipular los datos del input
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
 * zips guarda la tercer linea de los lotes de prueba
 * las dirreciones de los archivos zips
 * 
 * @var datos::contrasenas  
 * contrasenas guarda las contrasenas que 
 * desencriptan los zips
 * 
 * @var datos::thread_count
 * cantidad de hilos que se usaran
 * 
 * @var datos::cola
 * cola thread-safe para poder almacenar claves
 * 
 * @var datos::can_consume
 * semaforo que inidica a los consumidores
 * cuando pueden comenzar a consumir
 * 
 * @var datos::abrir_archivo
 * mutex para la apertura de archivos zip
 * 
 * @var datos::cambiar_variable
 * mutex para modificar memoria compartida
 * 
 * @var datos::leer_variable
 * mutex para leer memoria compartida
 * 
 * @var datos::contador_pass
 * indica cuantas claves se han encontrado
 * 
 * @var datos::claves
 * matriz de caracteres que almacena las claves
 * encontradas para desencriptar archivos
 * 
*/
typedef struct datos {
  arr_dinamico_t alfabeto;
  uint64_t limite;
  arr_dinamico_t zips;
  arr_dinamico_t contrasenas;
  uint64_t thread_count;
  queue_t cola;
  sem_t can_consume;
  pthread_mutex_t abrir_archivo;
  pthread_mutex_t cambiar_variable;
  size_t contador_pass;
  char claves[100][100];
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
 * @var datos_privados::datos_compartidos
 * puntero a la memoria compartida
 * 
*/
typedef struct datos_privados {
  pthread_t thread;
  arr_dinamico_t archivos;
  datos_t* datos_compartidos;
} datos_privados_t;

#endif  // COMMON_H
