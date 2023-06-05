#include <mpi.h>
#include <iostream>
#include <stdexcept>

#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief genera numeros pseudoaleatorios e imprime las estadisticas
 * @param process_number numero de proceso
 * @param process_count cantidad de procesos
*/
void generate_lucky_statistics(int process_number, int process_count);

// main
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int process_number = -1;
      // numero de proceso
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      // cantidad de procesos
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      // nombre de la maquina que ejecuta el proceso
      MPI_Get_processor_name(process_hostname, &hostname_length);

      generate_lucky_statistics(process_number, process_count);
    } catch (const std::runtime_error& exception) {
      std::cout << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void generate_lucky_statistics(int process_number, int process_count) {
  // Generate my lucky number
  UniformRandom<int> uniformRandom(process_number);
  const int my_lucky_number = uniformRandom.between(0, 100);

  std::cout << "Process " << process_number << ": my lucky number is "
    << my_lucky_number << std::endl;

  int all_min = -1;
  int all_max = -1;
  int all_sum = -1;

  // Update distributed statistics from processes' lucky numbers
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_min, /*count*/ 1
    , MPI_INT, MPI_MIN, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");
  }
  // Ya no hay raiz
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_max, /*count*/ 1
    , MPI_INT, MPI_MAX, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");
  }
  if (MPI_Allreduce(/*input*/ &my_lucky_number, /*output*/ &all_sum, /*count*/ 1
    , MPI_INT, MPI_SUM, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");
  }

  // Compara e imprime las estadisticas
  const double all_average = double(all_sum) / process_count;
  if (my_lucky_number == all_min) {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is the minimum (" << all_min << ")" << std::endl;
  }
  if (my_lucky_number < all_average) {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is less than the average (" << all_average
      << ")" << std::endl;
  } else if (my_lucky_number > all_average) {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is greater than the average (" << all_average
      << ")" << std::endl;
  } else {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is equal to the average (" << all_average
      << ")" << std::endl;
  }
  if (my_lucky_number == all_max) {
    std::cout << "Process " << process_number << ": my lucky number ("
      << my_lucky_number << ") is the maximum (" << all_max << ")" << std::endl;
  }
}
