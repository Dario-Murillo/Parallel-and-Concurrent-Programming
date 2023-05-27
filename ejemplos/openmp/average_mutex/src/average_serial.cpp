// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>

int main(int argc, char* argv[]) {
  // cantidad de hilos
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  // vector de valores
  std::vector<double> values;

  // obtiene indefinidos numeros y los agrega al arreglo
  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  // declaracion de la suma
  double sum = 0.0;

  // calcula el promedio
  for (size_t index = 0; index < values.size(); ++index) {
    sum += values[index];
  }

  // saca el promedio y lo imprimi
  const double average = values.size() ? sum / values.size() : 0.0;
  std::cout << average << std::endl;
}
