// Copyright 2023 <Dario Murillo Chaverri C15406>

#ifndef MPI_HPP
#define MPI_HPP

#include <mpi.h>
#include <string>
#include <stdexcept>

/// @brief clase wrapper que encapsula funcionalidades de mpi
class Mpi {
 private:
  int process_number;  /// numero de proceso
  int process_count;  /// cantidad total de procesos
  char* process_hostname;  /// nombre de la maquina corriendo el proceso
 public:
  /// @brief metodo get para el atributo process_number
  /// @return process_number
  inline int getProcessNumber();
  /// @brief metodo get para el atributo process_count
  /// @return process_count
  inline int getProcessCount();
  /// @brief metodo get para el atributo process_hostname
  /// @return process_hostname
  inline char* getHostname();
  /// @brief obtiene el numero del proceso
  /// @return numero del proceso
  inline int rank();
  /// @brief obtiene la cantidad de procesos
  /// @return cantidad de procesos
  inline int size();
  /// @brief constructor de la clase inicializada el ambiente mpi
  /// asi como los atributos por medio de metodos de mpi
  /// @param argc referencia al numero de argumentos
  /// @param argv referencia a los argumentos
  Mpi(int *argc, char** argv[]);
  /// @brief destructor de la clase, finaliza el ambiente mpi
  /// y libera memoria
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
        throw std::runtime_error("No se pudo obtener el numero de procesos\n");
      }
      // char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      this->process_hostname = reinterpret_cast<char*>
        (calloc(MPI_MAX_PROCESSOR_NAME, sizeof(char)));
      int hostname_length = -1;
      if (MPI_Get_processor_name(process_hostname, &hostname_length)
          != MPI_SUCCESS) {
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
  free(this->process_hostname);
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

inline int Mpi::rank() {
  return this->process_number;
}

inline int Mpi::size() {
  return this->process_count;
}
#endif
