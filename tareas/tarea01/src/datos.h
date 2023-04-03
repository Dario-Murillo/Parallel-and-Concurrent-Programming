#ifndef DATOS_H
#define DATOS_H

#include <arr_dinamico.h>

typedef struct datos {
    arr_dinamico_t alfabeto;
    char limite;
    char archivo1[21];
    char archivo2[21];
    char archivo3[21];
}datos_t;

void datos_innit(datos_t* datos);
void datos_destroy(datos_t* datos);



#endif 