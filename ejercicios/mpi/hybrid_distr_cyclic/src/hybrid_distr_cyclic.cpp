// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <stdexcept>
#include <vector>


int main(int argc, char* argv[]) {
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    // obtiene el tiempo para medirlo
    const double start_time = MPI_Wtime();

    int process_number = -1;  // rank
    MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

    int process_count = -1;
    MPI_Comm_size(MPI_COMM_WORLD, &process_count);

    char process_hostname[MPI_MAX_PROCESSOR_NAME];
    int hostname_length = -1;
    MPI_Get_processor_name(process_hostname, &hostname_length);


    int overall_start = 0;
    int overall_finish = 0;
    unsigned int thread_count = std::thread::hardware_concurrency();
    std::vector<int> work;

    if (argc == 3) {
      // si se ingresaron argumentos en la linea de comandos
      overall_start = atoi(argv[1]);
      overall_finish = atoi(argv[2]);
    } else {
      // si no se leen de la entrada estandar
      if (process_number == 0) {
        std::cout << "Valor inicial " << std::endl;
        std::cin >> overall_start;
        std::cout << "Valor final " << std::endl;
        std::cin >> overall_finish;
      }
      // se usa comunicacion colectiva
      if (MPI_Bcast(&overall_start, /*count*/ 1, MPI_INT, /*root*/ 0
       , MPI_COMM_WORLD) != MPI_SUCCESS ) {
        throw std::runtime_error("could not broadcast value count");
      }
      if (MPI_Bcast(&overall_finish, /*count*/ 1, MPI_INT, /*root*/ 0
       , MPI_COMM_WORLD) != MPI_SUCCESS ) {
        throw std::runtime_error("could not broadcast value count");
      }
    }
    // Le asigna al trabajador i todas las unidades de trabajo con indice
    // {i, i + w, i + 2w, etc...}
    for (int i = process_number + overall_start; i < overall_finish
      ; i += process_count) {
      work.push_back(i);
    }
    std::string msg;
    size_t process_size = work.size();
    for (size_t i = 0; i < process_size; i++) {
      if (i != process_size - 1) {
        msg = msg + std::to_string(work[i]) + ", ";
      } else {
        msg = msg + std::to_string(work[i]);
      }
    }


    #pragma omp parallel default(none) num_threads(thread_count) \
        shared(std::cout, process_hostname) \
        shared(process_number, work, thread_count)
    {  // NOLINT(whitespace/braces)
      std::vector<int> my_nums;
      for (size_t index = omp_get_thread_num();
        index < work.size(); index += thread_count) {
        my_nums.push_back(work[index]);
      }
      std::string msg;
      for (size_t i = 0; i < my_nums.size(); i++) {
        if (i != my_nums.size() -1) {
          msg = msg + std::to_string(my_nums[i]) + ", ";
        } else {
          msg = msg + std::to_string(my_nums[i]);
        }
      }
      #pragma omp critical(can_print)
      std::cout << '\t' << process_hostname << ':' << process_number << '.'
        << omp_get_thread_num() << ": range [" << msg << "] size "
        << my_nums.size() << std::endl;
    }

    const double elapsed = MPI_Wtime() - start_time;

    std::cout << process_hostname << ':' << process_number << ": range ["
    << msg << "] size " << process_size
    << " in " << elapsed << std::endl;

    MPI_Finalize();
  }
  return 0;
}

