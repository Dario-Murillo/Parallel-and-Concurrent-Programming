#include "MpiError.hpp"

class Mpi;

explicit MpiError::MpiError(const std::string& message) {
  std::runtime_error(message);
}

MpiError::MpiError(const std::string& message, const Mpi& mpi) {
  std::runtime_error( );
}

MpiError::MpiError(const std::string& message, 
  const Mpi& mpi, const int threadNumber) {
  std::runtime_error( );
}

MpiError::MpiError() {}

MpiError::~MpiError() {}

