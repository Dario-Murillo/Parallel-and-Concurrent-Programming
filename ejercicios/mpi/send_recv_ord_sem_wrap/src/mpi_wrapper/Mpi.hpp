// Copyright 2023 <Dario Murillo Chaverri C15406>

#ifndef MPI_HPP
#define MPI_HPP

#include <mpi.h>
#include <string>
#include <stdexcept>
#include "MpiError.hpp"


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
  /// @brief metodo plantilla para enviar mensajes a otros procesos
  /// @tparam DataType
  /// @param message mensaje o datos a comunicar a otro proceso
  /// @param toProcess numero del proceso al cual se va a enviar el mensaje
  /// @param tag tag
  template <typename DataType>
  void send(const DataType& message, int toProcess, int tag = 0) {
    if (MPI_Send(&message, /*count*/ 1, map(message), toProcess, tag
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw MpiError("No se pudo enviar el mensaje");
    }
  }
  /// @brief metodo plantilla para recibir mensajes de otros procesos
  /// @tparam DataType
  /// @param message mensaje o datos a recibir de otros proceso
  /// @param fromProcess numero del proceso del cual recibe el mensaje
  /// @param tag tag
  template <typename DataType>
  void receive(DataType& message, int fromProcess, int tag = MPI_ANY_SOURCE) {
    if (MPI_Recv(&message, /*count*/ 1, map(message), fromProcess
    , tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      throw MpiError("No se pudo recibir el mensaje");
    }
  }
  /// @brief metodos para devolver tipos de datos de MPI
  /// @return tipo de datos MPI
  static inline MPI_Datatype map(bool) { return MPI_C_BOOL; }
  static inline MPI_Datatype map(char) { return MPI_CHAR; }
  static inline MPI_Datatype map(unsigned char) { return MPI_UNSIGNED_CHAR; }
  static inline MPI_Datatype map(short) { return MPI_SHORT; }
  static inline MPI_Datatype map(unsigned short) { return MPI_UNSIGNED_SHORT; }
  static inline MPI_Datatype map(int) { return MPI_INT; }
  static inline MPI_Datatype map(unsigned) { return MPI_UNSIGNED; }
  static inline MPI_Datatype map(long) { return MPI_LONG; }
  static inline MPI_Datatype map(unsigned long) { return MPI_UNSIGNED_LONG; }
  static inline MPI_Datatype map(long long) { return MPI_LONG_LONG; }
  static inline MPI_Datatype map(unsigned long long)
  { return MPI_UNSIGNED_LONG_LONG; }
  static inline MPI_Datatype map(float) { return MPI_FLOAT; }
  static inline MPI_Datatype map(double) { return MPI_DOUBLE; }
  static inline MPI_Datatype map(long double) { return MPI_LONG_DOUBLE; }
  /// @brief constructor de la clase inicializada el ambiente mpi
  /// asi como los atributos por medio de metodos de mpi
  /// @param argc referencia al numero de argumentos
  /// @param argv referencia a los argumentos
  Mpi(int argc, char** &argv);
  /// @brief destructor de la clase, finaliza el ambiente mpi
  /// y libera memoria
  ~Mpi();
};

#endif
