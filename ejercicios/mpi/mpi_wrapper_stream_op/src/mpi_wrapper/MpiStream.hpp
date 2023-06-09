// Copyright 2023 <Dario Murillo Chaverri C15406>

#ifndef MPISTREAM_HPP
#define MPISTREAM_HPP

#include <iostream>
#include "Mpi.hpp"

class MpiStream {
 private:
  Mpi mpi;
  int rank;
 public:
  MpiStream(Mpi &mpi, int rank):mpi(mpi), rank(rank){};
  ~MpiStream(){};
  friend std::ostream& operator<<(ostream &os, const MpiStream &mpistream) {
    mpistream.mpi.send();
    return os;
  }
  friend std::istream& operator>>(istream &is, MpiStream &mpistream) {
    mpistream.mpi.receive();
    return is;
  }
};


#endif
