// Copyright 2023 <Dario Murillo Chaverri C15406>
#include <assert.h>
#include <chrono>
#include <iostream>
#include <memory>
#include <sstream>
#include <thread>
#include <vector>

// thread_shared_data_t
// estructura de datos de memoria compartida
// en este caso el dato que comparten todos los hilos es el total de hilos
typedef struct shared_data {
  uint64_t thread_count;
} shared_data_t;

// thread_private_data_t
// estructura de datos de memoria privada
// en este caso el dato privado de cada hilo, es su respectivo numero
typedef struct private_data {
  uint64_t thread_number;  // rank
  shared_data_t* shared_data;
} private_data_t;

/**
 * @brief ...
 */
void* greet(private_data_t priv_data);
int create_threads(std::shared_ptr<shared_data_t> shared_data);

// procedure main(argc, argv[])
int main(int argc, char* argv[]) {
  try {
    int error = EXIT_SUCCESS;
    uint64_t thread_count = std::thread::hardware_concurrency();
    std::stringstream read;
    if (argc == 2) {
        if (read << argv[1]) {
          read >> thread_count;
        } else {
            throw  error = 11;
        }
    }
    // memoria dinamica encargada de alojar los datos compartidos
    std::shared_ptr<shared_data_t> shared_data(new shared_data_t);
    if (shared_data) {
        shared_data->thread_count = thread_count;

        std::chrono::high_resolution_clock::time_point start_time
        = std::chrono::high_resolution_clock::now();

        error = create_threads(shared_data);

        std::chrono::high_resolution_clock::time_point finish_time
        = std::chrono::high_resolution_clock::now();


        std::chrono::duration<double> elapsed_time =
        std::chrono::duration_cast<std::chrono::duration<double>>
        (finish_time - start_time);
        std::cout << "Tiempo ocupado: " << elapsed_time.count() << std::endl;
    } else {
        throw error = 12;
    }
  } catch (int &error) {
    switch (error) {
    case 11:
      std::cerr << "Invalidad thread count" << std::endl;
      break;
    case 12:
      std::cerr << "Couldn't allocate data" << std::endl;
    default:
      break;
    }
  }
}  // end procedure

int create_threads(std::shared_ptr<shared_data_t> shared_data) {
  int error = EXIT_SUCCESS;

  std::vector<std::thread> threads;
  std::vector<private_data_t> priv_data(shared_data->thread_count);

  for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
      ; ++thread_number) {
    priv_data[thread_number].thread_number = thread_number;
    priv_data[thread_number].shared_data = shared_data.get();
    threads.emplace_back(std::thread(greet, priv_data[thread_number]));
  }
  std::cout << "Hello from main thread\n" << std::endl;

  for (uint64_t thread_number = 0; thread_number < shared_data->thread_count
      ; ++thread_number) {
      threads[thread_number].join();
  }
  return error;
}

void* greet(private_data_t arg) {
  private_data_t* private_data = reinterpret_cast<private_data_t*> (&arg);
  shared_data_t* shared_data = private_data->shared_data;
  std::stringstream message;
  message << "Hello from secondary thread " << private_data-> thread_number
  << " of " << shared_data->thread_count << "\n";
  std::cout << message.str();
  return NULL;
}  // end procedure
