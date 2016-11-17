//
// Created by jeansilva on 05/08/16.
//

#ifndef TSPRQ_SHARED_H
#define TSPRQ_SHARED_H

#include "macros.h"

class Shared {

private:
    Shared();

private:
    static Shared * instance;
    // number of cities
    int N;
    // cost matrix. It contains each traveling cost between the cities involved
    vvd costs;
    // passenger budget
    vd passengerBudget;
    // Departure city (position) and arrival (boarding[position]) of the passenger
    vi boarding;//Vértice de embarque (posição) e desembarque (conteúdo)
    // Number of passengers
    int numPassengers;
    // Vehicle capacity
    int vehicleCapacity;
    // Minimum quota
    int minimumQuota;
    // city bonuses
    vi bonuses;
    // number of evaluations
    int evaluations;
    // Implies which algorithm is running
    AlgorithmType algorithmType;
    // Implies which crossover operation have to be used
    Operator geneticOpearator;
public:
    Operator getGeneticOpearator() const;

    void setGeneticOpearator(Operator geneticOpearator);

public:

    static Shared * getInstance();

    int getN() const;

    void setN(int N);

    const vvd &getCosts() const;

    void setCosts(const vvd &costs);

    const vd &getPassengerBudget() const;

    void setPassengerBudget(const vd &passengerBudget);

    const vi &getBoarding() const;

    void setBoarding(const vi &boarding);

    int getNumPassengers() const;

    void setNumPassengers(int numPassengers);

    int getVehicleCapacity() const;

    void setVehicleCapacity(int vehicleCapacity);

    int getMinimumQuota() const;

    void setMinimumQuota(int quota);

    const vi &getBonuses() const;

    void setBonuses(const vi &bonuses);

    int getEvaluations() const;

    void setEvaluations(int evaluations);

    void incrementEvaluations();

    string toString();

    AlgorithmType getAlgorithmType() const;

    void setAlgorithmType(AlgorithmType algorithmType);

};


#endif //TSPRQ_SHARED_H
