#include <mpi.h>
#include <iostream>
#include <stdexcept>

#include "UniformRandom.hpp"

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief genera numeros pseudoaleatorios e imprime las estadisticas
 * @param process_number numerom de proceso
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
  // Obtiene un numero random
  const int my_lucky_number = uniformRandom.between(0, 100);

  std::cout << "Process " << process_number << ": my lucky number is "
    << my_lucky_number << std::endl;

  int all_min = -1;
  int all_max = -1;
  int all_sum = -1;

  // Update distributed statistics from processes' lucky numbers
  // MPI reduce se llama con distintas operaciones
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_min, /*count*/ 1
    , MPI_INT,/*min*/ MPI_MIN, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce min");
  }
  // Root 0 porque es el proceso donde quiero que termine la reduccion
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_max, /*count*/ 1
    , MPI_INT, /*max*/ MPI_MAX, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce max");
  }
  if (MPI_Reduce(/*input*/ &my_lucky_number, /*output*/ &all_sum, /*count*/ 1
    , MPI_INT, /*sumas*/ MPI_SUM, /*root*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not reduce sum");
  }

  // El proceso 0 se encarga de imprimir las estadisticas
  if (process_number == 0) {
    // Averigua el promedio del total de sumas
    const double all_average = double(all_sum) / process_count;
    std::cout << "Process " << process_number << ": all minimum = "
      << all_min << std::endl;
    std::cout << "Process " << process_number << ": all average = "
      << all_average << std::endl;
    std::cout << "Process " << process_number << ": all maximum = "
      << all_max << std::endl;
  }
}
