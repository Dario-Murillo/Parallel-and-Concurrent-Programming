// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <cstdlib>
#include <iostream>
#include <sstream>
#include <stdexcept>
#include <vector>
// macro para invocar al constructor de runtime_error
#define fail(msg) throw std::runtime_error(msg)

// capacidad del mensaje
const size_t MESSAGE_CAPACITY = 512;

/**
 * @brief mensaje de saludo de los procesos
 * @param process_number numero de proceso
 * @param process_count cantidad de procesos
 * @param process_hostname nombre del procesador o maquina ejecutando el proceso
*/
void greet(int process_number, int process_count, const char* process_hostname);

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
    // nombre de la maquina
    MPI_Get_processor_name(process_hostname, &hostname_length);

    try {
      // manda a los procesos a ejecutar el saludo
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
  // guarda el mensaje en el buffer
  std::stringstream buffer;
  buffer << "Hello from main thread of process " << process_number
    << " of " << process_count << " on " << process_hostname;

  if (process_number != 0) {
    // si no es el primer proceso, guarda el buffer en un string
    // y lo envia
    const std::string& message = buffer.str();
    // se usa la subrutina data ya que se necesitan los bytes del mensaje
    // lenght se usa para obtener el largo del string
    if (MPI_Send(message.data(), message.length(), MPI_CHAR, /*target*/ 0
      , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
      fail("could not send message");
    }
  } else {
    // el proceso 0 se encarga de recibir los mensajes
    // imprime su mensaje
    std::cout << buffer.str() << std::endl;
    // recibe los mensajes de todos los procesos menos de el mismo
    // ya que un proceso no puede enviar mensajes a si mismo
    for (int current = 1; current < process_count; ++current) {\
      // crea un arreglo para recibir el mensaje
      std::vector<char> message(MESSAGE_CAPACITY);
      // ahora ya no recibimos los mensajes en orden si no que recibimos
      // del cualquier proceso que llegue en la red de primer
      if (MPI_Recv(&message[0], MESSAGE_CAPACITY, MPI_CHAR, MPI_ANY_SOURCE
        , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
        fail("could not receive message");
      }
      std::cout << &message[0] << std::endl;
    }
  }
}
