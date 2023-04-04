// Copyright 2023 <Dario Murillo Chaverri C15406>
#ifndef DATOS_H
#define DATOS_H

#include <arr_dinamico.h>
#include <inttypes.h>
#include <zip.h>

typedef struct datos {
    arr_dinamico_t alfabeto;
    arr_dinamico_t zips;
    char limite;
    struct zip* pointer;
    struct zip_stat sb;
    struct zip_error_t* ze;
} datos_t;

datos_t* datos_create(void);
void datos_innit(datos_t* datos);
void datos_destroy(datos_t* datos);
int contra(datos_t* datos);


#endif
