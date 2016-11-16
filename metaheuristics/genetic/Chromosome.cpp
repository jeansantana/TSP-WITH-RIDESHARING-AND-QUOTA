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

    // add something here?

}

Chromosome Chromosome::reproduce(Chromosome other) {

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

void Chromosome::mutation(double a_lim) {
    // TODO: Ideia: get the minimum and replace by another with a good bonus?
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
