// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

/**
 * @brief Hay que declarar la funcion primero, si no el compilador no lo 
 * reconocera, la funcion que se encarga de saludar desde el segundo hilo
 * @param data
 */
void* greet(void* data);

/**
 * @brief Este es el metodo prinicipal el cual se encarga de 
 * crear el segundo hilo, e imprimir el primer saludo
 * 
*/
int main(void) {
  pthread_t thread;
  size_t number = 2;
  int error = pthread_create(&thread, /*attr*/ NULL, greet
  , /*arg*/ (void*)number);
  if (error == EXIT_SUCCESS) {
  printf("Hello from main thread\n");
  pthread_join(thread, /*value_ptr*/ NULL);  // liberacion de memoria
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n");
  }
  return error;
}  // end procedure

/**
 * @brief Segunda subrutina que es llamada a la hora de crear el segundo hilo 
 * se llamara recursivamente 
 * @param data
*/
void* greet(void* data) {
  size_t numero = (size_t)data;  // casteo para el numero
  pthread_t recursive;
  if (numero == 0) {
    printf("\nBye from secondary thread, number received:%d\n", (int)numero);
  } else if (numero > 0) {
    printf("\nHello from secondary thread, number received:%d", (int)numero);
    --numero;  // restar 1 al numero
    pthread_create(&recursive, NULL, greet, (void*)numero);
    // creacion de hilo que llama a la subrutina greet recursivamente
    pthread_join(recursive, NULL);  // liberacion de la memoria
  }
  return NULL;
}  // end procedure
