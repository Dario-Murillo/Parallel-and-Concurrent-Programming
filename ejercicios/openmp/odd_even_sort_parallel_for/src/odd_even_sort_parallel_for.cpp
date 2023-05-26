// Copyright Dario Murillo C15406
#include <omp.h>
#include <iostream>
#include <random>

void parallel_odd_even_sort(double* array, int n, int thread_count);
void swap(double* a, double* b);

int main(int argc, char *argv[]) {
  int size = 5;
  if (argc >= 2) {
    size = atoi(argv[1]);
  }

  int thread_count = size;
  if (argc >= 3) {
    thread_count = atoi(argv[2]);
  }


  double* array = new double[size];

  for (int i = 0; i < size; i++) {
    std::random_device rseed;
    std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> dist(0, 100);
    array[i] = dist(rng);
  }

  parallel_odd_even_sort(array, size, thread_count);
  std::cout << "After sorting" << std::endl;
  for (int i = 0; i < size; i++) {
    std::cout << array[i] << std::endl;
  }

  delete [] array;
  return 0;
}

void swap(double* a, double* b) {
  double temp = *a;
  *a = *b;
  *b = temp;
}

void parallel_odd_even_sort(double* array, int n, int thread_count) {
  for (int phase = 0; phase < n; ++phase) {
    if (phase % 2 == 0) {
      #pragma omp parallel for num_threads(thread_count) \
        default(none) shared(array, n)
      for (int i = 1; i < n; i += 2) {
        #pragma omp critical
        if (array[i - 1] > array[i]) {
          swap(&array[i-1], &array[i]);
        }
      }
    } else {
      #pragma omp parallel for num_threads(thread_count) \
        default(none) shared(array, n)
      for (int i = 1; i < n - 1; i += 2) {
        #pragma omp critical
        if (array[i] > array[i + 1]) {
          swap(&array[i], &array[i+1]);
        }
      }
    }
  }
}
