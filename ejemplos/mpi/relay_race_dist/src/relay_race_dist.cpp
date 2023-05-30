// Copyright 2021 Jeisson Hidalgo <jeisson.hidalgo@ucr.ac.cr> CC-BY 4.0

#include <mpi.h>
#include <unistd.h>

#include <cstdlib>
#include <iostream>
#include <stdexcept>
// macro para definir mensajes de error
#define fail(msg) throw std::runtime_error(msg)

/**
 * @brief controlador prinicipal de la carrera
 * @param argc cantidad de argumentos
 * @param argv argumentos
 * @param process_number numero de proceso
 * @param process_count cantidad de procesos
*/
void simulate_relay_race(int argc, char* argv[], int process_number
  , int process_count);

/**
 * @brief primera etapa de la carrera la corren los primeros procesos
 * de cada equipo de procesos
 * @param stage1_delay delay de la primer etapa
 * @param process_number numero de proceso
 * @param process_count cantidad de procesos
 * 
*/
void run_stage1(int stage1_delay, int process_number, int team_count);

/**
 * @brief segunda etapa de la carrera la corren los segundos procesos
 * de cada equipo de procesos luego de haber recibido el baton del 
 * proceso de su equipo en la primer etapa
 * @param stage1_delay delay de la primer etapa
 * @param process_number numero de proceso
 * @param process_count cantidad de procesos
 * 
*/
void run_stage2(int stage2_delay, int process_number, int team_count);

/**
 * @brief arbitro se encarga de recibir a todos los equipos e indicar
 * en que posicion llego cada uno
 * @param team_count cantidad de equipos
 * 
*/
void referee(int team_count);

// el main se encarga de inicializar el ambiente distribuido y sus variables
int main(int argc, char* argv[]) {
  int error = EXIT_SUCCESS;
  if (MPI_Init(&argc, &argv) == MPI_SUCCESS) {
    try {
      int process_number = -1;
      // numero de proceso
      MPI_Comm_rank(MPI_COMM_WORLD, &process_number);

      int process_count = -1;
      // cantidad de procesos
      MPI_Comm_size(MPI_COMM_WORLD, &process_count);

      char process_hostname[MPI_MAX_PROCESSOR_NAME] = { '\0' };
      int hostname_length = -1;
      // nombre de la maquina procesadora
      MPI_Get_processor_name(process_hostname, &hostname_length);

      // subrutina que simula la carrera
      simulate_relay_race(argc, argv, process_number, process_count);
    } catch (const std::runtime_error& exception) {
      std::cout << exception.what() << std::endl;
      error = EXIT_FAILURE;
    }
    MPI_Finalize();
  } else {
    std::cerr << "error: could not init MPI" << std::endl;
    error = EXIT_FAILURE;
  }
  return error;
}

void simulate_relay_race(int argc, char* argv[], int process_number
  , int process_count) {
  if (argc == 3) {
    if (process_count >= 3 && process_count % 2 == 1) {
      // obtiene la cantidad de equipos debe ser un numero impar mayor a 3
      const int team_count = (process_count - 1) / 2;
      const int stage1_delay = atoi(argv[1]);
      const int stage2_delay = atoi(argv[2]);

      // el proceso 0 hace rol de arbitro
      if (process_number == 0) {
        referee(team_count);
      } else if (process_number <= team_count) {
        // los primeros procesos hacen la primer etapa de la carrera
        run_stage1(stage1_delay, process_number, team_count);
      } else {
        // los demas hacen la segunda etapa de la carrera
        run_stage2(stage2_delay, process_number, team_count);
      }
    } else {
      fail("error: process count must be odd and greater than 3");
    }
  } else {
    fail("usage: relay_race_dist stage1_delay stage2_delay");
  }
}

void run_stage1(int stage1_delay, int process_number, int team_count) {
  // wait_barrier()
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  usleep(1000 * stage1_delay);
  // calcula el miembro del equipo al que debe enviar el mensaje
  const int peer = process_number + team_count;
  // el baton es un booleano que indica si puede hacer la segunda parte
  bool baton = true;
  // send(&baton, 1, peer)
  if (MPI_Send(&baton, /*count*/ 1, MPI_C_BOOL, peer, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not send baton");
  }
}

void run_stage2(int stage2_delay, int process_number, int team_count) {
  // wait_barrier()
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // obtiene el numero del proceso de el que debe recibir el mensaje
  int peer = process_number - team_count;
  // variable para almacenar el mensaje que va a recibir
  bool baton = false;
  // receive(&baton, 1, peer)
  if (MPI_Recv(&baton, /*capacity*/ 1, MPI_C_BOOL, /*source*/ peer
    , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
    fail("could not receive baton");
  }
  usleep(1000 * stage2_delay);
  // send(&peer, 1, 0)
  // una vez que termino la carrera se reportan al proceso 0, osea el arbitro
  if (MPI_Send(&peer, /*count*/ 1, MPI_INT, /*target*/ 0, /*tag*/ 0
    , MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could not send team number to referee");
  }
}

void referee(int team_count) {
  const double start_time = MPI_Wtime();
  // wait_barrier()
  if (MPI_Barrier(MPI_COMM_WORLD) != MPI_SUCCESS) {
    fail("error: could wait for barrier");
  }
  // posicion en que llego cada equipo
  int place = 0;
  for (int index = 0; index < team_count; ++index) {
    int team = 0;
    // receive(&team, 1, any_process)
    // recibe el mensaje del segundo corredor de cada equipo
    // reporta en que posicion llego el equipo
    if (MPI_Recv(&team, /*capacity*/ 1, MPI_INT, MPI_ANY_SOURCE
      , /*tag*/ 0, MPI_COMM_WORLD, MPI_STATUS_IGNORE) != MPI_SUCCESS ) {
      fail("error: could not receive team number");
    }
    // calcula el tiempo de ejecuccion desde la perspectiva del arbitro
    const double elapsed = MPI_Wtime() - start_time;
    ++place;
    std::cout << "Place " << place << ": team " << team << " in " << elapsed
      << "s" << std::endl;
  }
}
