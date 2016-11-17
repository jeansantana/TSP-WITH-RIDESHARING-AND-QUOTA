#include "common/macros.h"
#include "genetic/Chromosome.h"
#include "model/Solution.h"
#include "common/Shared.h"
#include "common/MersenneTwister.h"
#include "genetic/GeneticAlgorithm.h"

vector<string> readFile(string path) {
    vector<string> arq;
    string line;
    ifstream file(path.c_str());

    if ( file.is_open() ) {
        while ( getline(file, line) ) {
            if (!line.compare("") == 0)
                arq.pb( line );
        }
        file.close();
    }
    return arq;
}

vector<string> split(string str, string find) {
    vector<string> pieces;
    int idx = str.find(find);
    while (idx != -1 && idx != str.size() - 1) {

        pieces.pb(str.substr(0, idx));
        str = str.substr(idx + 1, str.size());
        idx = str.find(find);
    }
    pieces.pb(str);
    return pieces;
}

void assignPCVPQ(string name) {
    vector<string> file = readFile(name);
    Shared *shared = Shared::getInstance();
    vector<string> pieces = split(file[0], " ");

    shared->setN(stoi(pieces[0]));
    shared->setNumPassengers(stoi(pieces[1]));
    shared->setVehicleCapacity(stoi(pieces[2]));
    vvd M;
    FORR(i, 1, shared->getN() + 1) {
        M.pb(vd());
        vector<string> line = split(file[i], " ");
        FOR(j, line.size()) {
            M[i - 1].pb(stoi(line[j]));
        }
    }

    shared->setCosts(M);

    vd budgets;
    vi boarding;
    FORR(i, shared->getN() + 1, shared->getNumPassengers() + shared->getN() + 1) {
        vector<string> tarifas = split(file[i], " ");
        int a;
        double c;
        c = stod(tarifas[0]);
        //shared->setTarifaI(i - (shared->getN() + 1), c);
        budgets.pb(c);
        a = stoi(tarifas[2]);
        //shared->setEmbarqueI(i - (shared->getN() + 1), a);
        boarding.pb(a);
    }

    shared->setPassengerBudget(budgets);
    shared->setBoarding(boarding);
    vi bns;
    int lim = shared->getN() + 1 + shared->getNumPassengers();
    shared->setMinimumQuota(stoi(file[lim]));
    lim++;
    FORR(i, lim, file.size()) {
        vector<string> line = split(file[i], " ");
        bns.pb(stoi(line[1]));
    }
    shared->setBonuses(bns);
}

int main(int argc, char const *argv[]) {

    // TODO: set b1 = 0, the first bonus must be 0

    Shared * shared = Shared::getInstance();
    // read instance
    // number of cities
    int N;
    // cost matrix. It contains each traveling cost between the cities involved
    vvd costs;
    // passenger budget
    vd passengerBudget;
    // Departure city (position) and arrival (boarding[position]) of the passenger
    vi boarding; //Vértice de embarque (posição) e desembarque (conteúdo)
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

    /*freopen (argv[1], "r", stdin);

    cin >> N >> numPassengers >> vehicleCapacity;
    // cost matrix
    FOR(i, N) {
        costs.pb(vd());
        FOR(j, N) {
            double aux;;
            cin >> aux;
            costs[i].pb(aux);
        }
    }

    FOR(i, numPassengers) {
        double budget;
        int departure, arrival;

        cin >> budget >> departure >> arrival;

        passengerBudget.pb(budget);
        boarding.pb(arrival);

    }

    // read bonuses
    cin >> minimumQuota;
    FOR(i, N) {
        int city, cBonus;
        cin >> city >> cBonus;
        bonuses.pb(cBonus);
    }
    // ensures the bonus in the city 0 is 0
    //bonuses[0] = 0;
*/

    // ======= //

//    assignPCVPQ("/home/jeansilva/Dropbox/Mestrado/metaheuristica/PCV-PQ/src/instances/paper/all/10-10-50/TIC15-10-10-4.in");
    assignPCVPQ("/home/jeansilva/Dropbox/Mestrado/metaheuristica/PCV-PQ/src/instances/big.in");

    // ======= //


    // converts the problem to TSP with Passengers (with no quota)
    /*int somaBonus = 0;
    FOR(i, N) somaBonus+= bonuses[i];
    minimumQuota = somaBonus;*/

    // set the singleton Shared
    /*shared->setN(N);
    shared->setBoarding(boarding);
    shared->setBonuses(bonuses);
    shared->setMinimumQuota(minimumQuota);
    shared->setNumPassengers(numPassengers);
    shared->setVehicleCapacity(vehicleCapacity);
    shared->setPassengerBudget(passengerBudget);
    shared->setCosts(costs);*/

    //cout << shared->toString() << endl;

    /*vi route({0, 6, 2, 7, 5, 3, 1, 4, 8, 9});
    vi embark({1, 1, 1, 0, 0, 1, 0, 1, 1, 0});

    *//*Solution s(route, embark);
    cout << s.toString() << endl;*//*

    Chromosome c(route, embark);
    //cout << c.fitness().toString() << endl;
    cout << c.toString() << endl;
    c.repair();
    //cout << c.fitness().toString() << endl;
    cout << c.toString() << endl;

    vi _route({0, 5, 4, 6, 2});
    vi _embark({1, 0, 0, 1, 1});
    *//*vi r({0, 6, 2, 7, 5, 3});
    vi e({1, 1, 1, 0, 0, 1});*//*

    Chromosome b(_route, _embark);
    b.repair();

//    cout << "B:\n" << b.toString() << endl;

    Chromosome f1 = c.reproduce(b);
    //f1.fitness();
    cout << f1.toString() << endl;*/

    //c.reproduce()

    //cout << " ------------ \n";
    //int pro;
    //cout << s.fitness(route, embark, pro).getCost() << endl;
    //    int populationSize, double admissibilityLimit, int maxEvaluations, int recombinationRate

    /*GeneticAlgorithm ga(1000, 0.4, 10000, 0.5);

    pair<Solution, Chromosome> ans = ga.run();

    cout << ans.first.toString() << endl;*/

    vi _route({0, 1, 3, 2, 4});
    vi _embark({1, 1, 1, 0, 0});

    Solution s(_route, _embark);
    Chromosome ch(_route, _embark);
    ch.repair();
    cout << ch.toString() << endl << endl;

    //cout << ch.fitness().toString() << endl;

    vi __route({0, 3, 1});
    vi __embark({0, 0, 0});

    Solution _s(__route, __embark);
    Chromosome _ch(__route, __embark);
    _ch.repair();
    cout << _ch.toString() << endl << endl;

    // cout << _ch.fitness().toString() << endl;

    Chromosome child = ch.reproduce(_ch);
    child.repair();
    cout << child.toString() << endl;



    return 0;
}