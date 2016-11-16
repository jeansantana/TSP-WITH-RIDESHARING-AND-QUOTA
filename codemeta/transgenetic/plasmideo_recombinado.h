//
// Created by jeansilva on 07/06/16.
//

#ifndef TRANSGENETIC_PLASMIDIO_RECOMBINADO_H
#define TRANSGENETIC_PLASMIDIO_RECOMBINADO_H


#include "plasmideo.h"

class plasmideo_recombinado: public plasmideo {
public:
    plasmideo_recombinado();
    plasmideo_recombinado(vector<endossimbionte> informacaoGenetica);
    vector<endossimbionte> obterInformacaoGenetica();

};


#endif //TRANSGENETIC_PLASMIDIO_RECOMBINADO_H
