// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <pthread.h>
#include <stdlib.h>
#include <inttypes.h>
#include <stdio.h>

// declaracion de la subrutina number
void* number(void* data);

/**
 * @brief subrutina principal encargada de crear y destruir ambos hilos
*/
int main(void) {
    uint64_t numero_ran1 = 0;
    unsigned int seed1 =  time(NULL);  // generamos una 'seed'
    pthread_t* threads = (pthread_t*) malloc(2 * sizeof(pthread_t));
    for (int thread_num = 0; thread_num < 2; thread_num++) {
      pthread_create(&threads[thread_num], NULL, number, (void*) &seed1);
    }
    for (int thread_num = 0; thread_num < 2; thread_num++) {
      pthread_join(threads[thread_num],  (void*) &numero_ran1);
      printf("Numero del hilo %d: "  "%" PRIu64 "\n", thread_num, numero_ran1);
    }
    free(threads);  // liberar la memoria dinamica
}  //  end main


/**
 * @brief subrutina encargada de devolver el numero generar
 * numero pseudoaleatorio y retornarlo como una falsa direccion
 * de memoria
 * @param data
*/
void* number(void* data) {
  unsigned int *seed = data;  // recibir la seed unica
  int number = rand_r(seed) % 99;  // evitar que el numero sea mayor a 99
  uint64_t cast = (uint64_t) number;
  void* retorno = (void*) cast;
  return retorno;
}  // end number



/**  
 *  Esta es una version alterna del ejercicio devolviendo 
 *  la dirrecion de memoria de los numeros random 
 *  
*/

// void* number(void* data);

// /**
//  * @brief subrutina principal encargada de crear y destruir ambos hilos
// */
// int main(void) {
//     uint64_t* numero_ran1;
//     unsigned int seed1 =  time(NULL);  // generamos una 'seed'
//     pthread_t* threads = (pthread_t*) malloc(2 * sizeof(pthread_t));
//     for (int thread_num = 0; thread_num < 2; thread_num++) {
//       pthread_create(&threads[thread_num], NULL, number, (void*) &seed1);
//     }
//     for (int thread_num = 0; thread_num < 2; thread_num++) {
//       pthread_join(threads[thread_num], (void**) &numero_ran1);
//       printf("Numero del hilo %d: %zu\n" ,thread_num , *numero_ran1);
//       free(numero_ran1);
//     }
//     free(threads);  // liberar la memoria dinamica
// }  //  end main


// /**
//  * @brief subrutina encargada de devolver el numero generar
//  * numero pseudoaleatorio y retornarlo como una falsa direccion
//  * de memoria
//  * @param data
// */
// void* number(void* data) {
//   unsigned int *seed = data; 
//   uint64_t  number = rand_r(seed) % 99;
//   uint64_t* pointer =  malloc(sizeof(uint64_t));
//   *pointer = number;
//   return (void*)pointer;
// }