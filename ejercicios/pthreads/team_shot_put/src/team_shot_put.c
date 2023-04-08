// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <pthread.h>
#include <inttypes.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


typedef struct equipo {
  uint64_t thread_num;
  uint64_t team_num;
  unsigned int seed;
} equipo_t;

void* shoot(void* arg);
int create_threads(uint64_t num_atletas);

// main
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  uint64_t num_atletas;
  if (argc == 2) {
    if (sscanf(argv[1], "%" SCNu64 , &num_atletas) == 1) {
        if (num_atletas % 2 != 0) {
        } else {
            fprintf(stderr, "Error: athlete count must be odd\n");
            return 11;
        }
    } else {
      fprintf(stderr, "Error: invalid thread count\n");
      return 12;
    }
  } else {
    fprintf(stderr, "Error: must specify an odd number\n");
    return 13;
  }
  error = create_threads(num_atletas);
  return error;
}

/**
 * @brief funciona como el hilo principal es el encargado de multiples acciones
 * desde crear hilos, haciendo uso de memoria dinamica, como una impresion, 
 * y luego la liberacion de la memoria usada
 * @param num_atletas recibe del main cuantos hilos se van a crear segun
 * tiene que ser proveerse siempre un numero impar 
 * 
*/
int create_threads(uint64_t num_atletas) {
  int error = EXIT_SUCCESS;

  pthread_t* hilos_equipo1 =
  (pthread_t*) malloc(num_atletas * sizeof(pthread_t));
  pthread_t* hilos_equipo2 =
  (pthread_t*) malloc(num_atletas * sizeof(pthread_t));
  equipo_t* datos_equipo1 = (equipo_t*) calloc(num_atletas,
  sizeof(equipo_t));
  equipo_t* datos_equipo2 = (equipo_t*) calloc(num_atletas,
  sizeof(equipo_t));
  // creacion de dos equipos de hilos distintos con sus respectivos datos

  if (hilos_equipo1 && hilos_equipo2 && datos_equipo1 && datos_equipo2) {
    for (uint64_t thread_num = 0; thread_num < num_atletas; thread_num++) {
      datos_equipo1[thread_num].thread_num = thread_num;
      datos_equipo2[thread_num].thread_num = thread_num;
      datos_equipo1[thread_num].team_num = 1;
      datos_equipo2[thread_num].team_num = 2;
      datos_equipo1[thread_num].seed = time(NULL) ^ pthread_self();
      datos_equipo2[thread_num].seed = time(NULL) ^ pthread_self();
      error = pthread_create(&hilos_equipo1[thread_num], NULL, shoot,
      &datos_equipo1[thread_num]);
      error = pthread_create(&hilos_equipo2[thread_num], NULL, shoot,
      &datos_equipo2[thread_num]);
    }
  }
  float* numero1;  // variable para almacenar el dato devuelto por shoot
  float* numero2;
  int equipo1_pts = 0;
  int equipo2_pts = 0;
  for (uint64_t thread_number = 0; thread_number < num_atletas
    ; ++thread_number) {
    // recolecion de los numeros generados por medio de punteros
    pthread_join(hilos_equipo1[thread_number], (void**)&numero1);
    pthread_join(hilos_equipo2[thread_number], (void**)&numero2);
    // comparacion entre los hilos de cada equipo
    if (*numero1 > *numero2) {
      equipo1_pts++;
    } else {
      equipo2_pts++;
    }
    free(numero1);
    free(numero2);
  }

  if (equipo1_pts > equipo2_pts) {
    printf("equipo1 gana: %d : %d\n", equipo1_pts, equipo2_pts);
  } else {
    printf("equipo2 gana: %d : %d\n", equipo1_pts, equipo2_pts);
  }

  //  liberar memoria
  free(hilos_equipo1);
  free(hilos_equipo2);
  free(datos_equipo1);
  free(datos_equipo2);
  return error;
}

/**
 * @brief genera 3 numeros aleatorios entre 0.0 a 25.0 
 * retorna la direccion de memoria del mayor
 * @param arg
 * 
*/
void* shoot(void* arg) {
  equipo_t* datos_equipo = (equipo_t*) arg;
  unsigned int seed = time(NULL) ^ pthread_self();
  float best_shoot = 0;
  for (int i = 0; i < 3; i++) {
      float number = ((float)rand_r(&seed)/2147483648) * (25-0) + 0;
      if (number > best_shoot) {
        best_shoot = number;
      }
  }
  printf("%" PRIu64 "." "%" PRIu64 " Best shoot:%f\n",
  datos_equipo->team_num, datos_equipo->thread_num + 1, best_shoot);
  float* resultado = malloc(sizeof(float));
  *resultado = best_shoot;
  return (void*)resultado;
}

