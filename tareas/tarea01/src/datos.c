// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <datos.h>
#include <assert.h>
#include <stdio.h>
#include <string.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>
#include <zip.h>
#include <math.h>


datos_t* datos_create(void) {
    datos_t* datos = (datos_t*)
    malloc(1 * sizeof(datos_t));
    return datos;
}


void datos_innit(datos_t* datos) {
    assert(datos);
    arreglo_innit(&datos->alfabeto);
    arreglo_innit(&datos->zips);
    arreglo_innit(&datos->contrasenas);
    datos->limite = 0;
}


void datos_destroy(datos_t* datos) {
    assert(datos);
    arreglo_destroy(&datos->alfabeto);
    arreglo_destroy(&datos->zips);
    arreglo_destroy(&datos->contrasenas);
    free(datos);
}


int datos_analisis(datos_t* datos, FILE* input) {
    assert(datos);
    assert(input);
    int error = EXIT_SUCCESS;
    if (input != NULL) {
        char str[120];
        int linea = 0;
        while (fgets(str, 120, input)) {
            if (linea == 0) {
                /// strcspn evita agregar cambios de linea
                str[strcspn(str, "\n")] = 0;
                arreglo_agregar(&datos->alfabeto, str);
            } else if (linea == 1 && linea < 2) {
                str[strcspn(str, "\n")] = 0;
                sscanf(str, "%"  SCNi64 , &datos->limite);
            } else if (linea == 3) {
                str[strcspn(str, "\n")] = 0;
                arreglo_agregar(&datos->zips, str);
            }
            linea++;
        }
    } else {
        error = EXIT_FAILURE;
        return error;
    }
    return error;
}



int pass_base(size_t numero, size_t base) {
    if(numero == 0)
        return numero;
    return (numero % base) + 10 * pass_base(numero / base, base);
}

void datos_generate_passw(datos_t* datos) {
    // largo de la clave
    for (size_t i = 1; i <= (size_t)datos->limite; i++) {
        char* pass_temp = calloc(i + 1, sizeof(char*));
        // total de caracteres elevado a la i

        for (size_t j = 0; j < pow(strlen(datos->alfabeto.array[0]), i); j++) {
            // int base[n] == passBase(limite, strlen(datos->alfabeto.array[0]))
            unsigned int temp = pass_base(j, strlen(datos->alfabeto.array[0]));
            size_t cont = 0;
            while (temp > 0) {
                int digito = temp % 10;
                if (cont < i) {
                    pass_temp[cont] = datos->alfabeto.array[0][digito];
                }
                temp /= 10;
                cont++;
            }
            sleep(1);
            puts(pass_temp);
        }
        free(pass_temp);
    }
}


bool datos_abrir_archivo(datos_t* datos, char* key) {
    int error = EXIT_SUCCESS;
    size_t i = 0;
    const char* archive = datos->zips.array[i];
    zip_t* arch = NULL;


    if ((arch = zip_open(archive, 0, &error)) != NULL) {
    } else {
        error = EXIT_FAILURE;
    }

    struct zip_stat* finfo = NULL;


    finfo = calloc(256, sizeof(int));
    zip_stat_init(finfo);
    zip_file_t* fd = NULL;
    char* txt = NULL;
    int count = 0;

    while ((zip_stat_index(arch, count, 0, finfo)) == 0) {
        txt = calloc(finfo->size, sizeof(char));
        fd = zip_fopen_index_encrypted(arch, count, 0, key);
        zip_fread(fd, txt, finfo->size);
        //printf("%s", txt);
        if (strcmp(txt, "CI0117-23a")) {
            return true;
        }
        free(txt);
        // printf("%d\n",count);
        count++;
    }

    free(finfo);
    if (zip_close(arch) == -1) {
        fprintf(stderr, "Can't close zip archive `%s'/n", archive);
        return 1;
    }
    return false;
}

void datos_impresion(datos_t* datos) {
    FILE* output = stdout;
    for (size_t i = 0; i < datos->zips.total; i++) {
        fprintf(output, "%s" " %s", datos->zips.array[i]
        , datos->contrasenas.array[i]);
    }
}

