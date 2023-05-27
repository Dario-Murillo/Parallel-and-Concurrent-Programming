// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  // cantiad de hilos
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  // vector de valores
  std::vector<double> values;

  // obtener una cantidad indefinida de valores por la entrada estandar
  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  double sum = 0.0;

  // se calcula la suma usando reducciones de openmp
  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(values) reduction(+:sum)
  for (size_t index = 0; index < values.size(); ++index) {
    sum += values[index];
  }

  // se calcula e imprime el promedio
  const double average = values.size() ? sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
