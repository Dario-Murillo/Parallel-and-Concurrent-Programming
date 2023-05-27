// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  // obtiene una cantidad de hilos por default
  int thread_count = omp_get_max_threads();
  if (argc == 2) {
    thread_count = atoi(argv[1]);  // si se proveee por linea de comandos
  }

  // clausula num_threads nos permite indicar la cantida de hilos
  #pragma omp parallel num_threads(thread_count)
  {
    #pragma omp critical(stdout)
    std::cout << "Hello from secondary thread " << omp_get_thread_num()
      << " of " << omp_get_num_threads() << std::endl;
  }
}
