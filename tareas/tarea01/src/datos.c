// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <datos.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <unistd.h>


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
    zip_stat_init(&datos->sb);
    datos->za = NULL;
    datos->zf = NULL;
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


void datos_generate_passw(datos_t* datos, char* password) {
    if (strlen(password) > 0 && strlen(password) <= (size_t)datos->limite) {
        printf("%s\n", password);
    }

    if (strlen(password) <= (size_t)datos->limite) {
        for (size_t i = 0; i < strlen(datos->alfabeto.array[0]); i++) {
            char new_password[strlen(password) + 1];
            memcpy(new_password, password, strlen(password) + 1);
            char new_char[2];
            new_char[0] =  datos->alfabeto.array[0][i];
            strncat(new_password, new_char, 1);
            datos_generate_passw(datos, new_password);
        }
    }
}


int datos_abrir_archivo(datos_t* datos, char* key) {
    int error = EXIT_SUCCESS;
    size_t i = 0;
    const char* archive = datos->zips.array[i];
    char* password = key;
    char buf[100];

    if ((datos->za = zip_open(archive, 0, &error)) != NULL) {
    } else {
        zip_error_to_str(buf, sizeof(buf), error, errno);
        fprintf(stderr, "Can't open zip archive `%s': %s\n",
        archive, buf);
        return error;
    }

    for (zip_int64_t  i = 0; i < zip_get_num_entries(datos->za, 0); i++) {
        if (zip_stat_index(datos->za, i, 0, &datos->sb) == 0) {
            datos->zf = zip_fopen_index_encrypted(datos->za, i, 0, password);
            if (datos->zf != NULL) {
                zip_fread(datos->zf, buf, 100);
                /// verificar que no sea un falso positivo
                /// al leer el contendio del archivo
                if (buf[0] == 'C') {
                    arreglo_agregar(&datos->contrasenas,password);
                    // arreglo_agregar(&datos->contrasenas,'\n');
                } else {
                }
                zip_fclose(datos->zf);
            } else {
            }
        }
    }
    if (zip_close(datos->za) == -1) {
        fprintf(stderr, "Can't close zip archive `%s'/n", archive);
        return 1;
    }
    return error;
}

void datos_impresion(datos_t* datos) {
    FILE* output = stdout;
    for (size_t i = 0; i < datos->zips.total; i++) {
        fprintf(output, "%s" " %s", datos->zips.array[i]
        , datos->contrasenas.array[i]);
    }
}

