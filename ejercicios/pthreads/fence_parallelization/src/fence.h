// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <stdlib.h>
#include <pthread.h>
#include <stdbool.h>

typedef struct {
  size_t row;
  size_t column;
} coordinate_t;

// informacion necesaria para todos los hilos
typedef struct {
  size_t thread_count;
  size_t rows;
  size_t columns;
  char** terrain;
  size_t maximum_perimeter;
  coordinate_t maximum_top_left;
  coordinate_t maximum_bottom_right;
  pthread_mutex_t mutex;
} shared_data_t;

typedef struct {
  size_t thread_num; // numero privado de cada hilo
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief metodo que crea hilos y los dirige al metodo run
 * @param shared_data puntero de memoria compartida
 * @return codigo de error
*/
int create_threads(shared_data_t* shared_data);

/**
 * @brief metodo controlador del programa
*/
void* run(void* data);

/**
 * @brief lee del archivo de la entrada estandar
 * y asigna en variables los datos necesarios
 * @param shared_data puntero de memoria compartida
 * @return codigo de error
*/
int read_terrain(shared_data_t* shared_data);

/**
 * @brief crea en memoria el terreno recibido
 * @param rows numero de filas
 * @param colums numero de columnas
 * @return retorna el "terreno"
 * 
*/
char** create_terrain(const size_t rows, const size_t columns);

/**
 * @brief destruye el terreno
 * @param rows numero de filas
 * 
*/
void destroy_terrain(char** terrain, const size_t rows);

/**
 * @brief encuentra el maximo perimetro
 * @param thread_num numero de hilos
 * @param shared_data puntero de memoria compartida
 * @param shared_data puntero de memoria compartida
*/
void find_maximum_perimeter(const size_t thread_num,
  shared_data_t* shared_data);

/**
 * @brief encuentra el maximo perimetro local
 * @param top_left punto arriba en la izquierda
 * @param bottom_right punto abajo en la derecha
*/
coordinate_t find_maximum_local_perimeter(const size_t top_left_row,
  const size_t top_left_column, const shared_data_t* shared_data);

/**
 * @brief trata de formar un rectangulo dado las 4 lineas de parametro
 * @param top_left_row fila izquierda de arriba
 * @param top_left_column columna de izquierda de arriba
 * @param bottom_right_row fila derecha de abajo
 * @param bottom_right_column columna derecha de abajo
 * @param shared_data puntero de memoria compartida
 * @return verdadero o falso en caso de exito o fallo al crear el rectangulo
*/
bool can_form_rectangle(const size_t top_left_row, const size_t top_left_column,
  const size_t bottom_right_row, const size_t bottom_right_column,
  const shared_data_t* shared_data);

/**
 * @brief calcula el perimeto
 * @param top_left punto arriba en la izquierda
 * @param bottom_right punto abajo en la derecha
 * @return retorna el perimetro
 * 
*/
size_t calculate_perimeter(const coordinate_t top_left,
    const coordinate_t bottom_right);

/**
 * @brief imprime el perimetro una vez terminada la ejecuccion
 * @param shared_data puntero de memoria compartida
*/
void print_maximum_perimeter(const shared_data_t* shared_data);
