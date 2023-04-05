// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <datos.h>
#include <string.h>

int main() {
    FILE* input = stdin;
    datos_t *datos = datos_create();
    
    datos_innit(datos);
    datos_analisis(datos, input);
    datos_destroy(datos);
    return 0;
}
