//
// Created by jeansilva on 03/08/16.
//

#ifndef TSPRQ_MACROS_H_H
#define TSPRQ_MACROS_H_H

// Precompiled header that includes a lots of files
#include <bits/stdc++.h>

using namespace std;

typedef unsigned long long int ulli;
typedef pair<int, int> ii;
typedef vector< vector <int> > vvi;
typedef vector<int> vi;
typedef vector<string> vs;
typedef unsigned int ui;
typedef vector<ii> vii;
typedef vector<double> vd;
typedef vector< vector < double > > vvd;
//typedef pair<double, ui> solucao; // custo do caixeiro e custo da coleta de bônus
//typedef map<double, int> dic_solution;

#define FORR(i, a, b) for (int i = a; i < b; ++i)
#define FOR(i, a) FORR(i, 0, a)
#define FORE(i, a, b) for(int i = a; i >= b; --i)
#define pb push_back
#define rm erase
#define mp make_pair
#define MAX_N 10000
#define NUM_RODADAS 30
#define MAX_VALUE_PROB 1000000
#define MINIMUM_QUOTA_PROBLEM -2
#define SOLUTION_OK -1

//======== Random macros and vars ===========
typedef std::mt19937 mt;
//==================================
// Running algorithm type
enum AlgorithmType { GENETIC, MEMETIC, TRANSGENETIC, OTHER };
// Crossover/Recombination operation type
enum Operator { RECOMBINATION, SCX_ADP, SCX_BASED_ADP };

#endif //TSPRQ_MACROS_H_H
