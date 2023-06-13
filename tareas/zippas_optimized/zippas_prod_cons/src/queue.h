// Copyright 2021 Jeisson Hidalgo-Cespedes <jeisson.hidalgo@ucr.ac.cr> CC-BY-4

#ifndef QUEUE_H
#define QUEUE_H

#include <pthread.h>
#include <stdbool.h>
#include <inttypes.h>

/**
 * @struct queue_node
 * @brief nodos de la cola
 * 
 * @var queue_node::data
 * datos que se almacenan en los
 * nodos de la cola, en este caso
 * son arreglos de caracteres
 * 
 * @var queue_node::next
 * puntero que apunta al siguiente nodo
 * 
*/
typedef struct queue_node {
  char data[100];
  struct queue_node* next;
} queue_node_t;

/**
 * @struct queue
 * @brief cola-thread_safe
 * 
 * @var queue::can_access_queue
 * mutex para crear subrutinas thread-safe
 * 
 * @var queue::head
 * cabeza de la cola
 * 
 * @var queue::tail
 * cola de la cola
*/
typedef struct {
  pthread_mutex_t can_access_queue;
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
int queue_enqueue(queue_t* queue, char* data);

/**
 * @brief elimina un elemento de la cola 
 * @param queue puntero a la cola
 * @param data dato a eliminar de la cola 
 * @return codigo de error 0 en caso exitoso
 * 
*/
int queue_dequeue(queue_t* queue, char* data);

/**
 * @brief elimina todos los elementos de la cola
 * @param queue puntero a la cola
*/
void queue_clear(queue_t* queue);

#endif  // QUEUE_H
