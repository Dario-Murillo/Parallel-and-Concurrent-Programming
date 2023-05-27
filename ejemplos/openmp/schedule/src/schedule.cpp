// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

/**
 * @brief imprime el tipo de mapeo y sus digitos
 * @param type string para identificar el tipo de mapeo
 * @param mapping vector con las unidades de trabajo
 * 
*/
void print_mapping(const char* type, const std::vector<int>& mapping);

int main(int argc, char* argv[]) {
  // cantidad de hilos
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  // cantidad de iteraciones
  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  // tamano del bloque
  int block_size = 0;
  if (argc >= 4) {
    block_size = atoi(argv[3]);
  }

  std::vector<int> mapping(iteration_count);

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(iteration_count, mapping, block_size)
  {
    if (block_size == 0) {
      // schedule permite elegir el tipo de mapeom en este caso estatico
      #pragma omp for schedule(static)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        // guarda en el arreglo el numero del hilo efectuando la iteracion
        mapping[iteration] = omp_get_thread_num();
      }
      // single apra que solo sea ejecutado por un hilo
      #pragma omp single
      print_mapping("static    ", mapping);

      // mapeo dinamico
      #pragma omp for schedule(dynamic)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("dynamic   ", mapping);

      // mapeo guiado
      #pragma omp for schedule(guided)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("guided    ", mapping);
    } else {
      // igual que el mapeo estatico solo que se puede especificar el tamano
      // del bloque de las unidades
      #pragma omp for schedule(static, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("static,N  ", mapping);

      #pragma omp for schedule(dynamic, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("dynamic,N ", mapping);

      #pragma omp for schedule(guided, block_size)
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        mapping[iteration] = omp_get_thread_num();
      }

      #pragma omp single
      print_mapping("guided,N  ", mapping);
    }
  }
}

void print_mapping(const char* type, const std::vector<int>& mapping) {
  std::cout << type;
  for (size_t index = 0; index < mapping.size(); ++index) {
    std::cout << mapping[index] << (index == mapping.size() - 1 ? '\n' : ' ');
  }
}
