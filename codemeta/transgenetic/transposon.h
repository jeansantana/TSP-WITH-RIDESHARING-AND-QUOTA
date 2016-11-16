//
// Created by jeansilva on 07/06/16.
//

#ifndef TRANSGENETIC_TRANSPOSON_H
#define TRANSGENETIC_TRANSPOSON_H


#include "vetor_transgenetico.h"

class transposon: public vetor_transgenetico {
private:
    endossimbionte _simulacao;
    ii identificacao;
    endossimbionte buscaLocal(endossimbionte e);
    endossimbionte melhorVizinho(endossimbionte e);
public:
    transposon();
    transposon(ii identificacao);
    virtual bool ataque(const endossimbionte e);
    virtual bool transcrever(endossimbionte &e);
    ii getIdentificacao();
    void setIdenficacao(ii identificacao);
};



#endif //TRANSGENETIC_TRANSPOSON_H
