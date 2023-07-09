// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <iostream>
#include "Mpi.hpp"

const size_t MESSAGE_CAPACITY = 512;
/**
 * @brief main
 * @param argc cantidad de argumentos
 * @param argv argumentos
*/
int main(int argc, char* argv[]) {
  try {
    Mpi mpi(argc, argv);
    const int process_number = mpi.getProcessNumber();
    const int process_count = mpi.getProcessCount();
    std::string process_hostname = mpi.getHostname();

    std::stringstream buffer;
    buffer << "Hello from main thread of process " << process_number
      << " of " << process_count << " on " << process_hostname;

    if (process_number != 0) {
      // si no es el primer proceso, guarda el buffer en un string
      const std::string& text = buffer.str();
      mpi.send(text, 0, 0);
    } else {
      // el proceso 0 se encarga de recibir los mensajes
      // imprime su mensaje
      std::cout << process_number << " said " << buffer.str() << std::endl;
      // recibe los mensajes de todos los procesos menos de el mismo
      // ya que un proceso no puede enviar mensajes a si mismo
      for (int source = 1; source < process_count; ++source) {
        // crea un string para recibir el mensaje
        std::string message;
        message.reserve(MESSAGE_CAPACITY);
        mpi.receive(message, MESSAGE_CAPACITY, source);
        std::cout << source << " sent " << &message[0] << std::endl;
      }
    }
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
  }
}

