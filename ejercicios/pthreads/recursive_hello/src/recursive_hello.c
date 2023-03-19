#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
// #include <unistd.h>

/**
 * @brief Hay que declarar la funcion primero, si no el compilador no lo reconocera, esta es
 * la funcion que se encarga de saludar desde el segundo hilo
 * @param data
 */
void* greet(void* data);

/**
 * @brief Este es el metodo prinicipal el cual se encarga de crear el segundo hilo, e imprimir el primer saludo
 * 
*/
int main(void) {
  // create_thread(greet)
  pthread_t thread; //Declaracion del hilo
  size_t number = 2; //Variable que se usa para pasar como parametro a la subrutina greet
  int error = pthread_create(&thread, /*attr*/ NULL, greet, /*arg*/ (void*)number); //Llamado de funcion
  if (error == EXIT_SUCCESS) {
    // print "Hello from main thread"
    // usleep(1);  // indeterminism
    printf("Hello from main thread\n");
    pthread_join(thread, /*value_ptr*/ NULL); //Liberacion de memoria en caso de que se haya creado con exito
  } else {
    fprintf(stderr, "Error: could not create secondary thread\n"); //Mensaje de error
  }
  return error;
}  // end procedure

/**
 * @brief Segunda subrutina que es llamada a la hora de crear el segundo hilo, se llamara recursivamente 
 * @param data
*/
void* greet(void* data) {

  size_t numero = (size_t)data; //casteo para el numero que pasamos a la subrutina
  pthread_t recursive;
  
  if(numero == 0) {
    printf("\nBye from secondary thread, number received:%d", (int)numero);
    
  } else if (numero > 0){
    printf("\nHello from secondary thread, number received:%d", (int)numero);
    --numero; //Restar 1 al numero
    pthread_create(&recursive, NULL, greet, (void*)numero); //Creacion de hilo que llama a la subrutina greet recursivamente 
    pthread_join(recursive, NULL); //Liberacion de la memoria solicitada por los hilos
  }
  return NULL; 
}  // end procedure