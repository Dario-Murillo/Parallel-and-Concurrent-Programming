// Copyright 2023 <Dario Murillo Chaverri C15406>
#ifndef DATOS_H
#define DATOS_H

#include <arr_dinamico.h>
#include <inttypes.h>
#include <zip.h>

typedef struct datos {
    arr_dinamico_t alfabeto;
    arr_dinamico_t zips;
    uint64_t limite;
    struct zip* za;
    struct zip_file *zf;
    struct zip_stat sb;
} datos_t;

datos_t* datos_create(void);
void datos_innit(datos_t* datos);
void datos_destroy(datos_t* datos);
int datos_analisis(datos_t* datos, FILE* input);
int abrir_archivo(datos_t* datos);
void generate_passw(datos_t* datos);

#endif
