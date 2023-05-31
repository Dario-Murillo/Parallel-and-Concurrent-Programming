// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <stdexcept>
#include <sstream>
#include <string>
#include "Mpi.hpp"

#ifndef MPIERROR_HPP
#define MPIERROR_HPP

// forward declaration
class Mpi;

/// @brief clase wrapper para transportar errores en tiempo de ejecuccion
class MpiError: public std::runtime_error {
 public:
  /// @brief constructor de la clase
  /// @param message mensaje de error
  explicit MpiError(const std::string& message):std::runtime_error(message){}
  /// @brief reporta un mensaje de error
  /// @param message mensaje de error
  /// @param mpi instancia de la clase mpi
  MpiError(const std::string& message, const Mpi& mpi);
  /// @brief reporta un mensaje de error
  /// @param message mensaje de error
  /// @param mpi instancia de la clase mpi
  /// @param threadNumber numero de hilos
  MpiError(const std::string& message, const Mpi& mpi, const int threadNumber);
};

#endif
