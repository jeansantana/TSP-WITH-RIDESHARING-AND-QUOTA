//
// Created by jeansilva on 06/06/16.
//

#include "mersennetwister.h"

mersennetwister * mersennetwister::instance = NULL;
unsigned seed;
mt generator;

mersennetwister::mersennetwister() {

}

mersennetwister *mersennetwister::getInstance() {
    if (!instance) {
        instance = new mersennetwister;
        init();
    }
    return instance;
}

void mersennetwister::init() {
    // obtain a seed from the system clock:
    seed = chrono::system_clock::now().time_since_epoch().count();
    generator.seed(seed);
}
// a - limite inferior, b - limite superior
unsigned int mersennetwister::getRand(unsigned int a, unsigned int b) {
    //unsigned int value_rand = a + generator() % b;
    uniform_int_distribution<int> distribution(a, b);
    //cout << "Random value: " <<  value_rand << endl;
    unsigned int value_rand = distribution(generator);
    assert(value_rand >= a && value_rand <= b);
    return value_rand;
}

