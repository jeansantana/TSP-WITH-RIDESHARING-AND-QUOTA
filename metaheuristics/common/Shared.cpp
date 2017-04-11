//
// Created by jeansilva on 05/08/16.
//

#include "Shared.h"

Shared * Shared::instance;

Shared::Shared(): evaluations(0) {

}

Shared *Shared::getInstance() {
    if (!instance) {
        instance = new Shared;
        instance->setEvaluations(0);
    }
    return instance;
}

int Shared::getN() const {
    return N;
}

void Shared::setN(int N) {
    Shared::N = N;
}

const vvd &Shared::getCosts() const {
    return costs;
}

void Shared::setCosts(const vvd &costs) {
    Shared::costs = costs;
}

const vd &Shared::getPassengerBudget() const {
    return passengerBudget;
}

void Shared::setPassengerBudget(const vd &passengerBudget) {
    Shared::passengerBudget = passengerBudget;
}

const vi &Shared::getBoarding() const {
    return boarding;
}

void Shared::setBoarding(const vi &boarding) {
    Shared::boarding = boarding;
}

int Shared::getNumPassengers() const {
    return numPassengers;
}

void Shared::setNumPassengers(int numPassengers) {
    Shared::numPassengers = numPassengers;
}

int Shared::getVehicleCapacity() const {
    return vehicleCapacity;
}

void Shared::setVehicleCapacity(int vehicleCapacity) {
    Shared::vehicleCapacity = vehicleCapacity;
}

int Shared::getMinimumQuota() const {
    return minimumQuota;
}

void Shared::setMinimumQuota(int quota) {
    Shared::minimumQuota = quota;
}

const vi &Shared::getBonuses() const {
    return bonuses;
}

void Shared::setBonuses(const vi &bonuses) {
    Shared::bonuses = bonuses;
}

int Shared::getEvaluations() const {
    return evaluations;
}

void Shared::setEvaluations(int evaluations) {
    Shared::evaluations = evaluations;
}

void Shared::incrementEvaluations() {
    this->evaluations++;
}

string Shared::toString() {
    string str = "";
    str+= to_string(N) + " " + to_string(numPassengers) + " " + to_string(vehicleCapacity) + "\n";
    FOR(i, N) {
        FOR(j, N) {
            str+= to_string(costs[i][j]) + " ";
        }
        str+= "\n";
    }
    FOR(i, N) {
        str+= to_string(passengerBudget[i]) + " " + to_string(i) + " " + to_string(boarding[i]) + "\n";
    }
    str+= to_string(minimumQuota) + "\n";
    FOR(i, N) {
        str+= to_string(i) + " " + to_string(bonuses[i]) + "\n";
    }

    return str;
}

AlgorithmType Shared::getAlgorithmType() const {
    return algorithmType;
}

void Shared::setAlgorithmType(AlgorithmType algorithmType) {
    Shared::algorithmType = algorithmType;
}

GeneticOperator Shared::getGeneticOpearator() const {
    return geneticOpearator;
}

void Shared::setGeneticOpearator(GeneticOperator geneticOpearator) {
    Shared::geneticOpearator = geneticOpearator;
}

InstanceType Shared::getInstanceType() const {
    return instanceType;
}

void Shared::setInstanceType(InstanceType instanceType) {
    Shared::instanceType = instanceType;
}
