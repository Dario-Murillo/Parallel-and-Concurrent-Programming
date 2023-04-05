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
    zip_stat_init(&datos->sb);
    datos->limite = 0;
}

void datos_destroy(datos_t* datos) {
    assert(datos);
    arreglo_destroy(&datos->alfabeto);
    arreglo_destroy(&datos->zips);
    free(datos);
}

int datos_analisis(datos_t* datos, FILE* input) {
    assert(datos);
    int error = EXIT_SUCCESS;
    char value;
    char str[120];
    int linea = 0;
    while (fgets(str, 120, input) != NULL) {
         if (linea == 0) {
            arreglo_agregar(&datos->alfabeto, str);
        } else if (linea == 1) {
            fscanf(input, "%c", &value);
            datos->limite = value;
        } else if (linea >= 3) {
            arreglo_agregar(&datos->zips, str);
        }
        linea++;
    }    
    puts(datos->alfabeto.array[0]);
    return error;
}


void generate_passw(datos_t* datos) {
    uint64_t my_limite = datos->limite;
    char pass[my_limite+1];
    for (size_t size = 1; size < my_limite; size++) {
        memset(pass, 0, sizeof(pass));
        for (size_t i = 0; i < size; i++) {

        }
        while(1) {
            printf("%s\n", pass);
        }
    }
}

/**
 * 
 * 
 * https://gist.github.com/mobius/1759816
*/
int abrir_archivo(datos_t* datos) {
    int error = EXIT_SUCCESS;
    char* archive = "tests/zip_05/f02.zip";
    const char* password = "75";
    char buf[100];
    
    if ((datos->za = zip_open(datos->alfabeto.array[0], 0, &error)) != NULL) {
        printf("Hola");
    } else {
        zip_error_to_str(buf, sizeof(buf), error, errno);
        fprintf(stderr, "Can't open zip archive `%s': %s\n", 
            archive, buf);
        return error;
    }
 
    for (zip_int64_t  i = 0; i < zip_get_num_entries(datos->za, 0); i++) {
        if (zip_stat_index(datos->za, i, 0, &datos->sb) == 0) {
            datos->zf = 
            zip_fopen_index_encrypted(datos->za, i, 0, password);
                if (datos->zf) {
                    zip_fread(datos->zf, buf, 100);
                    if (buf[0] == 'C') {
                        printf("Correct");
                    } else {
                        //falso positivo
                    }
                }
            }
            zip_fclose(datos->zf);
        }

    if (zip_close(datos->za) == -1) {
        fprintf(stderr, "Can't close zip archive `%s'/n",archive);
        return 1;
    }
    return error;
}



