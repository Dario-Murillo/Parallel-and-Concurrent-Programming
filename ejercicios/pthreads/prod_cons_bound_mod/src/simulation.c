// Copyright 2023 <Dario Murillo Chaverri C15406>
#define _DEFAULT_SOURCE

#include <simulation.h>
#include <producer.h>
#include <consumer.h>

/**
 * @brief pone los argumentos de la linea de comandos en la estructura de datos
 * @param arg numero de argumentos
 * @param argv los argumentos
 * @param shared_data la estructura de datos que almacena
 * @return codigo de error
 *         0 si es exitoso
*/
int analyze_arguments(simulation_t* simulation, int argc, char* argv[]);

/**
 * @brief subrutina encargada de crear el hilo productor y consumidor
 * @param shared_data la memoria compartida con todos los datos necesarios
 * @return codigo de error 
*/
int create_threads(simulation_t* simulation);



simulation_t* simulation_create() {
    simulation_t* simulation = (simulation_t*)
    calloc(1, sizeof(simulation_t));

    if (simulation) {
        simulation->thread_count = 0;
        simulation->buffer_capacity = 0;
        simulation->buffer = NULL;
        simulation->rounds = 0;
        simulation->producer_min_delay = 0;
        simulation->producer_max_delay = 0;
        simulation->consumer_min_delay = 0;
        simulation->consumer_max_delay = 0;
    }
    return simulation;
}

void simulation_destroy(simulation_t* simulation) {
    sem_destroy(&simulation->can_consume);
    sem_destroy(&simulation->can_produce);
    free(simulation->buffer);
    free(simulation);
}


int simulation_run(simulation_t* simulation, int argc, char* argv[]) {
    int error = analyze_arguments(simulation, argc, argv);
    if (error == EXIT_SUCCESS) {
        simulation->buffer = (double*)
        calloc(simulation->buffer_capacity, sizeof(double));
        if (simulation->buffer) {
            // inicializacion de semaforos
            sem_init(&simulation->can_produce, 0, simulation->buffer_capacity);
            sem_init(&simulation->can_consume, 0, 0);

            unsigned int seed = 0u;
            // obtener una semilla en hardware
            getrandom(&seed, sizeof(seed), GRND_NONBLOCK);
            srandom(seed);

            struct timespec start_time;
            clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &start_time);

            error = create_threads(simulation);

            struct timespec finish_time;
            clock_gettime(/*clk_id*/CLOCK_MONOTONIC, &finish_time);

            double elapsed = (finish_time.tv_sec - start_time.tv_sec) +
            (finish_time.tv_nsec - start_time.tv_nsec) * 1e-9;
            printf("execution time: %.9lfs\n", elapsed);
      }
    }
    return error;
}

int analyze_arguments(simulation_t* simulation, int argc, char* argv[]) {
    int error = EXIT_SUCCESS;
    if (argc == 7) {
        if (sscanf(argv[1], "%zu", &simulation->buffer_capacity) != 1
        || simulation->buffer_capacity == 0) {
            fprintf(stderr, "error: invalid buffer capacity\n");
            error = ERR_BUFFER_CAPACITY;
        } else if (sscanf(argv[2], "%zu", &simulation->rounds) != 1
        || simulation->rounds == 0) {
            fprintf(stderr, "error: invalid round count\n");
            error = ERR_ROUND_COUNT;
        } else if (sscanf(argv[3], "%u",
        &simulation->producer_min_delay) != 1) {
            fprintf(stderr, "error: invalid min producer delay\n");
            error = ERR_MIN_PROD_DELAY;
        } else if (sscanf(argv[4], "%u",
        &simulation->producer_max_delay) != 1) {
            fprintf(stderr, "error: invalid max producer delay\n");
            error = ERR_MAX_PROD_DELAY;
        } else if (sscanf(argv[5], "%u",
        &simulation->consumer_min_delay) != 1) {
            fprintf(stderr, "error: invalid min consumer delay\n");
            error = ERR_MIN_CONS_DELAY;
        } else if (sscanf(argv[6], "%u",
        &simulation->consumer_max_delay) != 1) {
            fprintf(stderr, "error: invalid max consumer delay\n");
            error = ERR_MAX_CONS_DELAY;
        }
    } else {
        fprintf(stderr, "usage: prod_cons_bound buffer_capacity rounds"
        " producer_min_delay producer_max_delay"
        " consumer_min_delay consumer_max_delay\n");
        error = ERR_NO_ARGS;
    }
    return error;
}

int create_threads(simulation_t* simulation) {
    int error = EXIT_SUCCESS;
    pthread_t producer, consumer;

    error = pthread_create(&producer, /*attr*/ NULL, produce, simulation);
    if (error == EXIT_SUCCESS) {
    error = pthread_create(&consumer, /*attr*/ NULL, consume, simulation);
    if (error != EXIT_SUCCESS) {
        fprintf(stderr, "error: could not create consumer\n");
        error = ERR_CREATE_THREAD;
    }
    } else {
    fprintf(stderr, "error: could not create producer\n");
    error = ERR_CREATE_THREAD;
    }


    if (error == EXIT_SUCCESS) {
        pthread_join(producer, /*value_ptr*/ NULL);
        pthread_join(consumer, /*value_ptr*/ NULL);
    }
    return error;
}

__useconds_t random_between(__useconds_t min, __useconds_t max) {
    return min + (max > min ? (random() % (max - min)) : 0);
}
