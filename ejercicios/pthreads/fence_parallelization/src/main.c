// Copyright 2023 <Dario Murillo Chaverri C15406>
#define _POSIX_C_SOURCE 199309L
#include <time.h>
#include <unistd.h>
#include <stdio.h>

#include "fence.h"

int main(int argc, char* argv[]) {
  shared_data_t* shared_data =
   (shared_data_t*) calloc(1, sizeof(shared_data_t));

  if ( shared_data == NULL ) {
    return (void)fprintf(stderr,
      "error: could not allocate shared memory\n"), 1;
  }

  pthread_mutex_init(&shared_data->mutex, NULL);

  // numero default de hilos
  shared_data->thread_count = sysconf(_SC_NPROCESSORS_ONLN);
  if ( argc >= 2 ) {
    shared_data->thread_count = strtoull(argv[1], NULL, 10);
  }

  if (read_terrain(shared_data)) {
    return 2;
  }

  struct timespec start_time;
  clock_gettime(CLOCK_MONOTONIC, &start_time);

  // Find the maximum perimeter
  int error = create_threads(shared_data);
  if (error) {
    return error;
  }

  struct timespec finish_time;
  clock_gettime(CLOCK_MONOTONIC, &finish_time);

  double elapsed_seconds = finish_time.tv_sec - start_time.tv_sec
    + 1e-9 * (finish_time.tv_nsec - start_time.tv_nsec);

  // If maximum perimeter is 0
  if ( shared_data->maximum_perimeter == 0 ) {
    // Print impossible
    puts("impossible");
  } else {
    // Print maximum perimeter and its coordinates
    print_maximum_perimeter(shared_data);
  }

  fprintf(stderr, "Hello execution time %.9lfs\n", elapsed_seconds);

  pthread_mutex_destroy(&shared_data->mutex);
  free(shared_data);
  return 0;
}
