// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <iostream>
#include "Mpi.hpp"

/**
 * @brief main
 * @param argc cantidad de argumentos
 * @param argv argumentos
*/
int main(int argc, char* argv[]) {
  try {
    Mpi mpi(&argc, &argv);
    std::cout << "Hello from main thread of process " << mpi.getProcessNumber()
      << " of " << mpi.getProcessCount() << " on " << mpi.getHostname()
      << std::endl;
  } catch (const std::exception& error) {
    std::cerr << "error: " << error.what() << std::endl;
  }
}

