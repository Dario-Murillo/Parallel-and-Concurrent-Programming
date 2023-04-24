// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <datos.h>
#include <string.h>

int main() {
    FILE* input = stdin;
    int error = EXIT_SUCCESS;
    datos_t *datos = datos_create();
    datos_innit(datos);
    error = datos_analisis(datos, input);
    if (error == EXIT_SUCCESS) {
        datos_generate_passw(datos);
        datos_impresion(datos);
        datos_destroy(datos);
    } else {
        printf("No se pudieron leer los datos");
        error = EXIT_FAILURE;
    }
    return error;
}
