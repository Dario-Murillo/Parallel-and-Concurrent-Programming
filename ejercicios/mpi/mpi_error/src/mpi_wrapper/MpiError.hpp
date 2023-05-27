#include <stdexcept>
#include "Mpi.hpp"

#ifndef MPIERROR_HPP
#define MPIERROR_HPP

// forward declaration
class Mpi;

class MpiError: public std::runtime_error {
 private:
    
 public:
  explicit MpiError(const std::string& message);
  MpiError(const std::string& message, const Mpi& mpi);
  MpiError(const std::string& message, const Mpi& mpi, const int threadNumber);
  MpiError();
  ~MpiError();
};

#endif
