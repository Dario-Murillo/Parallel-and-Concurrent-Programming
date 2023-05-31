// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>
#include <vector>
#include <numeric>
#include <algorithm>
#include <math.h>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  std::vector<double> values;

  double value = 0.0;
  while (std::cin >> value) {
    values.push_back(value);
  }

  double sum = 0.0;
  double maxValue = values[0];
  double minValue = values[0];
  double standardDeviation = 0.0;
  double mean = std::accumulate(values.begin(),
   values.end(), 0.0) / values.size();


  #pragma omp parallel for num_threads(thread_count) schedule(runtime) \
    default(none) shared(values, mean) \
    reduction(+:sum) reduction(max:maxValue) \
    reduction(min:minValue) reduction(+:standardDeviation)
  for (size_t index = 0; index < values.size(); ++index) {
    sum += values[index];
    maxValue = maxValue > values[index] ? maxValue : values[index];
    minValue = minValue < values[index] ? minValue : values[index];
    standardDeviation += pow(values[index] - mean, 2);
  }

  // ordena el arreglo de manera concurrente para luego sacar la mediana
  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(values)
  for (size_t i = 0; i < values.size(); i++) {
    #pragma omp for
    for (size_t j = i; j < values.size(); j++) {
      if (values[i] > values[j]) {
        #pragma omp critical(acceso)
        std::swap(values[i], values[j]);
      }
    }
  }
  
  const double average = values.size() ? sum / values.size() : 0.0;
  standardDeviation = sqrt(standardDeviation / values.size());
  const double middle = values.size() / 2;
  const double median = values.size() % 2 == 0 ?
    (values[middle - 1] + values[middle]) / 2 : values[middle];

  std::cout << "Mediana " << median <<  std::endl;
  std::cout << "Promedio " << average << std::endl;
  std::cout << "Valor maximo " << maxValue << std::endl;
  std::cout << "Valor minimo " << minValue << std::endl;
  std::cout << "Deviacion estandar " << standardDeviation << std::endl;

}
