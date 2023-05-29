// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdint>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>

#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief imprime los valores del arreglo introducidos
 * en la entrada estandar
 * @param process_number numero de proceso
 * @param process_count cantidad de procesos
 * @param process_hostname nombre de la maquina que ejecta el proceso
 * 
*/
void process_values(int process_number, int process_count
  , const char* process_hostname);

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
      // MPI_Wtime mide el tiempo de ejecuccion
      const double start_time = MPI_Wtime();
      // manda los procesos a ejecutar la subrutina
      process_values(process_number, process_count, process_hostname);
      // termina la ejecuccion y mide cuanto tiempo duro
      const double elapsed = MPI_Wtime() - start_time;
      // impresion
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

void process_values(int process_number, int process_count
  , const char* process_hostname) {
  // crea un arreglo para almacenar los valores
  std::vector<double> values;
  // value count se declara desde fuera para que el recive
  // y el send pueda ver la variable
  size_t value_count = 0;

  if (process_number == 0) {
    // solo el proceso 0 va a obtener los valores de la entrada estandar
    double value = 0.0;
    while (std::cin >> value) {
      // agrega los valores al arreglo
      values.push_back(value);
    }

    // asigna a value_count cuantos valores tiene el arreglo
    value_count = values.size();

    for (int target = 1; target < process_count; ++target) {
      static_assert(sizeof(value_count) == sizeof(uint64_t)
        , "update MPI_Send data type to match your architecture");
      // antes de enviar el arreglo de valores envia a todos los procesos
      // la cantidad de valores que tiene el arreglo.
      // los mensajes se reciben en el orden en que se enviraron
      if (MPI_Send(&value_count, /*count*/ 1, MPI_UINT64_T, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send value count");
      }
      // envia el arreglo de valores
      if (MPI_Send(&values[0], value_count, MPI_DOUBLE, target
        , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
        fail("could not send values");
      }
    }
  } else {
    // recibe la cantidad de valores del arreglo
    if (MPI_Recv(&value_count, /*capacity*/ 1, MPI_UINT64_T, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive value count");
    }
    // ajusta el arreglo para que sea de la capacidad necesaria
    values.resize(value_count);

    // una vez que ajusto el arreglo lo recibe
    if (MPI_Recv(&values[0], /*capacity*/ value_count, MPI_DOUBLE, /*source*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive values");
    }
  }
  // imprime el numero de proceso la cantidad de procesos, asi como los valores
  // que contiene el arreglo que recibio por medio del recv
  for (size_t index = 0; index < values.size(); ++index) {
    std::cout << process_hostname << ":" << process_number << ".m: values["
      << index << "] == " << values[index] << std::endl;
  }
}
