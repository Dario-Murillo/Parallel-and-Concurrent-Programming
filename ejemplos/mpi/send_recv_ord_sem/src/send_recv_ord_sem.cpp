// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <stdexcept>

// DistributedExeption(process_number, exception_code, msg)
#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief proceso para enviar saludo y enviar mensajes a otro proceso
 * @param process_number numero de proceso
 * @param process_count cantidad de procesos
 * @param process_hostname nombre del host ejecutando el proceso
*/
void greet(int process_number, int process_count, const char* process_hostname);

/**
 * @brief hilo principal
 * @param argc cantidad de argumentos
 * @param argv argumentos
 * @return codigo de error
*/
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    // obtiene el numero de proceso
    int process_number = -1;
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    // obtiene la cantidad de procesos
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
    int hostname_length = -1;
    // obtiene el nombre de la maquina que ejecuta el proceso
    MPI_Get_processor_name(process_hostname, &hostname_length);

    // hacer que el proceso ejecute el saludo
    try {
      greet(process_number, process_count, process_hostname);
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

void greet(int process_number, int process_count
    , const char* process_hostname) {
  // proceso anterior
  const int previous_process = (process_count + process_number - 1)
    % process_count;
  // siguiente proceso
  const int next_process = (process_number + 1) % process_count;
  // el mensaje a enviar puede ser cualquier cosa en este caso es un bool
  bool can_print = true;

  if (process_number > 0) {
    // receive(&can_print, 1, previous_process)
    // si no es el primer proceso, espere a recibir un mensaje
    if (MPI_Recv(&can_print, /*count*/ 1, MPI_C_BOOL, previous_process
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("could not receive message");
    }
  }

  // se imprime el saludo
  std::cout << "Hello from main thread of process " << process_number
    << " of " << process_count << " on " << process_hostname << std::endl;

  // send(can_print, 1, next_process);
  // luego de que un ejecutante imprimer el saludo envia un mensaje al siguiente
  if (MPI_Send(&can_print, /*count*/ 1, MPI_C_BOOL, next_process, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("could not send message");
  }
}
