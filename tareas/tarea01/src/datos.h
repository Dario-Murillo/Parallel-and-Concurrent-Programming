// Copyright 2023 <Dario Murillo Chaverri C15406>
#ifndef DATOS_H
#define DATOS_H

#include <arr_dinamico.h>
#include <inttypes.h>
#include <zip.h>

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
 * @var datos:za,zf,sb
 * variables encargadas de la apertura y lectura
 * de archivos zip
*/
typedef struct datos {
    arr_dinamico_t alfabeto;
    int64_t limite;
    arr_dinamico_t zips;
    struct zip* za;
    struct zip_file *zf;
    struct zip_stat sb;
} datos_t;

datos_t* datos_create(void);
void datos_innit(datos_t* datos);
int datos_analisis(datos_t* datos, FILE* input);
int datos_abrir_archivo(datos_t* datos);
// void datos_generate_passw(datos_t* datos, char prefix);
void datos_destroy(datos_t* datos);

#endif
