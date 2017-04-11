//
// Created by jeansilva on 11/08/16.
//

#ifndef TSPRQ_MERSENNETWISTER_H
#define TSPRQ_MERSENNETWISTER_H

#include "macros.h"

/**
 * Use
 * MersenneTwister::getInstance()->getRand(a, b);
 * */

class MersenneTwister {
private:
    static MersenneTwister *instance;
    MersenneTwister();
    static void init();
public:
    static MersenneTwister * getInstance();
    unsigned int getRand(unsigned int a, unsigned int b);
};


#endif //TSPRQ_MERSENNETWISTER_H
