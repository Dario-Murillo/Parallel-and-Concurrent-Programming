// Copyright 2022 <Dario Murillo Chaverr C15406>
#include <time.h>
#include <random>
#include "AssemblerTest.hpp"
#include "iostream"
#include "Log.hpp"

AssemblerTest::AssemblerTest(float packagaProbability,
  size_t consumerCount, size_t packageCount)
  : packagaProbability(packagaProbability) ,
  consumerCount(consumerCount),
  packageCount(packageCount) {
}

int AssemblerTest::run()  {
  this->consumeForever();


  Log::append(Log::INFO, "Assembler", std::to_string(this->lostMessages)
    + " messages lost");
  Log::append(Log::INFO, "Assembler", std::to_string(this->redirectedMessages)
    + " messages redirected");

  return EXIT_SUCCESS;
}

void AssemblerTest::consume(NetworkMessage data) {
  this->mutex.lock();
  float number = 0;
  static std::random_device::result_type seed = std::random_device()();
  static std::mt19937 randomEngine(seed);
  std::uniform_int_distribution<int> randomDistribution(0, 100);
  number = static_cast<float>(randomDistribution(randomEngine));
  this->mutex.unlock();

  if (number < this->packagaProbability &&
    data.messageNumber != this->packageCount + 1) {
    ++this->lostMessages;
    (void) data;
  } else {
    this->mutex.lock();
    int new_target = 0;
    static std::random_device::result_type seed = std::random_device()();
    static std::mt19937 randomEngine(seed);
    std::uniform_int_distribution<int> randomDistribution(1,
      this->consumerCount);
    new_target = static_cast<int>(randomDistribution(randomEngine));
    data.target =  new_target;
    if (data.messageNumber != this->packageCount + 1) {
      this->produce(data);
      ++this->redirectedMessages;
    }
    this->mutex.unlock();
  }
  if (data.messageNumber == this->packageCount + 1) {
      this->produce(NetworkMessage());
  }
}
