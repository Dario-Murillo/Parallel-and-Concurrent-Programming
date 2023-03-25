# Enunciado del ejercicio

Una adorable abuelita juega todos los domingos lotería. Como es peligroso que ella salga de casa, envía a sus dos nietos a comprar un par de pedacitos a vendedores diferentes para incrementar la suerte. Ella siempre juega a "gallo tapado", es decir, sin saber el número que le venden. Sin embargo, cuando sus nietos llegan a la casa, por su cansado estado de la vista ella les tiene que preguntar el número que los vendedores le dieron a cada uno.

Modele la situación anterior, con la abuela como el hilo principal, y los dos nietos como dos hilos secundarios. Los nietos generan un número pseudoaleatorio para representar la compra de la fracción de lotería. La espera de la abuela del regreso a casa de los dos nietos es representado por su join. Los nietos comunican a la abuela los números obtenidos a través del valor de retorno de la rutina secundaria. Indague en la documentación de Pthreads cómo este valor es recuperado por pthread_join(). Implemente dos variantes en que los nietos comunican este valor a la abuela:

    1.Retornan la dirección de memoria de la variable que tiene el número comprado.

    2.Retornan el número comprado como una falsa dirección de memoria.

# Version alterna y fallida del ejercicio devolviendo la dirrecion de memoria de los numeros random

Por medio de herramientas de optimizacion como asan nos damos cuenta de que esta implementacion del codigo en la cual devolvemos la dirrecion de memoria de la variable no produce el resultado correcto


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
