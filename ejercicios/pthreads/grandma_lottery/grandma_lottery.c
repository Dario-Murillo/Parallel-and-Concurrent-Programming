#include <pthread.h>
#include <stdlib.h>
#include <sys/random.h>
#include <stdio.h>

<<<<<<< HEAD
// declaracion de la subrutina number 
void* number(void* data);

/**
 * @brief subrutina principal encargada de crear y destruir ambos hilos
*/
int main(void) {
=======
void* number(void* data);


int main(void) {


    
>>>>>>> d68af931a62220a79c13a14c4c3defab2c75e607
    size_t numero_ran1 = 0;
    size_t numero_ran2 = 0;

    // void* temp;
<<<<<<< HEAD
    pthread_t thread_ran1, thread_ran2;

    unsigned int seed1 =  time(NULL); // generamos una 'seed' distinta para cada hilo con el metodo time
    unsigned int seed2 =  time(NULL) * 10;

    pthread_create(&thread_ran1, NULL, number, (void*) &seed1); // creamos ambos hilos llamando a la subrutina greed y pasando como parametro 
    pthread_create(&thread_ran2, NULL, number, (void*) &seed2); // su respectivo seed

    // liberacion de los hilos y retorno del numero random que consiguieron
    pthread_join(thread_ran1, (void*)&numero_ran1);
    pthread_join(thread_ran2, (void*)&numero_ran2);

    // numero_ran1 = (size_t) temp;
    printf("Primer numero:%zu\n", numero_ran1);
    printf("Segundo numero:%zu\n", numero_ran2);
} // end main

/**
 * @brief subrutina number 
 * @param data recibe un seed unico que es usado para el metodo rand_r
*/
void* number(void* data) {
  unsigned int *seed = data; 
  int number = rand_r(seed);
  size_t cast = (size_t) number;
  void* retorno = (void*) cast;
  return retorno;
} // end number
=======
    pthread_t thread_ran1;
    pthread_create(&thread_ran1, NULL, number, NULL);

    pthread_t thread_ran2;
    pthread_create(&thread_ran2, NULL, number, NULL);


    pthread_join(thread_ran1, (void*)&numero_ran1);
    pthread_join(thread_ran2, (void*)numero_ran2);

    //numero_ran1 = (size_t) temp;
    printf("Numero:%zu\n", numero_ran1);
    

}


void* number(void* data) {
  
  (void) data;
  size_t numeroran =  99; 
  void* ptr = (void*)numeroran;
  return ptr;
}
>>>>>>> d68af931a62220a79c13a14c4c3defab2c75e607
