//
// Created by jeansilva on 02/06/16.
//

#ifndef TRANSGENETIC_PLASMIDIO_H
#define TRANSGENETIC_PLASMIDIO_H


#include "vetor_transgenetico.h"

class plasmideo: public vetor_transgenetico {
protected:
    vector<endossimbionte> informacaoGenetica;
    endossimbionte _simulacao;
public:
    plasmideo();
    plasmideo(vector<endossimbionte> informacaoGenetica);
    ~plasmideo();

    endossimbionte injetarCodigoGenetico(endossimbionte e);
    vector<endossimbionte> obterInformacaoGenetica();
    virtual bool ataque(const endossimbionte e);
    virtual bool transcrever(endossimbionte &e);
    vector<endossimbionte> getInformacaoGenetica();
    void setInformacaoGenetica(vector<endossimbionte> informacaoGenetica);

};


#endif //TRANSGENETIC_PLASMIDIO_H
