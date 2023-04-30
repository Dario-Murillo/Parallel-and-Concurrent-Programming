/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <cstdlib>
#include <iostream>

#include "ProducerConsumerTest.hpp"
#include "ConsumerTest.hpp"
#include "DispatcherTest.hpp"
#include "ProducerTest.hpp"

const char* const usage =
  "Usage: prodcons packages consumers prod_delay disp_delay cons_delay\n"
  "\n"
  "  packages    number of packages to be produced\n"
  "  consumers   number of consumer threads\n"
  "  producers   number of producer threads\n"
  "  prod_delay  delay of producer to create a package\n"
  "  disp_delay  delay of dispatcher to dispatch a package\n"
  "  cons_delay  delay of consumer to consume a package\n"
  "\n"
  "Delays are in millisenconds, negatives are maximums for random delays\n";

ProducerConsumerTest::~ProducerConsumerTest() {
  for ( ProducerTest* producer : this->producers )
    delete producer;
  delete this->dispatcher;
  for ( ConsumerTest* consumer : this->consumers )
    delete consumer;
}

int ProducerConsumerTest::start(int argc, char* argv[]) {
  // Parse arguments and store values in object members
  if ( int error = this->analyzeArguments(argc, argv) ) {
    return error;
  }

  // Create objects for the simulation

  this->dispatcher = new DispatcherTest(this->dispatcherDelay,
    this->producerCount);

  this->dispatcher->createOwnQueue();
  // Create each consumer
  this->consumers.resize(this->consumerCount);
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index] = new ConsumerTest(this->consumerDelay);
    assert(this->consumers[index]);
    this->consumers[index]->createOwnQueue();
  }
  // Create each producer
  this->producers.resize(this->producerCount);
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index] = new ProducerTest(0, this->productorDelay,
      this->consumerCount);
    this->producers[index]->setProducingQueue
      (this->dispatcher->getConsumingQueue());
  }


  // Repartimos la carga de produccion entre los trabajadores
  size_t index = 0;
  for (size_t i = 0; i < packageCount; i++) {
    this->producers[index]->aumentPackageCount();
    index = index + 1;
    if (index == (this->producerCount)) {
      index = 0;
    }
  }


  // Dispatcher delivers to each consumer, and they should be registered
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->dispatcher->registerRedirect(index + 1
      , this->consumers[index]->getConsumingQueue());
  }

  // Start the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->startThread();
  }
  this->dispatcher->startThread();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->startThread();
  }

  // Wait for objets to finish the simulation
  for ( size_t index = 0; index < this->producerCount; ++index ) {
    this->producers[index]->waitToFinish();
  }
  this->dispatcher->waitToFinish();
  for ( size_t index = 0; index < this->consumerCount; ++index ) {
    this->consumers[index]->waitToFinish();
  }

  // Simulation finished
  return EXIT_SUCCESS;
}

int ProducerConsumerTest::analyzeArguments(int argc, char* argv[]) {
  // 5 + 1 arguments are mandatory
  if ( argc != 7 ) {
    std::cout << usage;
    return EXIT_FAILURE;
  }

  int index = 1;
  this->packageCount = std::strtoull(argv[index++], nullptr, 10);
  this->producerCount = std::strtoull(argv[index++], nullptr, 10);
  this->consumerCount = std::strtoull(argv[index++], nullptr, 10);
  this->productorDelay = std::atoi(argv[index++]);
  this->dispatcherDelay = std::atoi(argv[index++]);
  this->consumerDelay = std::atoi(argv[index++]);

  // todo: Validate that given arguments are fine
  return EXIT_SUCCESS;
}
