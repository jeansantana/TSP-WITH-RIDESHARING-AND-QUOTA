//
// Created by jeansilva on 07/06/16.
//

#include "transposon.h"
#include "mersennetwister.h"
#include "PCVPQ.h"

//fazer 2-opt

//1 2 3 4 5 6 7
//(2, 6)
//1 [6 5 4 3 2] 7

transposon::transposon() {
    int n = PCVPQ::getInstance()->getN();
    int a = mersennetwister::getInstance()->getRand(1, n);
    int b = a;
    while (a == b) {
        b = mersennetwister::getInstance()->getRand(1, n);
    }

    if (b > a) swap(a, b);
    identificacao.first = a;
    identificacao.first = b;

}

transposon::transposon(ii identificacao) {
    assert(identificacao.second > identificacao.first);
    this->identificacao = identificacao;
}

endossimbionte doisOpt(endossimbionte e, int pos, int k) {
    vi rota, carregamento;
    // (0, pos - 1)
    FOR(i, pos) {
        rota.pb(e.getRotaI(i));
        carregamento.pb(e.getCarregamentoI(i));
    }
    //reverse
    FORE(i, k, pos) {
        rota.pb(e.getRotaI(i));
        carregamento.pb(e.getCarregamentoI(i));
    }
    
    // add restante da rota
    FORR(i, k + 1, e.size()) {
        rota.pb(e.getRotaI(i));
        carregamento.pb(e.getCarregamentoI(i));
    }
    endossimbionte endo(rota, carregamento);
    endo.reparar();
    return endo;
}

endossimbionte transposon::melhorVizinho(endossimbionte e) {
    endossimbionte melhor = e;
    FORR(i, identificacao.first, identificacao.second) {
        FORR(j, i + 1, identificacao.second + 1) {
            endossimbionte atual = doisOpt(melhor, i, j);
            if (atual < melhor) {
                melhor = atual;
            }
        }
    }
    return melhor;
}

endossimbionte transposon::buscaLocal(endossimbionte e) {
    endossimbionte melhor = e;
    bool continua = true;
    while(continua) {
        endossimbionte atual = melhorVizinho(melhor);
        if (atual < melhor) {
            melhor = atual;
        } else {
            continua = false;
        }
    }
    return melhor;
}

bool transposon::ataque(const endossimbionte e) {
    _simulacao = buscaLocal(e);
    return true;
}

bool transposon::transcrever(endossimbionte &e) {
    if (ataque(e)) {
        e.setRota(_simulacao.getRota());
        e.setCarregamento(_simulacao.getCarregamento());
    }
    return true;
}

ii transposon::getIdentificacao() {
    return identificacao;
}

void transposon::setIdenficacao(ii identificacao) {
    assert(identificacao.second > identificacao.first);
    this->identificacao = identificacao;
}





