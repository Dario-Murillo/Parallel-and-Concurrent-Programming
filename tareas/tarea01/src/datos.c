#include <datos.h>

void datos_innit(datos_t* datos) {
    arreglo_innit(&datos->alfabeto);
    datos->limite = 0;

}

void datos_destroy(datos_t* datos) {
    free(datos);
}