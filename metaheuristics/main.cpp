/**
 * @author Jean Silva
 * How to use the program: We have this parameters to be informed at the execution
 * 1. Entry file
 * 2. Population Size
 * 3. Number of avaliations
 * 4. Crossover rate
 * 5. Global mutation rate
 * 6. Algorithm type:
 *          6.1 - 1 to use Genetic Algorithm;
 *          6.2 - 2 to use Memetic Algorithm;
 * 7. Genetic operator for crossover/recombination:
 *          7.1 - 1 to use split and recombiantion operator
 *          7.2 - 2 to use the SCX operator
 *          7.3 - 3 to user the SCX based operator
 * 8. Instance type
 *          8.1 - 1 to indicate that the instance is SIMETRIC (local search used: Lin Kernighan)
 *          8.2 - 2 to indicate that the instance is ASSIMETRIC (local search used: 2-opt)
 *
 * Example: ./executable instances/big.in 100 200 0.5 0.6 1 1 1
 */


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

    // read instance
    // number of cities
    int N;
    // cost matrix. It contains each traveling cost between the cities involved
    vvd costs;
    // passenger budget
    vd passengerBudget;
    // Departure city (position) and arrival (boarding[position]) of the passenger
    vi boarding; // Vértice de embarque (posição) e desembarque (conteúdo)
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

    freopen (argv[1], "r", stdin);

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
    bonuses[0] = 0;


    // converts the problem to TSP with Passengers (with no quota)
    /*int somaBonus = 0;
    FOR(i, N) somaBonus+= bonuses[i];
    minimumQuota = somaBonus;*/

    stringstream sst;
    sst << argv[2];
    int populationSize; sst >> populationSize; sst.clear();
    sst << argv[3];
    int numEvaluates; sst >> numEvaluates; sst.clear();
    sst << argv[4];
    double corssoverRate; sst >> corssoverRate; sst.clear();
    sst << argv[5];
    // mutatition rate is the admissibility limit
    double mutationRate; sst >> mutationRate; sst.clear();
    sst << argv[6];
    int algorithmeType; sst >> algorithmeType; sst.clear();
    sst << argv[7];
    int geneticOperator; sst >> geneticOperator; sst.clear();
    sst << argv[8];
    int instanceType; sst >> instanceType; sst.clear();

    // não é um parâmetro para o iRace
    /*sst << argv[6];*/
    int rodadas = NUM_RODADAS; //sst >> rodadas; sst.clear();

    // set the singleton Shared
    Shared * shared = Shared::getInstance();

    // setting the size of cost matrix
    shared->setN(N);
    // setting the embarking
    shared->setBoarding(boarding);
    // setting the bonuses values
    shared->setBonuses(bonuses);
    // setting the minimum quota
    shared->setMinimumQuota(minimumQuota);
    // setting numeber of passengers
    shared->setNumPassengers(numPassengers);
    // setting the vehicle capacity
    shared->setVehicleCapacity(vehicleCapacity);
    // setting the passenger budget
    shared->setPassengerBudget(passengerBudget);
    // setting the costs
    shared->setCosts(costs);
    // setting the algorithm wich is running
    shared->setAlgorithmType((AlgorithmType) (algorithmeType - 1));
    // setting the agenetic operator wich is used
    shared->setGeneticOpearator((GeneticOperator) (geneticOperator - 1));
    // setting the instance type (1 - SIMETRIC or 2 - ASSIMETRIC)
    shared->setInstanceType((InstanceType) (instanceType - 1));

    // cout << "file name: " << argv[0] << endl;

    GeneticAlgorithm genetic(populationSize, mutationRate, numEvaluates, mutationRate);
    pair<Solution, Chromosome> sol = genetic.run();
    cout << sol.first.getCost() << endl;

    /*vi route({0, 3, 1, 4, 9, 7, 2, 5});
    vi embark({0, 0, 1, 0, 0, 0, 0, 0});

    vi _route({0, 7, 4, 3, 1, 8});
    vi _embark({1, 0, 1, 0, 0, 0});

    Chromosome a(route, embark);
    Chromosome b(_route, _embark);

    Chromosome c = a.reproduce(b);
    cout << c.toString() << endl;*/

    delete(shared);


    //cout << shared->toString() << endl;

    /*
    How to use the Chromosome and Solution Class
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
    cout << child.toString() << endl;*/

    return 0;
}