//
// Created by shlominissan on 12/21/19.
//

#ifndef CHIP8_RAND_H
#define CHIP8_RAND_H

#include <random>

class Rand {
public:
    Rand(int low, int high): rand_distribution(low, high), rand_engine((std::random_device())()) {}
    int operator()() {
        return rand_distribution(rand_engine);
    }
    void seed(int s) { rand_engine.seed(s); }
private:
    std::default_random_engine rand_engine;
    std::uniform_int_distribution<> rand_distribution;
};


#endif //CHIP8_RAND_H
