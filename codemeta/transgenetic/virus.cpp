//
// Created by jeansilva on 05/06/16.
//

#include "virus.h"
#include "mersennetwister.h"

virus::virus() { }

virus::virus(vector<endossimbionte> informacaoGenetica) {
    this->informacaoGenetica = informacaoGenetica;
}

bool virus::transcrever(endossimbionte &e) {
    plasmideo::transcrever(e);
    bloquear(mersennetwister::getInstance()->getRand(1, 10), e);
}

void virus::bloquear(int iteracoes, endossimbionte &e) {
    e.bloquear(iteracoes);
}
