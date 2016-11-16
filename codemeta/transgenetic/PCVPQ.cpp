//
// Created by jeansilva on 06/06/16.
//

#include "PCVPQ.h"

PCVPQ * PCVPQ::instance = NULL;

PCVPQ::PCVPQ() {

}

PCVPQ* PCVPQ::getInstance() {
    if (!instance) {
        instance = new PCVPQ;
    }
    return instance;
}

int PCVPQ::getIteracao() const {
    return this->iteracao;
}

void PCVPQ::setIteracao(int iteracao) {
    this->iteracao = iteracao;
}

int PCVPQ::incrementeIteracao() {
    this->iteracao++;
    return this->iteracao;
}

int PCVPQ::getN() const{
    return  this->n;
}

void PCVPQ::setN(int n) {
    this->n = n;
}

void PCVPQ::setM(int M[MAX_N][MAX_N]){
    for (int i = 0; i < n; ++i) {
        for (int j = 0; j < n; ++j) {
            costs[i][j] = M[i][j];
        }
    }
}

double PCVPQ::M(int i, int j) const {
    assert(i >= 0 && i < this->n && j >= 0 && j < this->n);
    return this->costs[i][j];
}

void PCVPQ::M(int i, int j, double value) {
    assert(i >= 0 && i < this->n && j >= 0 && j < this->n);
    this->costs[i][j] = value;
}

int PCVPQ::embarque(int i) {
    return _embarque[i];
}

int PCVPQ::tarifa(int i) {
    return _tarifa[i];
}

int PCVPQ::bonus(int i) {
    return _bonus[i];
}

int PCVPQ::capacidade() const {
    return this->_capacidade;
}

int PCVPQ::quota() {
    return _quota;
}

int PCVPQ::numPassageiros() {
    return _numPassageiros;
}


void PCVPQ::setEmbarqueI(int i, int value) {
    _embarque[i] = value;
}

void PCVPQ::setTarifaI(int i, int value) {
    _tarifa[i] = value;
}

void PCVPQ::setBonusI(int i, int value) {
    _bonus[i] = value;
}

void PCVPQ::setCapacidade(int _capacidade) {
    this->_capacidade = _capacidade;
}

void PCVPQ::setQuota(int _quota) {
    this->_quota = _quota;
}

void PCVPQ::setNumPassageiros(int _numPassageiros) {
    this->_numPassageiros = _numPassageiros;
}

int PCVPQ::getNumAvaliacoes() {
    return avaliacoes;
}

void PCVPQ::setNumAvaliacoes(int avaliacoes) {
    this->avaliacoes = avaliacoes;
}

void PCVPQ::incrementaAvaliacoes() {
    this->avaliacoes++;
}

string PCVPQ::toSting() {
    string res = "número de vértices, número de passageiros e capacidade do carro:\n";
    res+= to_string(n) + " " + to_string(_numPassageiros) + " " + to_string(_capacidade) + "\nMatriz de distâncias:\n";
    FOR(i, n) {
        FOR(j, n) {
            res+= to_string(costs[i][j]) + " ";
        }
        res+= "\n";
    }
    res+="Tarifas e itinerário dos passageiros:\n";
    FOR(i, _numPassageiros) {
        res+=to_string(_tarifa[i]) + " " + to_string(i) + " " + to_string(_embarque[i]) + "\n";
    }
    res+="Quota e bônus por cidade:\n";
    res+= to_string(_quota) + "\n";
    FOR(i, n) {
        res+= to_string(i) + " " + to_string(_bonus[i]) + "\n";
    }
    return res;
}


