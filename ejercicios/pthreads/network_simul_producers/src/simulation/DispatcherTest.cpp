/// @copyright 2020 ECCI, Universidad de Costa Rica. All rights reserved
/// This code is released under the GNU Public License version 3
/// @author Jeisson Hidalgo-Céspedes <jeisson.hidalgo@ucr.ac.cr>

#include "DispatcherTest.hpp"
#include "Util.hpp"

DispatcherTest::DispatcherTest(int dispatcherDelay, int producerCount)
  : dispatcherDelay(dispatcherDelay)
  , producerCount(producerCount) {
}

int DispatcherTest::run() {
  // Dispatch all the network messages we receive to their respective queues
  int aux = 0;
  while (aux < this->producerCount) {
      this->consumeForever();
      aux++;
  }


  // If we exited from the forever loop, the finish message was received
  // For this simulation is OK to propagate it to all the queues
  for ( const auto& pair : this->toQueues ) {
    pair.second->enqueue(this->stopCondition);
  }

  return EXIT_SUCCESS;
}

int DispatcherTest::extractKey(const NetworkMessage& data) const {
  // Propagate a message requires time, simulate it
  // IMPORTANT: This simulation uses sleep() to mimic the process of propagating
  // a message. However, you must NEVER use sleep() for real projects
  Util::sleepFor(this->dispatcherDelay);

  // Return the target where this message is going to
  // as the key to find the queue
  return data.target;
}
