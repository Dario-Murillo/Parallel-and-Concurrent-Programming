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
    char str[120];
    int linea = 0;
    while (fgets(str, 120, input)) {
        if (linea == 0) {
            str[strcspn(str, "\n")] = 0;
            arreglo_agregar(&datos->alfabeto, str);
        } else if (linea == 1 && linea < 2) {
            str[strcspn(str, "\n")] = 0;
            sscanf(str, "%"  SCNi64 , &datos->limite);
        } else if (linea >= 3) {
            str[strcspn(str, "\n")] = 0;
            arreglo_agregar(&datos->zips, str);
        }
        linea++;
    } 
    return error;
}


void datos_generate_passw(datos_t* datos) {
    uint64_t my_limite = datos->limite;
    
}


/**
 * 
 * 
 * https://gist.github.com/mobius/1759816
*/
int datos_abrir_archivo(datos_t* datos) {
    int error = EXIT_SUCCESS;
    size_t i = 0;
    while (i <  datos->zips.total) {
        char* archive = datos->zips.array[i];
        const char* password = "75";
        char buf[100];
        
        if ((datos->za = zip_open(archive, 0, &error)) != NULL) {
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
                            i++; // si encontro la contra pase al siguiente archivo
                            // agregar a un arreglo el archivo y la contra 
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
    }
    return error;
}



