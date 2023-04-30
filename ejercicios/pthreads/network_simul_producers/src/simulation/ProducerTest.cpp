/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include <random>
#include "ProducerTest.hpp"
#include "Log.hpp"
#include "Util.hpp"

ProducerTest::ProducerTest(size_t packageCount, int productorDelay
  , size_t consumerCount)
  : packageCount(packageCount)
  , productorDelay(productorDelay)
  , consumerCount(consumerCount) {
}

int ProducerTest::run() {
  // Produce each asked message
  for ( size_t index = 0; index < this->packageCount; ++index ) {
    this->produce(this->createMessage(index));
    this->produced++;
  }

  // Produce an empty message to communicate we finished

  this->produce(NetworkMessage());
   

  // Report production is done
  Log::append(Log::INFO, "Producer", std::to_string(this->produced)
    + " menssages sent");
  return EXIT_SUCCESS;
}

NetworkMessage ProducerTest::createMessage(size_t index) const {
  // Source is always 1: this producer
  const uint16_t source = 1;
  // Target is selected by random
  

  unsigned int seed = std::random_device()();
  const uint16_t target =  1 + (rand_r(&seed) % this->consumerCount);
  // IMPORTANT: This simulation uses sleep() to mimics the process of
  // producing a message. However, you must NEVER use sleep() for real projects

  // Sleep for the given amount of milliseconds
  std::this_thread::sleep_for(std::chrono::milliseconds(this->productorDelay));
  // Create and return a copy of the network message
  return NetworkMessage(target, source, index);
}

void ProducerTest::aumentPackageCount() {
  this->packageCount++;
}