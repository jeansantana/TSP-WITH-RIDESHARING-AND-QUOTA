//
// Created by jeansilva on 05/06/16.
//

#ifndef TRANSGENETIC_VIRUS_H
#define TRANSGENETIC_VIRUS_H


#include "plasmideo.h"

class virus: public plasmideo {

public:
    virus();
    virus(vector<endossimbionte> informacaoGenetica);
    virtual bool transcrever(endossimbionte &e);
    void bloquear(int iteracoes, endossimbionte &e);
};


#endif //TRANSGENETIC_VIRUS_H
