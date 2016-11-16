//
// Created by jeansilva on 02/06/16.
//

#include "plasmideo.h"
#include "mersennetwister.h"
#include "PCVPQ.h"

plasmideo::plasmideo() { }

plasmideo::plasmideo(vector<endossimbionte> informacaoGenetica) {
    this->informacaoGenetica = informacaoGenetica;
}

plasmideo::~plasmideo() {

}

vector<endossimbionte> plasmideo::getInformacaoGenetica() {
    return this->informacaoGenetica;
}

void plasmideo::setInformacaoGenetica(vector<endossimbionte> informacaoGenetica) {
    this->informacaoGenetica = informacaoGenetica;
}

/*plasmideo::plasmideo(endossimbionte e) {
    this->e = e;
}*/

endossimbionte plasmideo::injetarCodigoGenetico(endossimbionte e) {
    endossimbionte res;
    int n = PCVPQ::getInstance()->getN();
    int ponto = 1;
    if ( e.size() > 2 )
        ponto = mersennetwister::getInstance()->getRand(1, e.size() - 2);
    int inPath[n];

    FOR(i, n) {
        inPath[i] = 0;
    }
    inPath[0] = 1;
    vi rota, carregamento;
    rota.pb(e.getRotaI(0));
    carregamento.pb(e.getCarregamentoI(0));

    // get trecho de código genético do repo de informações genéticas
    int idxInfo = mersennetwister::getInstance()->getRand(0, informacaoGenetica.size() - 1);
    endossimbionte trecho = informacaoGenetica[idxInfo];

    /*FOR(i, trecho.size())  {
        inPath[trecho.getRotaI(i)] = 1;
    }*/

    FOR(i, ponto) {
        if (inPath[e.getRotaI(i)] == 0) {
            rota.pb(e.getRotaI(i));
            carregamento.pb(e.getCarregamentoI(i));
            inPath[e.getRotaI(i)] = 1;
        }
    }

    //injeta trecho no código genético do endossimbionte no intervalo (ponto, fim)
    FOR(i, trecho.size()) {
        if (inPath[trecho.getRotaI(i)] == 0) {
            rota.pb(trecho.getRotaI(i));
            carregamento.pb(trecho.getCarregamentoI(i));
            inPath[trecho.getRotaI(i)] = 1;
        }
    }

    FORR(i, ponto, e.size()) {
        if (inPath[e.getRotaI(i)] == 0) {
            rota.pb(e.getRotaI(i));
            carregamento.pb(e.getCarregamentoI(i));
            inPath[e.getRotaI(i)] = 1;
        }
    }

    res.setRota(rota);
    res.setCarregamento(carregamento);

    res.reparar();

    return res;
}

bool plasmideo::ataque(const endossimbionte e) {
    //simulação
    _simulacao = injetarCodigoGenetico(e);
    if (_simulacao < e) {
        return true;
    }

    return false;
}

bool plasmideo::transcrever(endossimbionte &e) {
    this->ataque(e);
    if (true) {
        e.setRota(_simulacao.getRota());
        e.setCarregamento(_simulacao.getCarregamento());
    }
    return true;
}

vector<endossimbionte> plasmideo::obterInformacaoGenetica() {
    return informacaoGenetica;
}

