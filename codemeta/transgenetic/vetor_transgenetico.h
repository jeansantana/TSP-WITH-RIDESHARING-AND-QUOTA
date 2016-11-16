//
// Created by jeansilva on 02/06/16.
//

#ifndef TRANSGENETIC_VETOR_TRANSGENETICO_H
#define TRANSGENETIC_VETOR_TRANSGENETICO_H

#include "endossimbionte.h"

class vetor_transgenetico {

public:
    vetor_transgenetico();

    virtual bool ataque(const endossimbionte e) = 0;
    virtual bool transcrever(endossimbionte &e) = 0;

};


#endif //TRANSGENETIC_VETOR_TRANSGENETICO_H
