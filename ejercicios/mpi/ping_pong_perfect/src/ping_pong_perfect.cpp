// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <unistd.h>
#include <mpi.h>
#include <iostream>
#include "Mpi.hpp"

/**
 * @brief simulacion de un juego de pingpong infinito entre
 * 2 procesos donde ninguno pierde ni falla
 * @param mpi referencia al objeto de clase mpi
 * @param process_number numero de proceso
 * @param wait cantidad de segundos a esperar entre cada tiro
*/
void ping_pong(Mpi &mpi, int process_number, int wait);

/**
 * @brief main
 * @param argc cantidad de argumentos
 * @param argv argumentos
*/
int main(int argc, char* argv[]) {
  try {
    Mpi mpi(&argc, &argv);
    const int rank = mpi.getProcessNumber();
    const int count = mpi.getProcessCount();
    if (count != 2) {
      throw MpiError("La cantidad de procesos debe ser 2");
    }
    int wait = 10000;
    if (argc == 2) {
      wait = std::stoi(argv[1]);
    }
    ping_pong(mpi, rank, wait);
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
  }
}

void ping_pong(Mpi &mpi, int process_number, int wait) {
  bool can_serve = true;

  while (true) {
    if (process_number == 0) {
      mpi.send(can_serve, 1);
      std::cout << process_number << " serves" << std::endl;
      usleep(wait);
      mpi.receive(can_serve, 1);
    }
    if (process_number != 0) {
      mpi.receive(can_serve, 0);
      std::cout << process_number << " serves" << std::endl;
      usleep(wait);
      mpi.send(can_serve, 0);
    }
  }
}
