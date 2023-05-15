// Copyright Dario Murillo C15406
#include <iostream>
#include <random>

void serial_odd_even_sort(double* array, int n);
void swap(double* a, double* b);

int main(int argc, char *argv[]) {
  int size = 5;
  if (argc >= 2) {
    size = atoi(argv[1]);
  }
  double* array = new double[size];

  for (int i = 0; i < size; i++) {
    std::random_device rseed;
    std::mt19937 rng(rseed());
    std::uniform_int_distribution<int> dist(0, 20);
    array[i] = dist(rng);
  }

  serial_odd_even_sort(array, size);
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

void serial_odd_even_sort(double* array, int n) {
  for (int phase = 0; phase < n; ++phase) {
    if (phase % 2 == 0) {
      for (int i = 1; i < n; i += 2) {
        if (array[i - 1] > array[i]) {
          swap(&array[i-1], &array[i]);
        }
      }
    } else {
      for (int i = 1; i < n - 1; i += 2) {
        if (array[i] > array[i + 1]) {
            swap(&array[i], &array[i+1]);
          }
        }
      }
    }
}
