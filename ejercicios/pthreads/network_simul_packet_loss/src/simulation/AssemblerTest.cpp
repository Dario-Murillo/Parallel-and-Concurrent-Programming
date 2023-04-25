// Copyright 2022 <Dario Murillo Chaverr C15406>
#include <time.h>
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
  Log::append(Log::INFO, "Assembler", std::to_string(this->lostMessages)
    + " messages lost");
  Log::append(Log::INFO, "Assembler", std::to_string(this->redirectedMessages)
    + " messages redirected");
  
  return EXIT_SUCCESS;
}

void AssemblerTest::consume(NetworkMessage data) {
  can_change_target.lock();
  unsigned int seed = time(NULL) ^ pthread_self();
  float number = ((float)rand_r(&seed)/2147483648) * (100-0) + 0;
  if (number < this->packagaProbability) {
    this->lostMessages++;
    (void) data;
    can_change_target.unlock();
  } else {
    unsigned int r_seed = time(NULL) ^ pthread_self();
    data.target =  1 + (rand_r(&r_seed) % this->consumerCount);
    ++this->redirectedMessages;
    can_change_target.unlock();
    this->produce(data);
  }
}
