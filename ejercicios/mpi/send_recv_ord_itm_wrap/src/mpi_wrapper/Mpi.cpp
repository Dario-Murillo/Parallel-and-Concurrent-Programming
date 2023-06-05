// Copyright 2023 <Dario Murillo Chaverri C15406>

#include "Mpi.hpp"

Mpi::Mpi(int* argc, char** argv[]) {
  try {
    if (MPI_Init(argc, argv) == MPI_SUCCESS) {
      this->process_number = -1;
      if (MPI_Comm_rank(MPI_COMM_WORLD, &process_number) != MPI_SUCCESS) {
        throw MpiError("Error al obtener el numero de proceso");
      }
      this->process_count = -1;
      if (MPI_Comm_size(MPI_COMM_WORLD, &process_count) != MPI_SUCCESS) {
        throw MpiError("Error al obtener el total de procesos\n");
      }
      // char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      char host[MPI_MAX_PROCESSOR_NAME];
      int hostname_length = -1;
      if (MPI_Get_processor_name(host, &hostname_length)
          != MPI_SUCCESS) {
        throw MpiError("Error al obtener el nombre del proceso\n");
      }
      this->process_hostname = std::string(host, hostname_length);
    } else {
      throw MpiError("No se pudo inicializar el ambiente MPI\n");
    }
  }
  catch(std::runtime_error& e) {
    std::cerr << e.what() << '\n';
  }
}

Mpi::~Mpi() {
  MPI_Finalize();
}

int Mpi::getProcessNumber() const {
  return this->process_number;
}

int Mpi::getProcessCount() const {
  return this->process_count;
}

std::string Mpi::getHostname() const {
  return this->process_hostname;
}

int Mpi::rank() const {
  return this->process_number;
}

int Mpi::size() const {
  return this->process_count;
}

