//
// Created by jeansilva on 03/08/16.
//

#include "Chromosome.h"
#include "../common/Shared.h"
#include "../common/MersenneTwister.h"

const vi &Chromosome::getRoute() const {
    return route;
}

void Chromosome::setRoute(const vi &route) {
    Chromosome::route = route;
}

const vi &Chromosome::getBoarding() const {
    return boarding;
}

void Chromosome::setBoarding(const vi &boarding) {
    Chromosome::boarding = boarding;
}

Chromosome::Chromosome(vi route, vi boarding) {
    this->route = route;
    this->boarding = boarding;
    this->idxProblem = -1;
}

Chromosome::Chromosome() {
    this->idxProblem = -1;
}

Solution Chromosome::fitness() {
    Solution s = Solution(route, boarding);
    if (!s.isValid()) {
        repair();
    }
    return s;
}

bool compareBonus(ii a, ii b) {
    return a.second > b.second;
}

void Chromosome::repair() {

    Shared * shared = Shared::getInstance();
    int n = shared->getN();
    vi bonus = shared->getBonuses();
    int minQuota = shared->getMinimumQuota();

    // quota repair
    int quota = 0;
    int inPath[n];
    vii quotas;

    FOR(i, n) {
        inPath[i] = 0;
        quotas.pb(ii(i, bonus[i]));
    }

    FOR(i, size()) {
        quota+= bonus[route[i]];
        inPath[route[i]] = 1;
    }

    sort(quotas.begin(), quotas.end(), compareBonus);

    // repair the bonus
    bool repaired = false;
    if (quota < minQuota) {
        // checking the candidates cities to be inserted in route
        for (int i = 0; i < n && !repaired ; ++i) {
            if ( inPath[ quotas[i].first ] == 0 ) {
                quota+= quotas[i].second;
                route.pb(quotas[i].first);
                boarding.pb(0);
            }
            if (quota >= minQuota) repaired = true;
        }
    }

    Solution s(route, boarding);
    while (!s.isValid()) {
        boarding[s.getProblem()] = 0;
        s.solve(route, boarding);
    }

    //Loading Heuristic

    FOR(i, size()) {
        if (boarding[i] != 1) {
            boarding[i] = 1;
            Solution aux(route, boarding);
            //s.solve(route, boarding);
            if (aux.isValid() && aux < s) {
                s = aux;
            } else {
                boarding[i] = 0;
            }
        }
    }

}

Chromosome Chromosome::splitAndRecombineAddUnvisited(Chromosome other) {

    // cout << "Split and Recombine Add Unvisited\n";

    int breakPoint = MersenneTwister::getInstance()->getRand(1, size() - 2);
    //breakPoint = 3;
    //cout << "breakPoint: " << breakPoint << endl;
    int n = Shared::getInstance()->getN();
    vi _route, _boarding;
    int added[n];
    FOR(i, n) added[i] = 0;

    FOR(i, breakPoint + 1) {
        added[ route[i] ] = 1;
        _route.pb( route[i] );
        _boarding.pb( boarding[i] );
    }

    vi _routeOther = other.getRoute();
    vi _boardingOther = other.getBoarding();

    FOR(i, other.size()) {
        if (added[ _routeOther[i] ] == 0) {
            _route.pb( _routeOther[i] );
            _boarding.pb( _boardingOther[i] );
        }
    }

    return Chromosome(_route, _boarding);

}

/**
 * Returns the unvisited city just after p
 * @param p Last chosen city
 * @param route Route where we have to find a new p candidate
 * @param taken Vector that implies who was taken in the offspring route
 * @return A new p candidate
*/

int Chromosome::getCandidateCity(int p, vi route, vi taken) {

    /*cout << "\n\nGetting the first candidate after " << p << " in:" << endl;

    FOR (i, route.size()) {
        cout << route[i] << " ";
    }
    cout << endl;

    FOR (i, taken.size()) {
        cout << i << ": " << taken[i] << "; ";
    }
    cout << endl;
    */
    Shared *shared = Shared::getInstance();

    int chosen = -1;

    vi::iterator it = find(route.begin(), route.end(), p);

    if (it != route.end()) {

        // cout << "uashausdhau\n";

        for (vi::iterator i = (it + 1); i != route.end(); i++) {
            if (taken[*i] == 0) {
                chosen = *i;
                break;
            }
        }

    }
    // if anyone was chosen we have to search some city that wasn't visited yet
    if (chosen == -1) {
        //cout << "UASHUSHAUSHAUSH\n";
        FOR (j, shared->getN()) {
            if ( taken[j] == 0 && find(route.begin(), route.end(), j) != route.end() ) {
                chosen = j;
                break;
            }
        }
    }

    // cout << "Chosen one: " << chosen << endl;

    return chosen;
}

Chromosome Chromosome::sequentialConstrutiveCrossover(Chromosome other) {

    // cout << "Sequential Construtive Crossover\n";

    /*cout << "\n\nPARENTS: \n";
    cout << this->toString() << endl;
    cout << other.toString() << endl;*/

    Shared *shared = Shared::getInstance();

    vvd costs = shared->getCosts();
    // offspring
    vi _route, _boarding;

    _route.pb(0);
    _boarding.pb(boarding[0]);

    vi _routeOther = other.getRoute();
    vi _boardingOther = other.getBoarding();

    int p = 0;
    vi taken(shared->getN(), 0);
    taken[0] = 1;

    int offspringSize = (size() + other.size()) / 2;

    while (_route.size() != offspringSize) {
        // getting candidates cities after p
        int c1 = getCandidateCity(p, route, taken);
        int c2 = getCandidateCity(p, _routeOther, taken);

        // c1 = c2 and != -1
        if (c1 == -1 || c2 == -1) {
            c1 = c1 * c2 * -1;
            c2 = c1;
        }

        // cout << "P: " << p << " C1: " << c1 << " C2: " << c2 << endl;

        int brdBit = 0; // Boarding indicator for the inserted p
        // next p
        if (costs[p][c1] <= costs[p][c2]) {
            p = c1;
            int idx = find(route.begin(), route.end(), p) - route.begin();
            if (idx < route.size()) {
                brdBit = boarding[idx];
            }
        } else {
            p = c2;
            int idx = find(_routeOther.begin(), _routeOther.end(), p) - _routeOther.begin();
            if (idx < _routeOther.size()) {
                brdBit = _boardingOther[idx];
            }
        }

        // cout << "ESSE É O P: " << p << endl;

        taken[p] = 1;
        _route.pb(p);
        _boarding.pb(brdBit);
    }


    /*cout << "OFFSPRING:\n";
    FOR(i, _route.size()) {
        cout << _route[i] << " ";
    }
    cout << endl;
    FOR(i, _boarding.size()) {
        cout << _boarding[i] << " ";
    }
    cout << endl;*/

    return Chromosome(_route, _boarding);
}

Chromosome Chromosome::sequentialConstrutiveCrossoverBased(Chromosome other) {

    /*cout << "\n\nPARENTS: \n";
    cout << this->toString() << endl;
    cout << other.toString() << endl;*/

    Shared *shared = Shared::getInstance();

    vvd costs = shared->getCosts();
    // offspring
    vi _route, _boarding;

    _route.pb(0);
    _boarding.pb(boarding[0]);

    vi _routeOther = other.getRoute();
    vi _boardingOther = other.getBoarding();

    int p = 0;
    vi taken(shared->getN(), 0);
    taken[0] = 1;

    int offspringSize = (size() + other.size()) / 2;

    // getting candidates cities after p
    int c1 = getCandidateCity(p, route, taken);
    int c2 = getCandidateCity(p, _routeOther, taken);

    // The gene was inhited from parente 1?
    bool inhP1 = false;

    // c1 = c2 and != -1
    if (c1 == -1 || c2 == -1) {
        c1 = c1 * c2 * -1;
        c2 = c1;
    }

    int brdBit = 0; // Boarding indicator for the inserted p
    int idx;
    // next p
    if (costs[p][c1] <= costs[p][c2]) {
        p = c1;
        idx = find(route.begin(), route.end(), p) - route.begin();
        if (idx < route.size()) {
            brdBit = boarding[idx];
            inhP1 = true;
        }
    } else {
        p = c2;
        idx = find(_routeOther.begin(), _routeOther.end(), p) - _routeOther.begin();
        if (idx < _routeOther.size()) {
            brdBit = _boardingOther[idx];
            inhP1 = false;
        }
    }

    taken[p] = 1;
    _route.pb(p);
    _boarding.pb(brdBit);

    // copy from p2

    if (inhP1) {

        // cout << "copy from p2\n";

        FORR (i, idx + 1, _routeOther.size()) {

            int rVal = -1, bVal = 0;

            if (taken[ _routeOther[i] ] == 0) {
                taken[ _routeOther[i] ] = 1;
                rVal = _routeOther[i];
                bVal = _boardingOther[i];
            }
            _route.pb(rVal);
            _boarding.pb(bVal);

        }
    } else { // copy from p1

        // cout << "copy from p1\n";

        FORR (i, idx + 1, route.size()) {

            int rVal = -1, bVal = 0;

            if (taken[ route[i] ] == 0) {
                taken[ route[i] ] = 1;
                rVal = route[i];
                bVal = boarding[i];
            }
            _route.pb(rVal);
            _boarding.pb(bVal);
        }
    }

    queue<int> candidates;
    // preprocessing the canditates list
    FOR (i, shared->getN()) {
        if (taken[i] == 0) {
            candidates.push(i);
        }
    }

    if (!candidates.empty()) {
        FOR (i, _route.size()) {
            if (_route[i] == -1) {
                _route[i] = candidates.front();
                candidates.pop();
            }
        }
    }

    /*cout << "Check debug:\n";
    FOR(i, _route.size()) {
        cout << _route[i] << " ";
    }
    cout << endl;
    FOR(i, _boarding.size()) {
        cout << _boarding[i] << " ";
    }
    cout << endl;*/

    return Chromosome(_route, _boarding);
}

Chromosome Chromosome::reproduce(Chromosome other) {

    Shared *share = Shared::getInstance();
    GeneticOperator op = share->getGeneticOpearator();

    if (op == GeneticOperator::RECOMBINATION) {
        return splitAndRecombineAddUnvisited(other);
    } else if (op == GeneticOperator::SCX_ADP) {
        return sequentialConstrutiveCrossover(other);
    } else if (op == GeneticOperator::SCX_BASED_ADP) {
        return sequentialConstrutiveCrossoverBased(other);
    } else {
        return splitAndRecombineAddUnvisited(other);
    }

}

void Chromosome::mutation(double a_lim) {
    double m_ratio = MersenneTwister::getInstance()->getRand(0, MAX_VALUE_PROB) / MAX_VALUE_PROB;
    if (m_ratio >= a_lim) {
        int idx = MersenneTwister::getInstance()->getRand(0, size() - 1);
        boarding[idx] = (boarding[idx] + 1) % 2;
    }
}

int Chromosome::size() {
    return route.size();
}

string Chromosome::toString() {
    string r = "", b  = "";
    FOR(i, size()) {
        r+= to_string(route[i]) + " ";
        b+= to_string(boarding[i]) + " ";
    }
    return r + "\n" + b + "\n" + fitness().toString();
}

// TODO: change the way to generate chroms, maybe use the shuffle method.
Chromosome Chromosome::generateChromosome(int size) {

    MersenneTwister * mt = MersenneTwister::getInstance();
    Shared * shared = Shared::getInstance();
    int n = shared->getN();

    // to grantee that will not exceed the limit (n)
    //size = size % n;
    if (size > n) {
        size = n;
    }

    vi _route, _boarding;
    int used[n];
    FOR(i, n) used[i] = 0;

    _route.pb(0);
    _boarding.pb(mt->getRand(0, 1));
    used[0] = 1;

    while (_route.size() != size) {
        int city = mt->getRand(1, n - 1);
        if (used[city] == 0) {
            _route.pb(city);
            _boarding.pb(mt->getRand(0, 1));
            used[city] = 1;
        } else {
            continue;
        }
    }

    return Chromosome(_route, _boarding);
}
