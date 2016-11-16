//
// Created by jeansilva on 02/06/16.
//

#ifndef TRANSGENETIC_ENDOSSIMBIONTE_H
#define TRANSGENETIC_ENDOSSIMBIONTE_H

#include "macros.h"
#include "solucao.h"

class endossimbionte {

private:
    vector<int> rota;
    vector<int> carregamento;

    vector<int> _consertar;

    bool bloqueado;
    int _iteracaoEnd;

public:
    endossimbionte(vector<int> rota, vector<int> carregamento);

    endossimbionte();

    ~endossimbionte();

    vector<int> getRota();

    vector <int> getCarregamento();

    void setRota(vector <int> rota);

    void setCarregamento(vector <int> carregamento);

    void setCarregamentoI(int i, int v);

    void setRotaI(int i, int v);

    int getCarregamentoI(int i);

    int getRotaI(int i);

    bool isBloqueado();

    void bloquear(int iteracoes);

    void setBloquear(bool bloq);

    int size();

    void reparar();

    solucao avaliacao();

    bool operator < (endossimbionte o);

    string toString();

    bool embarqueIsOK();

    void addGene(int cidade, int embarque);

    endossimbionte getTrechoRandom();

    endossimbionte getRandom();

};


#endif //TRANSGENETIC_ENDOSSIMBIONTE_H
