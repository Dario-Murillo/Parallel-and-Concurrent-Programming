// Copyright 2022 <Dario Murillo Chaverr C15406>
#ifndef ASSEMBLERTEST_HPP
#define ASSEMBLERTEST_HPP

#include "Assembler.hpp"
#include "NetworkMessage.hpp"
#include "ConsumerTest.hpp"
#include "ProducerTest.hpp"

class AssemblerTest: public Assembler<NetworkMessage, NetworkMessage> {
    DISABLE_COPY(AssemblerTest);

 public:
    float packagaProbability = 0;
    size_t consumerCount = 0;
    size_t packageCount = 0;
    size_t lostMessages = 0;
    size_t redirectedMessages = 0;
    std::mutex can_change_target;
 public:
    explicit AssemblerTest(float packagaProbability,
      size_t consumerCount, size_t packageCount);
    int run() override;
    void consume(NetworkMessage data) override;
};

#endif

