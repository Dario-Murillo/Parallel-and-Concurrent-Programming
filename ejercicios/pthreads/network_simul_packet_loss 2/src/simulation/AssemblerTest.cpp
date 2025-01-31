// Copyright 2022 <Dario Murillo Chaverr C15406>
#include <time.h>
#include <random>
#include "AssemblerTest.hpp"
#include "Util.hpp"
#include "iostream"
#include "Log.hpp"

AssemblerTest::AssemblerTest(float packagaProbability,
  size_t consumerCount)
  : packagaProbability(packagaProbability) , consumerCount(consumerCount) {
}

int AssemblerTest::run()  {
  this->consumeForever();

  this->produce(NetworkMessage());


  Log::append(Log::INFO, "Assembler", std::to_string(this->lostMessages)
    + " messages lost");
  Log::append(Log::INFO, "Assembler", std::to_string(this->redirectedMessages)
    + " redirected messages");

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

  if (number < this->packagaProbability) {
    this->lostMessages++;
    (void) data;
  } else {
    this->mutex.lock();
    int new_target = 0;
    static std::random_device::result_type seed = std::random_device()();
    static std::mt19937 randomEngine(seed);
    std::uniform_int_distribution<int> randomDistribution(1,
      this->consumerCount);
    new_target = static_cast<float>(randomDistribution(randomEngine));
    data.target =  new_target;
    this->mutex.unlock();
    ++this->redirectedMessages;
    this->produce(data);
  }
}
