#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

typedef struct {
  unsigned int counter;
  unsigned int max;
  pthread_mutex_t mutex;
  /// Variable de condicion
  pthread_cond_t cond_var;
} mist_t;

/// @brief Inicializa los datos
/// @param mist registro mist
/// @param max cantidad de hilos
void mist_init(mist_t* mist, unsigned max) {
  mist->counter = 0;
  mist->max = max;
  pthread_mutex_init(&mist->mutex, NULL);
  // variable de condicion
  pthread_cond_init(&mist->cond_var, NULL);
}

/// @brief Destructor
/// @param mist registro a liberar
void mist_destroy(mist_t* mist) {
  mist->counter = 0;
  pthread_mutex_destroy(&mist->mutex);
  pthread_cond_destroy(&mist->cond_var);
}

/// @brief Subrutina donde se aplica la variable de condicion
/// @param mist registro tipo mist
void mistery(mist_t* mist) {
  pthread_mutex_lock(&mist->mutex);
  ++mist->counter;
  // Revisa si ya llegaron todos los hilos
  if (mist->counter < mist->max) {
    // Preferred: while ( pthread_cond_wait(...) != 0 ) /* empty */;
    // Las variables de condicion requieren de un mutex
    pthread_cond_wait(&mist->cond_var, &mist->mutex);
  } else {
    // Si es el ultimo hilo pone el contador en 0
    mist->counter = 0;
    // Y hace broadcast  liberando al resto de hilos que estaban
    // esperando en la variable de condicion, que actua como barrera
    pthread_cond_broadcast(&mist->cond_var);
  }
  pthread_mutex_unlock(&mist->mutex);
}

// Registro opaco
static mist_t mist;

/// @brief Contiene las impresiones y los llamados a mist
/// @param data numero de index, indica cuanto tiempo duerme el hilo
void* run(void* data) {
  fprintf(stderr, "%zu: before mist()\n", (size_t)data);
  sleep((unsigned)(size_t)data);
  mistery(&mist);
  fprintf(stderr, "%zu: after mist()\n", (size_t)data);
  return NULL;
}

int main() {
  // Inicializa el registro con max := 3
  mist_init(&mist, 3);
  // Crea 3 hilos
  pthread_t* workers = malloc(3 * sizeof(pthread_t));
  for (size_t index = 0; index < 3; ++index) {
    pthread_create(&workers[index], NULL, run, (void*)index);
  }
  // Espera por los 3 hilos
  for (size_t index = 0; index < 3; ++index) {
    pthread_join(workers[index], NULL);
  }
  // Llama al destructor
  mist_destroy(&mist);
  free(workers);
  return 0;
}
