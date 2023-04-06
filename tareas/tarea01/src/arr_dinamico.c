// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <arr_dinamico.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>

/**
 * @brief aumenta la capacidad de memoria actual
 * @details mueve los datos del arreglo actual a uno nuevo
 * con mas memoria con la funcion realloc 
 * @param arr puntero a la variable de tipo arr_dinamico_t
 * @return codigo de error
*/
int aumentar_capacidad(arr_dinamico_t* arr);

/**
 * @brief inicializa los valores del struct
 * @details reserva memoria inicial para el primer elemento
 * @param arr puntero a la variable de tipo arr_dinamico_t
 * @return retorna codigo de error 
*/
int arreglo_innit(arr_dinamico_t* arr) {
    int error = EXIT_SUCCESS;
    assert(arr);
    arr->array = malloc(1* sizeof(char*));
    arr->capacidad = 0;
    arr->total = 0;
    return error;
}

/**
 * @brief libera los alojamientos de memoria 
 * @param arr puntero a la variable de tipo arr_dinamico_t
 * @return void
*/
void arreglo_destroy(arr_dinamico_t* arr) {
    assert(arr);
    for (size_t i = 0; i < arr->total; i++) {
        free(arr->array[i]);
    }
    free(arr->array);
    arr->array = NULL;
    arr->capacidad = 0;
    arr->total = 0;
}

/**
 * @brief agrega un nuevo elemento de tipo char* al arreglo
 * @param arr  puntero a la variable de tipo arr_dinamico_t
 * @param elemento cadena de caracteres que se agregara al arreglo
 * @return codigo de error 
*/
int arreglo_agregar(arr_dinamico_t* arr, const char* elemento) {
    assert(arr);
    if (arr->total == arr->capacidad) {
        aumentar_capacidad(arr);
    }
    char* aux = malloc(strlen(elemento) + 1);
    strcpy(aux, elemento);
    arr->array[arr->total++] = aux;
    return EXIT_SUCCESS;
}

int aumentar_capacidad(arr_dinamico_t* arr) {
    int error = EXIT_SUCCESS;
    size_t nueva_capacidad = 2 * (arr->total ? arr->total : 1);
    char** nuevos_elementos =
    realloc(arr->array, nueva_capacidad * sizeof(char**));
    if (nueva_capacidad) {
        arr->capacidad = nueva_capacidad;
        arr->array = nuevos_elementos;
    } else {
        error = EXIT_FAILURE;
    }
    return error;
}

/**
 * @brief imprime el arreglo
 * @param arr  puntero a la variable de tipo arr_dinamico_t
 * @return void 
*/
void arreglo_imprimir(arr_dinamico_t* arr) {
    for (size_t i = 0; i < arr->total; i++) {
        printf("%s", arr->array[i]);
    }
}

