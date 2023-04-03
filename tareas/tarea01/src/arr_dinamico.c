#include <arr_dinamico.h>
#include <assert.h>

int aumentar_capacidad(arr_dinamico_t* arr);


int arreglo_innit(arr_dinamico_t* arr) {
    int error = EXIT_SUCCESS;
    assert(arr);
    arr->capacidad = 0;
    arr->total = 0;
    arr->elementos = NULL;
    return error;
}
void arreglo_destroy(arr_dinamico_t* arr){
    assert(arr);
    free(arr);
}

int arreglo_agregar(arr_dinamico_t* arr, char elemento){
    assert(arr);
    if(arr->total == arr->capacidad) {
        aumentar_capacidad(arr);
    }
    arr->elementos[arr->total++] = elemento;
    return EXIT_SUCCESS;
}

int aumentar_capacidad(arr_dinamico_t* arr) {
    int error = EXIT_SUCCESS;
    size_t nueva_capacidad = 10 * (arr->total ? arr->total : 1);
    char* nuevos_elementos = (char*)
    realloc(arr->elementos, nueva_capacidad * sizeof(char));
    if (nueva_capacidad) {
        arr->capacidad = nueva_capacidad;
        arr->elementos = nuevos_elementos;
    } else {
        error = EXIT_FAILURE;
    }
    return error;
}

void arreglo_imprimir(arr_dinamico_t* arr) {
    for (size_t i = 0; i < arr->total; i++)
    {
        printf("Valor:%c\n", arr->elementos[i]);
    }
}
