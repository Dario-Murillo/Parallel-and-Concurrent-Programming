// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <string.h>
#include "simulacion.h"

int main(int argc, char* argv[]) {
  FILE* input = stdin;
  int error = EXIT_SUCCESS;
  datos_t *datos = datos_create();
  datos_innit(datos);
  error = datos_run(datos, input, argc, argv);
  if (error == EXIT_SUCCESS) {
    datos_destroy(datos);
  } else {
    printf("No se pudieron leer los datos");
    error = EXIT_FAILURE;
  }
  return error;
}
