// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4
// TODO(all): Implement a thread-safe queue

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>

typedef struct queue_node {
  size_t data;
  struct queue_node* next;
} queue_node_t;

typedef struct {
  pthread_mutex_t can_access_queue;  // mutex para crear subrutinas thread-safe
  queue_node_t* head;
  queue_node_t* tail;
} queue_t;

 /**
  * @brief inicializa los datos de la cola incluyendo el mutex
  * @param queue puntero a la cola
  * @return codigo de error de la inicializacion del mutex 0 en caso exitoso
 */
int queue_init(queue_t* queue);

/**
  * @brief destruye los datos de la cola y el mutex 
  * @param queue puntero a la cola
  * @return codigo de error de la destruccion del mutex 0 en caso exitoso
 */
int queue_destroy(queue_t* queue);

/**
 * @brief consulta si la lista esta vacia
 * @param queue puntero a la cola
 * @return true si la cola esta vacia, falso en caso alterno
*/
bool queue_is_empty(queue_t* queue);

/**
 * @brief agrega un elemento a la cola 
 * @param queue puntero a la cola
 * @param data dato a insertar en la cola 
 * @return codigo de error 0 en caso exitoso
*/
int queue_enqueue(queue_t* queue, const size_t data);

/**
 * @brief elimina un elemento de la cola 
 * @param queue puntero a la cola
 * @param data dato a eliminar de la cola 
 * @return codigo de error 0 en caso exitoso
 * 
*/
int queue_dequeue(queue_t* queue, size_t* data);

/**
 * @brief elimina todos los elementos de la cola
 * @param queue puntero a la cola
*/
void queue_clear(queue_t* queue);

/**
 * @brief calcula la cantidad de elementos en la cola
 * @param queue puntero a la cola
 * @return cantidad de elementos en la cola
*/
int queue_getcount(queue_t* queue);

#endif  // QUEUE_H
