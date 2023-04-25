// Copyright 2022 <Dario Murillo Chaverr C15406>
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
  unsigned int seed = time(NULL) ^ pthread_self();
  float number = ((float)rand_r(&seed)/2147483648) * (100-0) + 0;
  if (number < this->packagaProbability) {
    (void) data;
    ++this->lostMessages;
  } else {
    can_change_target.lock();
    data.target = Util::random(1, 
    static_cast<int>(this->consumerCount)+1);
    ++this->redirectedMessages;
    this->produce(data);
    can_change_target.unlock();
  }
}
