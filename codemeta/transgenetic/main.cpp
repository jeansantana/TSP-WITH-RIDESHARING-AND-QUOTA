#include <iostream>
#include "vetor_transgenetico.h"

#include "PCVPQ.h"
#include "plasmideo.h"
#include "mersennetwister.h"
#include "virus.h"
#include "plasmideo_recombinado.h"
#include "transposon.h"
#include "transgenetic.h"

using namespace std;

string getValue(endossimbionte e) {
    return e.isBloqueado() == true ? "bloqueado" : "desbloqueado";
}

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
    vector<string> file = readFile("/home/jeansilva/ClionProjects/transgenetic/instancias/" + name);
    PCVPQ *pcvpq = PCVPQ::getInstance();
    pcvpq->setNumAvaliacoes(0);
    vector<string> pieces = split(file[0], " ");

    pcvpq->setN(stoi(pieces[0]));
    pcvpq->setNumPassageiros(stoi(pieces[1]));
    pcvpq->setCapacidade(stoi(pieces[2]));

    FORR(i, 1, pcvpq->getN() + 1) {
        vector<string> line = split(file[i], " ");
        FOR(j, line.size()) {
            pcvpq->M(i - 1, j, stoi(line[j]));
        }
    }

    FORR(i, pcvpq->getN() + 1, pcvpq->numPassageiros() + pcvpq->getN() + 1) {
        vector<string> tarifas = split(file[i], " ");
        int a;
        double c;
        c = stod(tarifas[0]);
        pcvpq->setTarifaI(i - (pcvpq->getN() + 1), c);
        a = stoi(tarifas[2]);
        pcvpq->setEmbarqueI(i - (pcvpq->getN() + 1), a);
    }

    int lim = pcvpq->getN() + 1 + pcvpq->numPassageiros();
    pcvpq->setQuota(stoi(file[lim]));
    lim++;
    FORR(i, lim, file.size()) {
        vector<string> line = split(file[i], " ");
        pcvpq->setBonusI(stoi(line[0]), stoi(line[1]));
    }

}

solucao experimento(string name) {
    assignPCVPQ(name);
    solucao best;
    transgenetic at(300, 150, 1000000);
    best = at.run().avaliacao();

    FOR(i, NUM_RODADAS - 1) {
        solucao atual = at.run().avaliacao();
        if (atual < best)
            best = atual;
    }
    return best;
}

int main(int argc, char const *argv[]) {

    int NUM_INSTANCIAS = 10;
    string instances[] = {"paper/TIC52-40-40-4.in",
                          "paper/TIC53-20-20-4.in",
                          "paper/TIC53-50-50-4.in",
                          "paper/TIC54-40-40-4.in",
                          "paper/TIC55-20-20-4.in",
                          "paper/TIC56-20-20-4.in",
                          "paper/TIC58-40-40-4.in",
                          "paper/TIC58-50-50-4.in",
                          "paper/TIC59-50-50-4.in",
                          "paper/TIC60-30-30-4.in"};
    FOR(i, NUM_INSTANCIAS) {
        clock_t tStart = clock();
        //cout << "#processando instancia: " << instances[i] << " ..." << endl;
        solucao s = experimento(instances[i]);

        double tmp = (double)(clock() - tStart)/CLOCKS_PER_SEC;
        cout << instances[i] << ", " << s.getCusto() << ", " << tmp/NUM_RODADAS << endl;
    }

    return 0;

    //leitura da entrada padrão
    /*freopen (argv[1], "r", stdin);
    int n, p, k;
    while(cin >> n >> p >> k) {
        PCVPQ * pcvpq = PCVPQ::getInstance();
        pcvpq->setN(n);
        pcvpq->setNumPassageiros(p);
        pcvpq->setCapacidade(k);

        FOR(i, n) {
            FOR(j, n) {
                int aux;
                cin >> aux;
                pcvpq->M(i, j, aux);
            }
        }

        FOR(i, p) {
            double a;
            int b, c;
            cin >> a >> c >> b;
            pcvpq->setTarifaI(i, a);
            pcvpq->setEmbarqueI(i, b);
        }

        FOR(i, n) {
            pcvpq->setBonusI(i, 0);
        }

        int min_bonus;
        cin >> min_bonus;
        pcvpq->setQuota(min_bonus);

        FOR(i, n) {
            int vertice, bonus_value;
            cin >> vertice >> bonus_value;
            pcvpq->setBonusI(vertice, bonus_value);
        }

        // transgenetic(int tamPop, int numSelecionados, int numAvaliacoes);
        FOR(i, n) {
            FOR(j, n) {
                cout << pcvpq->M(i, j) << " ";
            }
            cout << endl;
        }
        cout << endl;
        transgenetic at(100, 20, 400);
        endossimbionte sol = at.run();
        cout << sol.toString() << endl;

    }
    return 0;*/

}