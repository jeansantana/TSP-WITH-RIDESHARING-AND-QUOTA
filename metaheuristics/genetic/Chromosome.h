//
// Created by jeansilva on 03/08/16.
//

#ifndef TSPRQ_CHROMOSOME_H
#define TSPRQ_CHROMOSOME_H

#include "../common/macros.h"
#include "../model/Solution.h"

class Chromosome {
private:
    vi route;
    vi boarding;
    int idxProblem;
public:

    explicit Chromosome(vi route, vi boarding);
    Chromosome();

    const vi &getRoute() const;

    void setRoute(const vi &route);

    const vi &getBoarding() const;

    void setBoarding(const vi &boarding);

    Solution fitness();

    void repair();

    Chromosome reproduce(Chromosome other);

    void mutation(double a_lim);
    
    int size();

    // creates a random chromosome, there is no garantee that represents a valid solution
    Chromosome generateChromosome(int size);

    string toString();
};


#endif
