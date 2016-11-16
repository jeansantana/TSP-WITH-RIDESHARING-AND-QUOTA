//
// Created by jeansilva on 02/06/16.
//

#include "endossimbionte.h"
#include "PCVPQ.h"
#include "mersennetwister.h"

endossimbionte::endossimbionte(vector<int> rota, vector<int> carregamento) {
    this->rota = rota;
    this->carregamento = carregamento;
    this->bloqueado = false;
}

endossimbionte::endossimbionte() {
    this->bloqueado = false;
}

endossimbionte::~endossimbionte() { }

vector<int> endossimbionte::getRota() {
    return rota;
}

vector <int> endossimbionte::getCarregamento() {
    return carregamento;
}

void endossimbionte::setRota(vector <int> rota) {
    this->rota = rota;
}

void endossimbionte::setCarregamento(vector <int> carregamento) {
    this->carregamento = carregamento;
}

int endossimbionte::size() {
    return rota.size();
}

void endossimbionte::setCarregamentoI(int i, int v) {
    assert(i < this->size() && i >= 0);
    this->carregamento[i] = v;
}

void endossimbionte::setRotaI(int i, int v) {
    assert(i < this->size() && i >= 0);
    this->rota[i] = v;
}

int endossimbionte::getCarregamentoI(int i) {
    assert(i < this->size() && i >= 0);
    return this->carregamento[i];
}

int endossimbionte::getRotaI(int i) {
    assert(i < this->size() && i >= 0);
    return this->rota[i];
}

bool endossimbionte::isBloqueado() {
    if (bloqueado && PCVPQ::getInstance()->getIteracao() > _iteracaoEnd) {
        // desbloqueio automático quando for necessário usar esse endossimbionte
        this->bloqueado = false;
    }
    return this->bloqueado;
}

void endossimbionte::bloquear(int iteracoes) {
    _iteracaoEnd = PCVPQ::getInstance()->getIteracao() + iteracoes;
    this->bloqueado = true;
}

// se precisar forcar o bloqueio/desbloqueio
void endossimbionte::setBloquear(bool bloq) {
    this->bloqueado = bloq;
}

bool endossimbionte::operator < (endossimbionte o) {
    // solucao = pair<double, ui>
    solucao local = this->avaliacao();
    solucao outra = o.avaliacao();

    if ( local < outra ) {
        return true;
    }
    return false;
}

bool compareBonus(ii a, ii b) {
    return a.second > b.second;
}

void endossimbionte::reparar() {
    // Pego todas as cidades que não estejam na rota, ordeno em ordem crescente e vou inserindo e testando se atigi a cota
    //se a cota não é alcançada! Faça com que seja!
    PCVPQ * pcvpq = PCVPQ::getInstance();
    int n = pcvpq->getN();

    int cota = 0;
    int inPath[n];
    vii cotas;
    FOR(i, n) {
        inPath[i] = 0;
        cotas.push_back(ii(i, pcvpq->bonus(i)));
    }

    FOR(i, rota.size()) {
        cota+= pcvpq->bonus( rota[i] );
        inPath[ rota[i] ] = 1;
    }

    sort(cotas.begin(), cotas.end(), compareBonus);

    //reparar bonus
    bool reparou = false;
    if (cota < pcvpq->quota()) {
        // pegando os candidatos
        for (int i = 0; i < n && !reparou; i++) {
            if ( inPath[ cotas[i].first ] == 0 ) {
                cota+= cotas[i].second;
                rota.pb( cotas[i].first );
                carregamento.pb(0);
            }
            if ( cota >= pcvpq->quota() ) reparou = true;
        }
    }

    solucao s = avaliacao();

    while ( _consertar.size() != 0 ) {
        carregamento[ _consertar[0] ] = 0;
        s = avaliacao();
    }

    //Consertar guarda as posições em que precisam de reparação
    //tenta carregar todos os passageiros possíveis
    FOR(i, carregamento.size()) {
        if (carregamento[i] != 1) {
            carregamento[i] = 1;
            solucao sol = avaliacao();
            if (_consertar.size() == 0 && sol < s) {
                s = sol;
            } else {
                carregamento[i] = 0;
            }
        }
    }

    /*s = avaliacao(false);
    if (_consertar.size() == 0) {
        a.avaliacao.first = s.first;
        a.avaliacao.second = s.second;
    }*/
}

bool endossimbionte::embarqueIsOK() {

    /*O vértice de desembarque e embarque de
    cada passageiro que pretende embarcarm,
    deve estar em v, senão
    a configuração é inválida.*/
    PCVPQ * pcvpq = PCVPQ::getInstance();
    int n = pcvpq->getN();

    int inPath[n];//para cada vétice, se o vértice está contido ou n no caminho
    FOR(i, n) inPath[i] = 0;
    bool res = true;

    for (int i = 0; i < rota.size(); ++i) {
        //se for embarcar no vértice v[i] o vértice de desembarque tem que estar no caminho v
        if ( rota[i] != 0 ) inPath[ rota[i] ] = 1;
        if ( carregamento[i] == 1 && inPath[ pcvpq->embarque( rota[i] ) ] == 1 ) {
            res = false;
            _consertar.push_back(i);
        }
    }
    return res;
}

solucao endossimbionte::avaliacao() {

    _consertar.clear();

    PCVPQ * pcvpq = PCVPQ::getInstance();

    int bonus_coletado = 0;
    double p_cost = 0.0;
    int n = pcvpq->getN();

    int D[n];//Quantidade de desembarque em cada vértice
    int passou[n];//marca as cidades que já passou
    double taximetro[n + 1]; // valores parciais da viagem no instante t
    FOR(i, n) {
        //inicializando os vetores
        D[i] = 0;
        passou[i] = 0;
        taximetro[i] = 0;
    }
    taximetro[n] = 0;
    double total_cost = 0.0;
    int pass_num = 1; // contando com o motorista

    solucao s;
    //checa os vértices de desembarque dos passageiros que embarcam está OK
    if (!embarqueIsOK()) {
        return solucao (-1, -1);
    }

    FOR(i, size()) {
        bonus_coletado+= pcvpq->bonus( rota[i] );
        //desembarcar passageiro
        if (pass_num != 1 && i != 0) {
            pass_num-= D[ rota[i] ];
        }

        // se o passageiro i for embarcar e sua cidade de desembarque E[i] não passou, embarque i
        if (carregamento[i] == 1 && passou[ pcvpq->embarque( rota[i] ) ] == 0) {
            //embarque de passageiro
            // nas instancias a capacidade do carro n conta com o motorista, por isso soma + 1
            if (pass_num < pcvpq->capacidade() + 1) {
                pass_num++;
                //armazenando o momento em que terá de descer;
                D[ pcvpq->embarque( rota[i] ) ]++;
            } else {
                // violação na capacidade do carro
                s = solucao(-1, -1);
                _consertar.push_back(i);

            }
        } else if (carregamento[i] == 1 && passou[ pcvpq->embarque( rota[i] ) ] == 1) {
            // violação na cidade: cidade já passou
            s = solucao(-1, -1);
            _consertar.push_back(i);
        }

        double parc_cost = 0.0;
        if (i == rota.size() - 1) {
            parc_cost = pcvpq->M( rota[i], 0) / pass_num;
            p_cost = parc_cost;
        } else {
            parc_cost = pcvpq->M( rota[i], rota[i + 1] ) / pass_num;
        }
        taximetro[i + 1] = taximetro[i] + parc_cost;
        total_cost+= parc_cost;
        if (rota[i] != 0) passou[ rota[i] ] = 1;
    }

    // restrição de tarifa
    FOR(i, size()){
        int preprocessor[n];
        FOR(i, n) preprocessor[i] = 0;
        //proprocessamento para saber em que posição do vetor v, o vértice está
        FOR(j, size()) {
            preprocessor[rota[j]] = j;
        }

        int origem = preprocessor[i];
        int destino = preprocessor[ pcvpq->embarque(i) ];

        if (destino == 0) {
            destino = size() + 1;
        }

        if (carregamento[origem] == 1 && pcvpq->tarifa(i) < taximetro[destino] - taximetro[origem]) {
            s = solucao(-1, -1);
            _consertar.push_back(origem);
        }

    }

    if (_consertar.size() > 0) {
        s = solucao(-1, -1);
    } else {
        s = solucao(total_cost, bonus_coletado);
    }
    pcvpq->incrementaAvaliacoes();
    return s;
}

string endossimbionte::toString() {
    string res = "";
    for (int i = 0; i < this->size(); i++ ) {
        res+= to_string(rota[i]) + " ";
    }
    res+= "\n";
    for (int i = 0; i < this->size(); i++ ) {
        res+= to_string(carregamento[i]) + " ";
    }
    res+="\n";
    solucao s = avaliacao();
    res+= s.toString() + "\n";
    return res;
}


endossimbionte endossimbionte::getTrechoRandom() {

    vi rota, carregamento;
    /*rota.pb(0);
    carregamento.pb(0);*/
    int n = PCVPQ::getInstance()->getN();
    int tam = mersennetwister::getInstance()->getRand(1, n - 1);

    int inPath[n];
    FOR(i, n) inPath[i] = 0;
//    inPath[0] = 1;

    FOR(i, tam) {
        int cidade = mersennetwister::getInstance()->getRand(1, tam);
        if (inPath[cidade] == 0) {
            rota.pb(cidade);
            carregamento.pb(0);
            inPath[cidade] = 1;
        }
    }

    return endossimbionte(rota, carregamento);
}

endossimbionte endossimbionte::getRandom() {
    endossimbionte e = getTrechoRandom();
    e.addGene(0, 0);
    e.reparar();
    return e;
}

void endossimbionte::addGene(int cidade, int embarque) {

    int n = PCVPQ::getInstance()->getN();
    int inPath[n];
    FOR(i, n) inPath[i] = 0;

    if (inPath[cidade] == 0) {
        rota.pb(cidade);
        carregamento.pb(embarque);
        if (cidade == 0) {
            swap(rota[0], rota[size() - 1]);
            swap(carregamento[0], carregamento[size() - 1]);
        }
    }
}

