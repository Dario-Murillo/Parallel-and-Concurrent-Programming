// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0
#include <omp.h>
#include <iostream>

int main(int argc, char* argv[]) {
  int thread_count = omp_get_max_threads();
  if (argc >= 2) {
    thread_count = atoi(argv[1]);
  }

  int iteration_count = thread_count;
  if (argc >= 3) {
    iteration_count = atoi(argv[2]);
  }

  int stage_count = thread_count;
  if (argc >= 4) {
    stage_count = atoi(argv[3]);
  }

  #pragma omp parallel num_threads(thread_count) \
    default(none) shared(stage_count, iteration_count, std::cout)
  {
    for (int count = 0; count < stage_count; count++) {
      #pragma omp for
      for (int iteration = 0; iteration < iteration_count; ++iteration) {
        #pragma omp critical(stdout)
        std::cout << "stage " << count+1 << ": " << omp_get_thread_num() << '/'
          << omp_get_num_threads() << ": iteration " << iteration << '/'
          << iteration_count << std::endl;
      }
      #pragma omp single
      std::cout << std::endl;
    }
  }
}

