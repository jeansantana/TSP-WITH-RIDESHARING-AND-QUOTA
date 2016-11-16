//
// Created by jeansilva on 15/08/16.
//

#ifndef TSPRQ_GENETICALGORITHME_H
#define TSPRQ_GENETICALGORITHME_H

#include "../common/macros.h"
#include "Chromosome.h"

class GeneticAlgorithm {

private:
    vector<Chromosome> population;
    // Fixed population size
    int populationSize;
    // for mutation
    double admissibilityLimit;
    // max number of evaluations
    int maxEvaluations;
    // to calculate the number of recombination
    double recombinationRate;

public:
    GeneticAlgorithm();

    GeneticAlgorithm(int populationSize, double admissibilityLimit, int maxEvaluations, double recombinationRate);

    GeneticAlgorithm(const vector<Chromosome> &population, int populationSize, double admissibilityLimit,
                     int maxEvaluations, double recombinationRate);

    const vector<Chromosome> &getPopulation() const;

    void setPopulation(const vector<Chromosome> &population);

    int getPopulationSize() const;

    void setPopulationSize(int populationSize);

    double getAdmissibilityLimit() const;

    void setAdmissibilityLimit(double admissibilityLimit);

    int getMaxEvaluations() const;

    void setMaxEvaluations(int maxEvaluations);

    double getRecombinationRate() const;

    void setRecombinationRate(double numRecombinations);

    int getEvaluations() const;

    void setEvaluations(int evaluations);

    int getCurrentPopulationSize();

    int getNumRecombinations();
    // starting population
    void initPopulation();
    // select nInds Chromossomes
    vector<Chromosome> selection(int nInds);
    // create next generation
    void createNextGeneration();
    // stop criteria
    bool stopCriteria();

    void recombination();

    pair<Solution, Chromosome> run();

    int roulletSelectOne(vd roullet);

    pair<Solution, Chromosome> getBestSolutionOfPopulation();

};


#endif //TSPRQ_GENETICALGORITHME_H
