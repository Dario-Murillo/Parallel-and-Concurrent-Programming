// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

/**
 * @brief Hay que declarar la funcion primero, si no el compilador no lo 
 * reconocera, esta es la funcion que se encarga 
 * de saludar desde el segundo hilo
 * @param data
 */
void* greet(void* data);

/**
 * @brief Este es el metodo prinicipal el cual se encarga de crear 
 * el segundo hilo, e imprimir el primer saludo
 * 
*/
int main(void) {
  // create_thread(greet)
  pthread_t thread;  //  Declaracion del hilo
  //  Llamado de funcion
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ NULL);
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");
    //  Liberacion de memoria en caso de que se haya creado con exito
    pthread_join(thread,  /*value_ptr*/   NULL);
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

/**
 * @brief Segunda subrutina que es llamada a la hora de crear el segundo hilo
 * @param data (void)
*/
void* greet(void* data) {
  (void)data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread\n");
  return NULL;
}  // end procedure
