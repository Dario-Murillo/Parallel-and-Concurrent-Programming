// Copyright Dario Murillo C15406
#include <iostream>

void serial_odd_even_sort(int n, double* array);

int main(int argc, char *argv[]) {

  int size = 0;
  if (argc >= 2) {
    size = atoi(argv[1]);
  }
  double* array = new double[size]; 

  for (int i = 0; i < size; i++) {
    array[i] = rand() % 10;
  }

  serial_odd_even_sort(size, array);

  for (int i = 0; i < size; i++) {
   std::cout << array[i] << std::endl;
  }

  delete [] array;
  return 0;
}

void serial_odd_even_sort(int n, double* array) {
  for (int phase = 0; phase < n; ++phase) {
  if (phase % 2 == 0) {
    for (int i = 1; i < n; i += 2) {
      if (array[i - 1] > array[i]) {
        std::swap(&array[i - 1], &array[i]);
      }
    }
  } else {
    for (int i = 1; i < n - 1; i += 2) {
      if (array[i] > array[i + 1]) {
          std::swap(&array[i - 1], &array[i]);
        }
      }
    }
  }
}
