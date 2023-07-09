// Copyright 2023 <Dario Murillo Chaverri C15406>

#ifndef MPI_HPP
#define MPI_HPP

#include <mpi.h>
#include <string>
#include <stdexcept>
#include <vector>
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
  /// @brief Envia un mensaje escalar a otro proceso
  /// @tparam DataType
  /// @param value dato a enviar
  /// @param toProcess numero del proceso al cual se va a enviar el mensaje
  /// @param tag tag
  template <typename Type>
  void send(const Type& value, int toProcess, int tag = 0) {
    if (MPI_Send(&value, /*count*/ 1, map(value), toProcess, tag
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw MpiError("No se pudo enviar el mensaje");
    }
  }
  /// @brief Recibe un valor escalar a otro proceso
  /// @tparam DataType
  /// @param value dato a recibir
  /// @param fromProcess numero del proceso del cual recibe el mensaje
  /// @param tag tag
  template <typename Type>
  void receive(Type& value, int fromProcess = MPI_ANY_SOURCE,
    int tag = MPI_ANY_TAG) {
    if (MPI_Recv(&value, /*count*/ 1, map(value), fromProcess,
    tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      throw MpiError("No se pudo recibir el mensaje");
    }
  }
  /// @brief Envia un arreglo de counts a otro proceso
  /// @tparam Type
  /// @param values valores a enviar
  /// @param count numero de valores
  /// @param toProcess proceso a cual enviar el mensaje
  /// @param tag tag
  template <typename Type>
  void send(const Type* values, int count, int toProcess, int tag = 0) {
    if (MPI_Send(values, count, map(values[0]), toProcess, tag
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw MpiError("No se pudo enviar el mensaje");
    }
  }
  /// @brief Espera a recibir maxima cantidad de valores de otro proceso
  /// @tparam Type
  /// @param values valores a recibir
  /// @param capacity cantidad de valores
  /// @param fromProcess proceso del que se recibe el mensaje
  /// @param tag tag
  template <typename Type>
  void receive(Type* values, int capacity, int fromProcess = MPI_ANY_SOURCE,
    int tag = MPI_ANY_TAG) {
    if (MPI_Recv(values, capacity, map(values[0]), fromProcess
      , tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      throw MpiError("No se pudo recibir el mensaje");
    }
  }
  /// @brief Envia arreglo de valores a otro proceso
  /// @tparam Type
  /// @param values arreglo de valores
  /// @param toProcess proceso al cual se va a neviar el arreglo
  /// @param tag tag
  template <typename Type>
  void send(const std::vector<Type>& values, int toProcess, int tag = 0) {
    if (MPI_Send(values.data(), values.size(), map(values[0]), toProcess, tag
      , MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw MpiError("No se pudo enviar el mensaje");
    }
  }
  /// @brief Espera a recibir maxima cantidad de valores de otro proceso
  /// @tparam Type
  /// @param values valores a recibir
  /// @param capacity capacidad
  /// @param fromProcess proceso del que recibe el arreglo
  /// @param tag tag
  template <typename Type>
  void receive(std::vector<Type>& values, int capacity,
    int fromProcess = MPI_ANY_SOURCE, int tag = MPI_ANY_TAG) {
    if (MPI_Recv(values.data(), capacity, map(values[0]), fromProcess
      , tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      throw MpiError("No se pudo recibir el mensaje");
    }
  }
  /// @brief  Envia texto a otro proceso
  /// @param text texto a enviar
  /// @param toProcess proceso al que se envia
  /// @param tag tag
  void send(const std::string& text, int toProcess, int tag = 0) {
    if (MPI_Send(text.data(), text.length(), map(text[0]), toProcess
      , tag, MPI_COMM_WORLD) != MPI_SUCCESS) {
      throw MpiError("No se pudo enviar el mensaje\n");
    }
  }
  /// @brief Recibe texto de otro proceso
  /// @param text texto a recibir
  /// @param capacity capacidad a almacenar
  /// @param fromProcess proceso del que se recibe
  /// @param tag tag
  void receive(std::string& text, int capacity,
  int fromProcess = MPI_ANY_SOURCE, int tag = MPI_ANY_TAG) {
    if (MPI_Recv(&text[0], capacity , map(text[0]), fromProcess
      , tag, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      throw MpiError("No se pudo recibir el mensaje\n");
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
  Mpi(int &argc, char** &argv);
  /// @brief destructor de la clase, finaliza el ambiente mpi
  /// y libera memoria
  ~Mpi();
};

#endif
