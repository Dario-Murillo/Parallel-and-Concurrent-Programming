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
  std::string process_hostname;  /// nombre de la maquina corriendo el proceso
 public:
  /// @brief metodo get para el atributo process_number
  /// @return process_number
  int getProcessNumber() const;
  /// @brief metodo get para el atributo process_count
  /// @return process_count
  int getProcessCount() const;
  /// @brief metodo get para el atributo process_hostname
  /// @return process_hostname
  std::string getHostname() const;
  /// @brief obtiene el numero del proceso
  /// @return numero del proceso
  int rank() const;
  /// @brief obtiene la cantidad de procesos
  /// @return cantidad de procesos
  int size() const;
  /// @brief constructor de la clase inicializada el ambiente mpi
  /// asi como los atributos por medio de metodos de mpi
  /// @param argc referencia al numero de argumentos
  /// @param argv referencia a los argumentos
  Mpi(int *argc, char** argv[]);
  /// @brief destructor de la clase, finaliza el ambiente mpi
  /// y libera memoria
  ~Mpi();
};

#endif
