// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

// DistributedExeption(process_number, exception_code, msg)
#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief subrutina para que cada proceso saque los valores del arreglo
 * @param process_number numero de proceso
 * @param process_hostname nombre de la maquina que ejecuta el proceso
*/
void process_values(int process_number, const char* process_hostname);

int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
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

    try {
      // obtiene el tiempo para medirlo
      const double start_time = MPI_Wtime();
      // hace que cada proceso efectue la subrutina
      process_values(process_number, process_hostname);
      const double elapsed = MPI_Wtime() - start_time;
      std::cout << process_hostname << ":" << process_number
        << ".m: elapsed time " << elapsed << "s" << std::endl;
    } catch (const std::runtime_error& exception) {
      std::cerr << "error: " << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }

    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void process_values(int process_number, const char* process_hostname) {
  std::vector<double> values;
  size_t value_count = 0;
  // el proceso 0 se encarga de recibir los valores de la entrada estandar
  // los guarda en el arreglo
  if (process_number == 0) {
    double value = 0.0;
    while (std::cin >> value) {
      values.push_back(value);
    }
    // value count indica cuantos valores hay en el arreglo
    value_count = values.size();
  }

  // static assert por si el tipo de datos no coincide
  static_assert(sizeof(value_count) == sizeof(uint64_t)
    , "update MPI_Send data type to match your architecture");
  // el proceso 0 se encarga de enviar la cantidad de numeros en el arreglo
  // a todos los demas procesos mediante comunicacion colectiva
  if (MPI_Bcast(&value_count, /*count*/ 1, MPI_UINT64_T, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fail("could not broadcast value count");
  }
  // los procesos ajustan el tamano del arreglo segun el value_count
  // que fue comunicado por el proceso 0
  values.resize(value_count);


  // el proceso 0 se encarga de enviar el arreglo de valores
  // a todos los demas procesos mediante comunicacion colectiva
  if (MPI_Bcast(&values[0], value_count, MPI_DOUBLE, /*root*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS ) {
    fail("could not broadcast values");
  }
  // imprimer los valores del arreglo
  for (size_t index = 0; index < values.size(); ++index) {
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;
  }
}
