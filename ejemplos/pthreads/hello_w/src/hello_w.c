// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>   // libreria para los macros
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/**
 * @brief Declaracion del metodo greet para que el compilador
 *  lo reconozca antes de llamarlo
 * 
 */
void* greet(void* data);

/**
 * @brief Declaracion del metodo encargado de crear el numero de hilos 
 * minimos(numero de nucleos)
 * o en otro caso la cantidad de hilos solicitada por el usuario
*/
int create_threads(uint64_t thread_count);

// metodo main(argc, argv[]), recibe input del usuario
int main(int argc, char* argv[]) {
#if 0
  for (int index = 0; index < argc; ++index) {
    printf("argv[%d] = '%s'\n", index, argv[index]);
  }
  return 0;
#endif
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  // el numero introducido por el usuario
  // se guardara en la primer posicion del arreglo argv[]
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  // caso default  se usa el numero de nucleos como el numero de hilos
  // esto es en caso de que el usuario no haya introducido nada
  if (argc == 2) {   // se verifica que el numero de argumentos del usuario
  // sea 2, y se asigna al thread_count el dato en la pos 1
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
      // se usa el macro SCNu64 para reconocer el tipo de dato
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;
    }
  }
  error = create_threads(thread_count);
  return error;
}  // end procedure

/**
 * @brief Es la subrutina encargada de crear los hilos por 
 * defecto o los solicitados por el usuario
 * @param thread_count
 * 
*/
int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // usamos malloc para hacer uso de memoria dinamica
  // thread_count * por la cantidad de bytes necesarios
  // guarda esto en un puntero de tipo pthread_t
  // crea un puntero donde guarda memoria para crear los hilos
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
  if (threads) {  // Se debe verificar que sea valido
    for (uint64_t thread_number = 0; thread_number < thread_count;
    ++thread_number) {
      // create_thread(greet, thread_number)
      // por medio de un for creamos la cantidad de hilos necesarios
      error = pthread_create(/*indice*/&threads[thread_number],
      /*attr*/ NULL, greet, /*arg*/ (void*) thread_number);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;
        break;
      }
    }
    printf("Hello from main thread\n");
    // liberamos la memoria ocupada por los hilos
    // en caso de estar en el mismo ciclo for estariamos serializando el codigo
    for (uint64_t thread_number = 0; thread_number < thread_count;
        ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }
    // tambien se libera el puntero inicial de memoria dinamica
    free(threads);
  } else {
    fprintf(stderr,
    "Error: could not allocate %" PRIu64 " threads\n", thread_count);
    error = 22;
  }
  return error;
}

// procedure greet:
void* greet(void* data) {
  // assert(data);
  const uint64_t rank = (uint64_t) data;
  // casteo para obtener el dato obtenido y no un puntero nulo
  printf("Hello from secondary thread %" PRIu64 "\n", rank);
  return NULL;
}  // end procedure

