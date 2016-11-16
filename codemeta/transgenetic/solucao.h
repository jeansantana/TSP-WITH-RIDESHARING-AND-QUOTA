//
// Created by jeansilva on 07/06/16.
//

#ifndef TRANSGENETIC_SOLUCAO_H
#define TRANSGENETIC_SOLUCAO_H


#include "macros.h"

class solucao {

private:
    double custo;
    ui quota;
public:
    solucao();
    solucao(double custo, ui quota);

    double getCusto();
    ui getQuota();
    void setCusto(double custo);
    void setQuota(ui quota);
    string toString();
    bool operator <(solucao o);
};


#endif //TRANSGENETIC_SOLUCAO_H
