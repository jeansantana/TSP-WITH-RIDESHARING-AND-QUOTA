//
// Created by jeansilva on 08/06/16.
//

#include "transgenetic.h"
#include "virus.h"
#include "plasmideo_recombinado.h"
#include "transposon.h"
#include "mersennetwister.h"
#include "PCVPQ.h"

mersennetwister* ms = mersennetwister::getInstance();
PCVPQ* pcvpq = PCVPQ::getInstance();

void transgenetic::iniciarPopulacao() {
    endossimbionte aux;
    FOR(i, this->tamPop) {
        aux = aux.getRandom();
        this->pop.pb(aux);
    }
}

void transgenetic::obterInformacaoGenetica() {
    endossimbionte aux;
    FOR(i, this->tamPop) {
        this->informacaoGenetica.pb(aux.getTrechoRandom());
    }
}

void transgenetic::criarVetoresTransgeneticos() {

    virus *vrs = new virus(informacaoGenetica);
    this->VT.pb(vrs);
    plasmideo * plmd = new plasmideo(informacaoGenetica);
    this->VT.pb(plmd);
    plasmideo_recombinado * plmdr = new plasmideo_recombinado(informacaoGenetica);
    this->VT.pb(plmdr);
    transposon * transp = new transposon();
    this->VT.pb(transp);

}

//seleciona um individuo, via roleta. Retorna a posição dele na população
int selecaoRoleta(vector<endossimbionte> pop) {

    double totalFitness = 0;
    FOR( i, pop.size() ) {
        // como estamos trabalhando com problema de minimização, trabalhabalha-se com o iverso da fitiness
        totalFitness+= 1.0 / pop[i].avaliacao().getCusto();
    }
    //sorteando uma porcentagem
    double prob = ms->getRand(0, MAX_VALUE_PROB)/1000000.0;//aumenta a precisão
    double sum = 0;
    FOR ( i, pop.size() ) {
        sum+= ( 1.0 / pop[i].avaliacao().getCusto() ) / totalFitness;
        // Verficando os intervalos
        if ( prob <= sum ) {
            /*cout << "Returned: ";
            cout << i << " of " << pop.size() << endl;*/
            return i;
        }
    }
    //cout << "FAILED";
    //cout << sum << " " << prob;
    return ms->getRand(0, pop.size() - 1);
}

//roleta
vector<endossimbionte> transgenetic::selecao() {
    vector<endossimbionte> subPop;

    FOR(i, this->numSelecionados) {
        int idx = selecaoRoleta(pop);
        subPop.pb(pop[idx]);
    }

    return subPop;
}

void transgenetic::manipularCromossomos(vector<endossimbionte> subPop) {

    FOR(i, subPop.size()) {
        int idx = ms->getRand(0, VT.size() - 1);
        endossimbionte e = subPop[i];
        VT[idx]->transcrever(e);
        pop.pb(e);
    }
}

// refazer
void transgenetic::atualizarPopulacao() {
    vector<endossimbionte> subPop;

    while (subPop.size() != tamPop) {
        subPop.pb(pop[ selecaoRoleta(pop) ]);
    }

    pop = subPop;
}
// TODO
void transgenetic::atualizaInformacaoGenetica() {

}

transgenetic::transgenetic(int tamPop, int numSelecionados, int numAvaliacoes) {
    this->tamPop = tamPop;
    this->numSelecionados = numSelecionados;
    this->numAvaliacoes = numAvaliacoes;
}

endossimbionte transgenetic::getMelhor() {
    endossimbionte eMelhor = pop[0];
    solucao sMelhor = eMelhor.avaliacao();

    FORR(i, 1, pop.size()) {
        solucao atual = pop[i].avaliacao();
        if (atual < sMelhor) {
            sMelhor = atual;
            eMelhor = pop[i];
        }
    }
    return eMelhor;
}

endossimbionte transgenetic::run() {

    pcvpq->getInstance()->setNumAvaliacoes(0);
    pcvpq->getInstance()->setIteracao(1);

    iniciarPopulacao();
    obterInformacaoGenetica();

    pcvpq->getInstance()->setNumAvaliacoes(0);
    while (pcvpq->getNumAvaliacoes() <= this->numAvaliacoes) {
        criarVetoresTransgeneticos();
        vector<endossimbionte> subPop = selecao();
        //cout << "ANTES: " << pop.size() << endl;
        manipularCromossomos(subPop);
        //cout << "DEPOIS: " << pop.size() << endl;
        atualizarPopulacao();
        atualizaInformacaoGenetica();
    }
    //condicao de parada: número de iteracoes sem melhora? Ou numero de avaliacoes
    return getMelhor();
}
