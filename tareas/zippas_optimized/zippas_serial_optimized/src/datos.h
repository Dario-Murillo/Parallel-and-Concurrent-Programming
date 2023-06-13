// Copyright 2023 <Dario Murillo Chaverri C15406>
#ifndef DATOS_H
#define DATOS_H

#include <arr_dinamico.h>
#include <inttypes.h>
#include <stdbool.h>


/**
 * @struct datos
 * @brief estructura encargada de recibir y manipular los datos del imput
 * 
 * @var datos::alfabeto
 * alfabeto guarda la primer linea de los lotes de prueba 
 * los caracteres con los que se va a trabajar
 * 
 * @var datos::limite
 * limite guarda la segunda linea de los lotes de prueba
 * el limite de largo de la contrasena
 * 
 * @var datos::zips
 * zips guarda la tercer lines de los lotes de prueba
 * las dirreciones de los archivos zips
 * 
 * @var datos::contrasenas  
 * contrasenas guarda las contrasenas que 
 * desencriptan los zips
 * 
*/
typedef struct datos {
  arr_dinamico_t alfabeto;
  uint64_t limite;
  arr_dinamico_t zips;
  arr_dinamico_t contrasenas;
} datos_t;

/**
 * @brief reserva memoria para crear una variable de tipo datos_t
 * @return variable de tipo datos_t
*/
datos_t* datos_create();

/**
 * @brief inicia los miembros del struct datos
 * @param datos puntero de una variable tipo datos_t
 * @return void
*/
void datos_innit(datos_t* datos);

/**
 * @brief lee el archivo introducido por la entrada estandar
 * y carga los valores en las variables respectivas
 * @param datos puntero de una variable tipo datos_t
 * @param input archivo introducido de la entrada estandar
 * @return codigo de error 
*/
int datos_analisis(datos_t* datos, FILE* input);

/**
 * @brief genera todas las posibles claves dado 
 * un alfabeto y un maximo de la clave e abre los archivos con
 * dicha clave
 * @param datos puntero de una variable tipo datos_t
 * @details: 
 *  Adaptado de 
 * https://www.geeksforgeeks.org/
 * c-program-to-read-and-print-all-files-from-a-zip-file/
 * @return void
*/
void datos_generate_passw(datos_t* datos);

/**
 * @brief libera la memoria usada
 * @see datos_innit
*/
void datos_destroy(datos_t* datos);

/**
 * @brief imprime las dirreciones de los zips y sus contrasenas
 * @param datos puntero de una variable tipo datos_t
 * @return void
 * 
*/
void datos_impresion(datos_t* datos);

#endif
