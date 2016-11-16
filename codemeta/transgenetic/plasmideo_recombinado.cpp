//
// Created by jeansilva on 07/06/16.
//

#include "plasmideo_recombinado.h"
#include "mersennetwister.h"
#include "PCVPQ.h"


plasmideo_recombinado::plasmideo_recombinado() {}

plasmideo_recombinado::plasmideo_recombinado(vector<endossimbionte> informacaoGenetica) {
    this->informacaoGenetica = informacaoGenetica;
    obterInformacaoGenetica();
}

endossimbionte mesclar(endossimbionte a, endossimbionte b) {

    int n = PCVPQ::getInstance()->getN();
    int inPath[n];
    FOR(i, n) inPath[i] = 0;

    FOR(i, a.size()) {
        inPath[a.getRotaI(i)] = 1;
    }

    FOR(i, b.size()) {
        if (inPath[b.getRotaI(i)] == 0) {
            a.addGene(b.getRotaI(i), b.getCarregamentoI(i));
        }
    }

    return a;
}

vector<endossimbionte> plasmideo_recombinado::obterInformacaoGenetica() {
    // gerar trechos de código aleatórios:
    endossimbionte aux;
    vector<endossimbionte> newInfo;

    FOR(i, informacaoGenetica.size()) {
        aux = aux.getTrechoRandom();
        aux = mesclar(informacaoGenetica[i], aux);
        newInfo.pb(aux);
    }
    informacaoGenetica = newInfo;
    return informacaoGenetica;
}



