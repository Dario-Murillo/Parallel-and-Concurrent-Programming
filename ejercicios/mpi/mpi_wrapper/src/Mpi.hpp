// Copyright 2023 <Dario Murillo Chaverri C15406>

#ifndef MPI_HPP
#define MPI_HPP

#include <mpi.h>
#include <string>
#include <stdexcept>

class Mpi {
 private:
  int process_number;
  int process_count;
  char* process_hostname;
 public:
  inline int getProcessNumber();
  inline int getProcessCount();
  inline char* getHostname();
  Mpi(int *argc, char** argv[]);
  ~Mpi();
};

Mpi::Mpi(int* argc, char** argv[]) {
  try {
    if (MPI_Init(argc, argv) == MPI_SUCCESS) {
      this->process_number = -1;
      if (MPI_Comm_rank(MPI_COMM_WORLD, &process_number) != MPI_SUCCESS) {
        throw std::runtime_error("No se pudo obtener el numero de proceso\n");
      }
      this->process_count = -1;
      if (MPI_Comm_size(MPI_COMM_WORLD, &process_count) != MPI_SUCCESS) {
        throw std::runtime_error("No se pudo obtener el numero total de procesos\n");
      }
      // char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      this->process_hostname = (char*)calloc(MPI_MAX_PROCESSOR_NAME, sizeof(char));
      int hostname_length = -1;
      if (MPI_Get_processor_name(process_hostname, &hostname_length)) {
        throw std::runtime_error("No se pudo obtener el nombre del proceso\n");
      }
    } else {
      throw std::runtime_error("No se pudo inicializar el ambiente MPI\n");
    } 
  }
  catch(std::runtime_error& e) {
    std::cerr << e.what() << '\n';
  }   
}

Mpi::~Mpi() {
  MPI_Finalize();
}

inline int Mpi::getProcessNumber() {
  return this->process_number;
}

inline int Mpi::getProcessCount() {
  return this->process_count;
}

inline char* Mpi::getHostname() {
  return this->process_hostname;
}

#endif
