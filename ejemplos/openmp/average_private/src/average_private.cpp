// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  // numero de hilos
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  // vector de valores
  std::vector<double> values;

  // obtiene valores de la entrada estandar
  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  double total_sum = 0.0;

  // declara zona paralelizada
  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(values, total_sum)
  {
    double my_partial_sum = 0.0;

    // reutilizada los hilos con el for y los reparte con un mapeo
    // elegido por el programa
    #pragma omp for schedule(runtime)
    for (size_t index = 0; index < values.size(); ++index) {
      my_partial_sum += values[index];
    }

    // agrega todas las sumas parciales individuales de cada hilo a una sola
    #pragma omp critical(can_add)
    total_sum += my_partial_sum;
  }

  // imprime el promedio
  const double average = values.size() ? total_sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
