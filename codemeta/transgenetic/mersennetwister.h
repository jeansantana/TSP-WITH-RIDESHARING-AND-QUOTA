//
// Created by jeansilva on 06/06/16.
//

#ifndef TRANSGENETIC_RANDOM_H
#define TRANSGENETIC_RANDOM_H


#include "macros.h"

class mersennetwister {

private:

    /*static unsigned seed;
    static mt generator;*/

    static mersennetwister *instance;
    mersennetwister();
    static void init();
public:
    static mersennetwister * getInstance();
    unsigned int getRand(unsigned int a, unsigned int b);

};


#endif //TRANSGENETIC_RANDOM_H
