//
// Created by jeansilva on 06/06/16.
//

#ifndef TRANSGENETIC_PCVPQ_H
#define TRANSGENETIC_PCVPQ_H

#include "macros.h"

class PCVPQ {
private:
    double costs[MAX_N][MAX_N];
    double _tarifa[MAX_N];//Tarifa máxima por passageiro
    int _embarque[MAX_N];//Vértice de embarque (posição) e desembarque (conteúdo)
    int n; // número de cidades/vértices
    int _numPassageiros;
    int _capacidade; // capacidade do carro
    int _quota; // quota mínima a ser atendida
    int _bonus[MAX_N];
    int iteracao;
    int avaliacoes;
    static PCVPQ * instance;
    PCVPQ();
public:
    static PCVPQ * getInstance();
    void setN(int n);
    int getN() const;
    double M(int i, int j) const;
    void M(int i, int j, double value);
    void setM(int M[MAX_N][MAX_N]);
    int getIteracao() const ;
    void setIteracao(int iteracao);
    int incrementeIteracao();
    int embarque(int i);
    void setEmbarqueI(int i, int value);
    int tarifa(int i);
    void setTarifaI(int i, int value);
    int bonus(int i);
    void setBonusI(int i, int value);
    int capacidade() const;
    void setCapacidade(int _capacidade);
    int quota();
    void setQuota(int _quota);
    int numPassageiros();
    void setNumPassageiros(int _numPassageiros);
    void incrementaAvaliacoes();
    int getNumAvaliacoes();
    void setNumAvaliacoes(int avaliacoes);
    string toSting();
};


#endif //TRANSGENETIC_PCVPQ_H
