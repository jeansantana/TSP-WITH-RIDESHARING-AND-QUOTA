//
// Created by jeansilva on 15/08/16.
//

#include "GeneticAlgorithm.h"
#include "../common/MersenneTwister.h"
#include "../common/Shared.h"
#include "../../commons/LKHParser.h"



GeneticAlgorithm::GeneticAlgorithm() {

}


GeneticAlgorithm::GeneticAlgorithm(int populationSize, double admissibilityLimit, int maxEvaluations,
                                   double recombinationRate) : populationSize(populationSize),
                                                               admissibilityLimit(admissibilityLimit),
                                                               maxEvaluations(maxEvaluations),
                                                               recombinationRate(recombinationRate) {}

GeneticAlgorithm::GeneticAlgorithm(const vector<Chromosome> &population, int populationSize, double admissibilityLimit,
                                   int maxEvaluations, double recombinationRate) : population(population),
                                                                                   populationSize(populationSize),
                                                                                   admissibilityLimit(
                                                                                           admissibilityLimit),
                                                                                   maxEvaluations(maxEvaluations),
                                                                                   recombinationRate(
                                                                                           recombinationRate) {}

const vector<Chromosome> &GeneticAlgorithm::getPopulation() const {
    return population;
}

void GeneticAlgorithm::setPopulation(const vector<Chromosome> &population) {
    GeneticAlgorithm::population = population;
}

int GeneticAlgorithm::getPopulationSize() const {
    return populationSize;
}

void GeneticAlgorithm::setPopulationSize(int populationSize) {
    GeneticAlgorithm::populationSize = populationSize;
}

double GeneticAlgorithm::getAdmissibilityLimit() const {
    return admissibilityLimit;
}

void GeneticAlgorithm::setAdmissibilityLimit(double admissibilityLimit) {
    GeneticAlgorithm::admissibilityLimit = admissibilityLimit;
}

int GeneticAlgorithm::getMaxEvaluations() const {
    return maxEvaluations;
}

void GeneticAlgorithm::setMaxEvaluations(int maxEvaluations) {
    GeneticAlgorithm::maxEvaluations = maxEvaluations;
}

double GeneticAlgorithm::getRecombinationRate() const {
    return recombinationRate;
}

void GeneticAlgorithm::setRecombinationRate(double numRecombinations) {
    GeneticAlgorithm::recombinationRate = numRecombinations;
}

int GeneticAlgorithm::getNumRecombinations() {
    int recombinations = ceil((this->populationSize * this->recombinationRate) / 2.0);
    return recombinations;
}


void GeneticAlgorithm::initPopulation() {

    MersenneTwister * mt = MersenneTwister::getInstance();
    Shared * shared = Shared::getInstance();
    int n = shared->getN();

    FOR(i, populationSize) {
        Chromosome chrome = Chromosome();
        chrome = chrome.generateChromosome(mt->getRand(1, n - 1));
        chrome.repair();
        population.pb(chrome);
    }
}

template <typename T>
inline T const binarySearch(T const guess, vector<T> const list) {

    int begin = 0, end = list.size() - 1, mid = (begin + end) / 2;

    while ( !(mid == end || mid == begin) ) {
        if (list[mid] == guess) {
            begin = end = mid;
        } else if (guess > list[mid]) {
            begin = mid;
        } else {
            end = mid;
        }
        mid = (begin + end) / 2;
    }

    if (guess > list[mid] ) return end;
    else if (guess < list[mid]) return begin;
    else return mid;

}

int GeneticAlgorithm::roulletSelectOne(vd roullet) {
    // double totalSum = roullet.back();

    double guess = (double) MersenneTwister::getInstance()->getRand(1, MAX_VALUE_PROB)/MAX_VALUE_PROB;

    return binarySearch(guess, roullet);

}
// roulette
vector<Chromosome> GeneticAlgorithm::selection(int nInds) {

    vector<Chromosome> selects;

    // Total sum of individuals' fitness of the population
    double totalSum = 0.0;
    FOR(i, population.size()) {
        totalSum+= 1.0 / population[i].fitness().getCost();
    }

    // sum vector of fitness values of population
    vd sum;
    double fit = 1.0 / population[0].fitness().getCost();
    sum.pb(fit/totalSum);

    FORR(i, 1, population.size()) {
        fit = 1.0 / population[i].fitness().getCost() + sum[i - 1];
        sum.pb(fit/totalSum);
    }

    FOR(i, nInds) {
        selects.pb(population[roulletSelectOne(sum)]);
    }

    return selects;

}

void GeneticAlgorithm::createNextGeneration() {
    // killing
    population = selection(this->populationSize);
}

bool GeneticAlgorithm::stopCriteria() {
    Shared * shared = Shared::getInstance();
    return shared->getEvaluations() >= maxEvaluations;
}

int GeneticAlgorithm::getCurrentPopulationSize() {
    return population.size();
}

/**
 * Recombine Cromosomes and put their offspring on population
 * */
void GeneticAlgorithm::recombination() {

    Shared *shared = Shared::getInstance();

    //MersenneTwister * mtr = MersenneTwister::getInstance();

    Chromosome parent1, parent2, offspring1, offspring2;

    // parent's selection by roulette wheel
    vector<Chromosome> parents = selection(2);
    parent1 = parents[0];
    parent2 = parents[1];

    /*cout << "\n\nCHOSEN PARENTS: \n";
    cout << parent1.toString() << endl;
    cout << parent2.toString() << endl;*/

    offspring1 = parent1.reproduce(parent2);

    offspring1.mutation(admissibilityLimit);
    offspring1.repair();

    if (shared->getAlgorithmType() == AlgorithmType::MEMETIC) {
        // Memetic improvment function applied to the offspring before store it in the population
        localSearch(offspring1);
    }

    population.pb(offspring1);

    if (shared->getGeneticOpearator() == GeneticOperator::RECOMBINATION) {

        offspring2 = parent2.reproduce(parent1);
        offspring2.mutation(admissibilityLimit);
        offspring2.repair();

        if (shared->getAlgorithmType() == AlgorithmType::MEMETIC) {
            // Memetic improvment function applied to the offspring before store it in the population
            localSearch(offspring1);

        }

        population.pb(offspring2);

    }

}

pair<Solution, Chromosome> GeneticAlgorithm::getBestSolutionOfPopulation() {

    Solution sbest = population[0].fitness();
    Chromosome cbest = population[0];

    FORR (i, 1, population.size()) {
        Solution current = population[i].fitness();
        if (current < sbest) {
            sbest = current;
            cbest = population[i];
        }
    }

    /*cout << "POPULATION\n";
    for (int i = 0; i < population.size(); ++i) {
        cout << population[i].fitness().getCost() << endl;
    }*/

    return pair<Solution, Chromosome>(sbest, cbest);

}

pair<Solution, Chromosome> GeneticAlgorithm::run() {
    // TODO: add a number of runs here to control the runs
    // TODO: create folder and store the runs and a final file with the informations
    // initializing the initial population
    initPopulation();

    int numRecombinations = getNumRecombinations();
    //cout << population.size() << endl;

    while (!stopCriteria()) {
        FOR (i, numRecombinations) {
            // that includes the mutation
            recombination();
        }
        // selection: create next generation
        createNextGeneration();
    }

    return getBestSolutionOfPopulation();
}

void GeneticAlgorithm::localSearch(Chromosome &chromosome) {
    Shared *shared = Shared::getInstance();
    if (shared->getInstanceType() == InstanceType::ASSIMETRIC) {
        // cout << "Assimetric using 2-opt\n";
        // cout << "===============\nBEFORE: " << chromosome.fitness().getCost() << endl;
        localSearch2Opt(chromosome);
        // cout << "AFTER: " << chromosome.fitness().getCost() << "\n=================\n\n";
    } else {
        // cout << "Simetric using LKH\n";
        linKernighan(chromosome);
    }
}

void GeneticAlgorithm::linKernighan(Chromosome &chromosome) {
    string LKH_PATH = "../commons/LKH-2.0.7/";
    string LK_FILES_PATH = "../commons/LK_FILES/";

    Shared *shared = Shared::getInstance();
    unsigned int time = chrono::system_clock::now().time_since_epoch().count();
    string fileName = "metaheustic_" + to_string(getpid()) + "_" + to_string(time);
    // cout << fileName << endl;
    LKHParser parser(LKH_PATH, LK_FILES_PATH, chromosome.getRoute(), shared->getCosts(), fileName, string("TSP"));
    vi path = parser.LKHSolution();
    chromosome.setRoute(path);

    chromosome.repair();
}

void GeneticAlgorithm::localSearch2Opt(Chromosome &chromosome) {
    for (int i = 1; i < chromosome.getRoute().size(); ++i) {
        for (int k = i + 1; k < chromosome.getRoute().size(); ++k) {
            Chromosome candidate = twoOpt(chromosome, i, k);
            if (candidate.fitness() < chromosome.fitness()) {
                chromosome = candidate;
            }
        }
    }
}

Chromosome GeneticAlgorithm::twoOpt(Chromosome chromosome, int i, int k) {
    vector<int> route = chromosome.getRoute();
    vector<int> boarding = chromosome.getBoarding();
    reverse(route.begin() + i, route.begin() + k + 1);
    reverse(boarding.begin() + i, boarding.begin() + k + 1);

    chromosome.setRoute(route);
    chromosome.setBoarding(boarding);

    chromosome.repair();
    return chromosome;
}
