// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <pthread.h>
#include <stdlib.h>
#include <sys/random.h>
#include <stdio.h>

/**  
 * esta es una version alterna y fallida del ejercicio devolviendo 
 * la dirrecion de memoria de los numeros random 
 * por medio de herramientas de optimizacion como asan 
 * nos damos cuenta de que esta implementacion del codigo en 
 *  la cual devolvemos la dirrecion de memoria de la variable 
 *  no produce el resultado correcto
 * 
*/


// declaracion de la subrutina number
void* number(void* data);

/**
 * @brief subrutina principal encargada de crear y destruir ambos hilos
*/
int main(void) {
    int* numero_ran1 = NULL;
    unsigned int seed1 =  time(NULL);  // generamos una 'seed'
    pthread_t* threads = (pthread_t*) malloc(2 * sizeof(pthread_t));
    for (int thread_num = 0; thread_num < 2; thread_num++) {
      pthread_create(&threads[thread_num], NULL, number, (void*) &seed1);
    }
    for (int thread_num = 0; thread_num < 2; thread_num++) {
      pthread_join(threads[thread_num], (void*) &numero_ran1);
      printf("Numero del hilo %d: %ls\n" , thread_num, numero_ran1);
    }
    free(threads);  // liberar la memoria dinamica
    free(numero_ran1);
}  //  end main


/**
 * @brief subrutina encargada de devolver el numero generar
 * numero pseudoaleatorio y retornarlo como una falsa direccion
 * de memoria
 * @param data
*/
void* number(void* data) {
  unsigned int *seed = data; // recibir la seed unica
  int number = rand_r(seed) % 99; // evitar que el numero sea mayor a 99
  int* pointer = (int*) malloc(number * sizeof(int));
  return pointer;
} // end number
