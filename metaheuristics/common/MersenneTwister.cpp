//
// Created by jeansilva on 06/06/16.
//

#include "MersenneTwister.h"

MersenneTwister * MersenneTwister::instance = NULL;
unsigned seed;
mt generator;

MersenneTwister::MersenneTwister() {

}

MersenneTwister *MersenneTwister::getInstance() {
    if (!instance) {
        instance = new MersenneTwister;
        init();
    }
    return instance;
}

void MersenneTwister::init() {
    // obtain a seed from the system clock:
    seed = chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
}
// a - inf bound, b - sup bound
unsigned int MersenneTwister::getRand(unsigned int a, unsigned int b) {
    uniform_int_distribution<int> distribution(a, b);
    unsigned int value_rand = distribution(generator);
    assert(value_rand >= a && value_rand <= b);
    return value_rand;
}