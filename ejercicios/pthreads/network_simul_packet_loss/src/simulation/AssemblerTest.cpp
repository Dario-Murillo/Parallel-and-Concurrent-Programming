// Copyright 2022 <Dario Murillo Chaverr C15406>
#include "AssemblerTest.hpp"
#include "Util.hpp"
#include "iostream"

AssemblerTest::AssemblerTest(float packagaProbability,
  size_t consumerCount)
  : packagaProbability(packagaProbability) , consumerCount(consumerCount) {
}

int AssemblerTest::run()  {
  this->consumeForever();
  return EXIT_SUCCESS;
}

void AssemblerTest::consume(NetworkMessage data) {
  unsigned int seed = time(NULL) ^ pthread_self();
  float number = ((float)rand_r(&seed)/2147483648) * (100-0) + 0;
  if (number < this->packagaProbability) {
    (void) data;
  } else {
    can_change_target.lock();
    data.target = Util::random(0
    , static_cast<int>(this->consumerCount + 1)); ;
    produce(data);
    can_change_target.unlock();
  }
}
