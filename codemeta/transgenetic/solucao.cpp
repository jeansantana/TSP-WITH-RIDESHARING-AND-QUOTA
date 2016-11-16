//
// Created by jeansilva on 07/06/16.
//

#include "solucao.h"


solucao::solucao() {

}

solucao::solucao(double custo, ui quota) {
    this->custo = custo;
    this->quota = quota;
}

double solucao::getCusto() {
    return custo;
}

ui solucao::getQuota() {
    return quota;
}

void solucao::setCusto(double custo) {
    this->custo = custo;
}

void solucao::setQuota(ui quota) {
    this->quota = quota;
}

string solucao::toString() {
    return "(" + to_string(custo) + ", " + to_string(quota) + ")";
}

bool solucao::operator<(solucao o) {
    return (custo < o.getCusto()) || (custo == o.getCusto() && quota > o.getQuota());
}

