//
// Created by jeansilva on 08/06/16.
//

#ifndef TRANSGENETIC_TRANSGENETIC_H
#define TRANSGENETIC_TRANSGENETIC_H


#include "vetor_transgenetico.h"

class transgenetic {
private:
    //vetor transgenetico
    vector<vetor_transgenetico*> VT;
    int tamPop;
    int numSelecionados;
    vector<endossimbionte> pop;
    vector<endossimbionte> informacaoGenetica;
    int numAvaliacoes;

    void iniciarPopulacao();
    void obterInformacaoGenetica();
    void criarVetoresTransgeneticos();
    vector<endossimbionte> selecao();
    void manipularCromossomos(vector<endossimbionte> subPop);
    void atualizarPopulacao();
    void atualizaInformacaoGenetica();
    endossimbionte getMelhor();

public:
    transgenetic(int tamPop, int numSelecionados, int numAvaliacoes);
    endossimbionte run();
};


#endif //TRANSGENETIC_TRANSGENETIC_H
