#include <pthread.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct private_data {
  uint64_t thread_number;  
  uint64_t thread_count;
  struct private_data* next;
} private_data_t;


void* shoot(void*);
int create_threads(int num_atletas);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  int atletas;
  if (argc == 2) {  
    if (sscanf(argv[1], "%d" , &atletas) == 1) {   
        if(atletas % 2 != 0) {

        } else {
            fprintf(stderr, "Error: athlete count must be odd\n");
            return 11;
        }
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 12;
    }
  }
  error = create_threads(atletas);
  return error;
}

int create_threads(int num_atletas) {
    int error = EXIT_SUCCESS;
    double numero = 0;
    pthread_t* hilos = (pthread_t*) malloc(num_atletas * sizeof(pthread_t));
    int seed = time(NULL) * pthread_self();
    if(hilos) {
      for(int index = 0; index < atletas; index) {
        pthread_join(&hilos[index], NULL, shoot, (void*)seed);
      }
    }
  
    for (int thread_number = 0; thread_number < num_atletas
            ; ++thread_number) {
              pthread_join(hilos[thread_number], (void*) &numero);
    }
   free(hilos);
   return error;

}

void* shoot(void* arg){
  unsigned int *seed = arg;  // recibir la seed unica
  double best_shoot = 0;
  for(int i = 0; i < 3; i++) {
      int number = rand_r(seed) % 25;  // evitar que el numero sea mayor a 99
      if(best_shoot > number) {
        best_shoot = number;
      }
  }
  size_t cast = (size_t) best_shoot;
  void* retorno = (void*) cast;
  return retorno;
}