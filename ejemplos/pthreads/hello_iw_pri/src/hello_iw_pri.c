// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <assert.h>
#include <inttypes.h>
#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

// thread_private_data_t
// este struc funciona de manera similar a una clase y se encarga
// de guardar de manera privada los datos de los hilos
typedef struct private_data {
  uint64_t thread_number;  // rank
  uint64_t thread_count;
  struct private_data* next;
} private_data_t;

/**
 * @brief declaracion de metodos antes por la naturaleza del compilador 
 */
void* greet(void* data);
int create_threads(uint64_t thread_count);
// procedure main(argc, argv[])
// el main es el encargado de recibir el input del usuario y verificar que este
// sea valido, si es exitoso llama a la subrutina create_threads encargada de
// crear los hilos y pasa como argumento el tread_count
// es decir la cantidad de hilos a crear
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  // create thread_count as result of converting argv[1] to integer
  // thread_count := integer(argv[1])
  uint64_t thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64, &thread_count) == 1) {
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 11;  // usamos numeros de error como 11,
      // 1 = primer subrutina, el otro 1 = primero error
    }
  }
  error = create_threads(thread_count);
  return error;
}  // end procedure

/**
 * @brief subrutina encargada de crear los hilos
 * @param thread_count para saber cuantos hilos necesitamos hacer
 *
*/
int create_threads(uint64_t thread_count) {
  int error = EXIT_SUCCESS;
  // for thread_number := 0 to thread_count do
  pthread_t* threads = (pthread_t*) malloc(thread_count * sizeof(pthread_t));
  private_data_t* private_data = (private_data_t*)
    calloc(thread_count, sizeof(private_data_t));
  // usamos calloc para reservar memoria dinamica inicializada
  if (threads && private_data) {
    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      // como la memoria esta inicializada en 0 por el metodo calloc
      // hay que sobreescribirla
      private_data[thread_number].thread_number = thread_number;
      private_data[thread_number].thread_count = thread_count;
      // create_thread(greet, thread_number)
      // ahora accedemos a los hilos y a su numero por medio de un indice
      // es un arreglo de memoria
      error = pthread_create(&threads[thread_number], /*attr*/ NULL, greet
        , /*arg*/ &private_data[thread_number]);
      if (error == EXIT_SUCCESS) {
      } else {
        fprintf(stderr, "Error: could not create secondary thread\n");
        error = 21;  // mismo formato de error
        break;
      }
    }

    // print "Hello from main thread"
    printf("Hello from main thread\n");

    // por medio de un for liberamos la memoria ocupada por los hilos
    for (uint64_t thread_number = 0; thread_number < thread_count
        ; ++thread_number) {
      pthread_join(threads[thread_number], /*value_ptr*/ NULL);
    }
    // desocupamos la memoria dinamica ocupada
    free(private_data);
    free(threads);
  } else {
    fprintf(stderr, "Error: could not allocate %" PRIu64 " threads\n"
      , thread_count);
    error = 22;
  }
  return error;  // retorna la variable del error
}

/**
 * @brief subrutina que los hilos secundarios usan para imprimir su saludo
 * @param data
*/
void* greet(void* data) {
  // assert(data);
  private_data_t* private_data = (private_data_t*) data;
  // print "Hello from secondary thread"
  printf("Hello from secondary thread %" PRIu64 " of %" PRIu64 "\n"
    , (*private_data).thread_number, private_data->thread_count);
  // ahora imprimos tanto el numero del hilo, como el numero del total de hilos
  return NULL;
}  // end procedure
