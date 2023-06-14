// Copyright 2023 <Dario Murillo Chaverri C15406>

#include <mpi.h>
#include <omp.h>
#include <algorithm>
#include <cstdlib>
#include <iostream>
#include <thread>
#include <string>
#include <stdexcept>
#include <thread>


/**
 * @brief prinicipio de mapeo por bloque
 * @param rank numero de hilo
 * @param end final del rango
 * @param workers cantidad de trabajadores
 * @param begin inicio del rango
 * @return int que inidica el inicio del mapeo
*/
int calculate_start(int rank, int end, int workers, int begin);

/**
 * @brief final de mapeo por bloque
 * @param rank numero de hilo
 * @param end final del rango
 * @param workers cantidad de trabajadores
 * @param begin inicio del rango
 * @return int que inidica el final del mapeo
*/
int calculate_finish(int rank, int end, int workers, int begin);

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
    unsigned int thread_number = std::thread::hardware_concurrency();

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
        for (int target = 0; target < process_count; target++) {
          if (MPI_Send(&overall_start, 1, MPI_INT, target
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
            throw std::runtime_error("could not send message");
          }
          if (MPI_Send(&overall_finish, 1, MPI_INT, target
          , /*tag*/ 0, MPI_COMM_WORLD) != MPI_SUCCESS) {
            throw std::runtime_error("could not send message");
          }
        }
      } else {
        if (MPI_Recv(&overall_start, 1, MPI_INT, 0
          , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
          throw std::runtime_error("could not receive message");
        }
        if (MPI_Recv(&overall_finish, 1, MPI_INT, 0
          , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
          throw std::runtime_error("could not receive message");
        }
      }
    }
  
    // inicio de mapeo por bloque
    const int process_start = calculate_start(process_number, overall_finish
      , process_count, overall_start);
    // fin de mapeo por bloque
    const int process_finish = calculate_finish(process_number, overall_finish
      , process_count, overall_start);
    const int process_size = process_finish - process_start;

 

    #pragma omp parallel num_threads(thread_number) \
      default(none) shared(std::cout, process_hostname) \
      shared(process_number, process_start, process_finish)
    {  // NOLINT(whitespace/braces)
      // variable para saber cuando empece
      int thread_start = -1;
      // variable para saber cuando termine
      int thread_finish = -1;

      #pragma omp for schedule(static)
      for (int index = process_start; index < process_finish; ++index) {
        if (thread_start == -1) {
          thread_start = index;
        }
        thread_finish = index;
      }

      // mantiene en la variable donde termina
      ++thread_finish;
      const int thread_size = thread_finish - thread_start;

      #pragma omp critical(can_print)
      std::cout << '\t' << process_hostname << ':' << process_number << '.'
        << omp_get_thread_num() << ": range [" << thread_start << ", " <<
        thread_finish << "[ size " << thread_size << std::endl;
    }


    const double elapsed = MPI_Wtime() - start_time;

    std::cout << process_hostname << ':' << process_number << ": range ["
      << process_start << ", " << process_finish << "[ size " << process_size
      << " in " << elapsed << std::endl;

    MPI_Finalize();
  }
  return 0;
}

int calculate_start(int rank, int end, int workers, int begin) {
  const int range = end - begin;
  // se agrega el begin en caso de que la carga no inicie en 0
  return begin + rank * (range / workers) + std::min(rank, range % workers);
}

int calculate_finish(int rank, int end, int workers, int begin) {
  return calculate_start(rank + 1, end, workers, begin);
}

