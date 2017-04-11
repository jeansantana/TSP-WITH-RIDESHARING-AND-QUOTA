//
// Created by jeansilva on 03/08/16.
//

#include "Solution.h"
#include "../common/Shared.h"

Solution::Solution(double cost, int quota) : cost(cost), quota(quota) {

}

Solution::Solution(double cost, int quota, int problem) : cost(cost), quota(quota), problem(problem) {

}

Solution::Solution(): cost(-1), quota(-1), problem(1) {
}

Solution::Solution(vi route, vi boarding) {
    solve(route, boarding);
    if (!isValid()) {
        this->cost = -1;
        this->quota = -1;
    }
    /*Solution s = solve(route, boarding);
    this->quota = s.getQuota();
    this->cost = s.getCost();*/
}

double Solution::getCost() const {
    return cost;
}

void Solution::setCost(double cost) {
    Solution::cost = cost;
}

int Solution::getQuota() const {
    return quota;
}

void Solution::setQuota(int quota) {
    Solution::quota = quota;
}

bool Solution::cmpTo(Solution other) {
    if (this->getCost() < other.getCost()) {
        return true;
    } else if (this->getCost() == other.getCost()) {
        return this->getQuota() > other.getQuota();
    }
    return false;
}

bool Solution::operator<(const Solution other) {
    return cmpTo(other);
}

bool Solution::operator>(const Solution other) {
    return !cmpTo(other);
}

string Solution::toString() {
    string out = "(" + to_string(this->getCost()) + ", " + to_string(this->getQuota()) + ")";
    return out;
}
/**
 * Return -1 if embarque is OK, otherwise, the city position that we have a problem
 */
int embarqueIsOK(vi route, vi loading) {

    Shared * shared = Shared::getInstance();
    int n = shared->getN();
    vi E = shared->getBoarding();


    int inPath[n];
    FOR(i, n) inPath[i] = 0;

    for (int i = 0; i < route.size(); ++i) {
        // if a passenger are going to embark in the city i and disembark on j, j have to be on route
        if ( route[i] != 0 ) inPath[ route[i] ] = 1;
        // if the city has passed this boarding is not valid
        if ( loading[i] == 1 && inPath[ E[ route[i] ] ] == 1 ) {
            return i;
        }
    }

    // Verifyng if the depot and arrival city of each passenger in loading is on route
    inPath[0] = 1;
    FOR(i, route.size()) {
        if ( loading[i] == 1 ) {
            // if the arrival city of a passenger is not on rout, this solution is invalid
            if ( inPath[ E[ route[i] ] ] == 0 ) {
                return i;
            }
        }
    }

    return -1;
}

bool bonusOk(vi route) {

    Shared * shared = Shared::getInstance();
    vi bonus = shared->getBonuses();
    int minQuota = shared->getMinimumQuota();

    int bonus_coletado = 0;
    FOR(i, route.size()) {
        bonus_coletado+= bonus[ route[i] ];
        if (bonus_coletado >= minQuota) return true;
    }
    return bonus_coletado >= minQuota;
}

bool isValidSolution(vi route, vi loading, int &problem) {
    problem = embarqueIsOK(route, loading);
    bool res = problem == -1 && bonusOk(route);
    return res;
}

Solution Solution::solve(vi route, vi boarding) {

    Shared * shared = Shared::getInstance();
    int n = shared->getN();
    vvd M = shared->getCosts();
    vi bonus = shared->getBonuses();
    vi E = shared->getBoarding();
    int k = shared->getVehicleCapacity();
    vd T = shared->getPassengerBudget();

    shared->incrementEvaluations();

    int bonus_coletado = 0;
    cpp_rational p_cost = 0;
    // Disembark quantity per vertex
    int D[n];
    // if city i has gone
    int gone[n];
    /* Stores how much the drive have to pay on the city i.
     * To know how much a passenger that depart from city i,
     * and arrive in j have to pay: we do:
     * taximetro[j] - taximetro[j] (arrival - departure)*/
    cpp_rational taximetro[n + 1];
    FOR(i, n) {
        D[i] = 0;
        gone[i] = 0;
        taximetro[i] = 0;
    }
    taximetro[n] = 0;
    cpp_rational total_cost = 0;
    int pass_num = 1; // we count the driver
    // Lookin for problem in route with its boarding configuration
    if (!isValidSolution(route, boarding, problem)) {
        this->quota = -1;
        this->cost = -1;
        // if is not a valid solution and problem is -1 (it mean that is valid) the problem is minimum quota was not accomplished
        if (problem == SOLUTION_OK)
            problem = MINIMUM_QUOTA_PROBLEM;
        return Solution(-1, -1, MINIMUM_QUOTA_PROBLEM); // -1, -1
    }

    for (int i = 0; i < route.size(); i++) {
        bonus_coletado+= bonus[ route[i] ];
        // disembarking a passenger
        if (pass_num != 1 && i != 0) {
            pass_num-= D[ route[i] ];
        }
        // if the passenger i are going to embark and his arrival city E[i] hasn't gone, let this passenger in on vehicle
        if (boarding[i] == 1 && gone[ E[ route[i] ] ] == 0) {
            // put this passenger on the vehicle
            if (pass_num < k + 1) {
                pass_num++;
                // Store the moment the passenger has to disembark
                D[ E[ route[i] ] ]++;
            } else {
                problem = i;
                this->quota = -1;
                this->cost = -1;
                return Solution();
            }
        } else if (boarding[i] == 1 && gone[ E[ route[i] ] ] == 1) {
            problem = i;
            this->quota = -1;
            this->cost = -1;
            return Solution();
        }

        cpp_rational parc_cost;
        if (i == route.size() - 1) {
            parc_cost = cpp_rational(M[ route[i] ][0]) / cpp_rational(pass_num);
            p_cost = parc_cost;
        } else {
            parc_cost = cpp_rational(M[ route[i] ][ route[i + 1] ]) / cpp_rational(pass_num);
        }
        taximetro[i + 1] = taximetro[i] + cpp_rational(parc_cost);
        total_cost+= parc_cost;
        if (route[i] != 0) gone[ route[i] ] = 1;
    }

    int preprocessor[n];
    FOR(i, n) preprocessor[i] = 0;
    /* preprocessot to know which route position vector the vertex is. For exemple:
    the route 0 3 4 5; They position are: 0(0) 3(1) 4(2) 5(3) */
    for (int j = 0; j < route.size(); ++j) {
        preprocessor[ route[j] ] = j;
    }

    // budget constraint
    for (int i = 0; i < route.size(); ++i) {

        int origem = preprocessor[route[i]];
        int destino = preprocessor[E[route[i]]];

        if (destino == 0) {
            destino = route.size();
        }

        if (boarding[origem] == 1 && cpp_rational(T[i]) < taximetro[destino] - taximetro[origem]) {
            problem = origem;
            this->quota = -1;
            this->cost = -1;
            return Solution();
        }
    }

    this->cost = (double) numerator(total_cost) / (double) denominator(total_cost);
    this->quota = bonus_coletado;

    return Solution((double) numerator(total_cost) / (double) denominator(total_cost), bonus_coletado, problem);
}

int Solution::getProblem() const {
    return problem;
}

bool Solution::isValid() {
    return this->problem == -1;
}

void Solution::setProblem(int problem) {
    Solution::problem = problem;
}
