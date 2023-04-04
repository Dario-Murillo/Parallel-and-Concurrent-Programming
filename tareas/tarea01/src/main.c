// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <datos.h>
#include <string.h>

int main() {
    FILE* input = stdin;
    datos_t *datos = datos_create();
    char value;
    int linea = 0;
    datos_innit(datos);

    while (fscanf(input, "%c", &value) != EOF) {
        if (linea == 0) {
            arreglo_agregar(&datos->alfabeto, value);
        } else if (linea == 1) {
            if (value != '\n') {
                datos->limite = value;
            }
        } else if (linea >= 3) {
            arreglo_agregar(&datos->zips, value);
        }
        if (value == '\n') {
            linea++;
        }
    }
    datos_destroy(datos);
    return 0;
}
