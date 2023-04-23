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
public:
    explicit AssemblerTest(float packagaProbability, size_t consumerCount = 0);
    int run() override;
    void consume(NetworkMessage data) override;
};

#endif
