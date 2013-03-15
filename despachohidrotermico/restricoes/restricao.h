#ifndef restricao_h
#define restricao_h

#include <iostream>
#include "restricao_atendimento_demanda.h"
#include "restricao_balanco_hidrico.h"
#include "restricao_defluencia_minima.h"
#include "restricao_limite_variaveis.h"

using namespace std;

class Restricao {
  public:
    RestricaoAtendimentoDemanda *atendimento_demanda;
    RestricaoBalancoHidrico *balanco_hidrico;
    RestricaoDefluenciaMinima *defluencia_minima;
    RestricaoLimiteVariaveis *limite_variaveis;

    void setAtendimentoDemanda(RestricaoAtendimentoDemanda *restricao);
    void setBalancoHidrico(RestricaoBalancoHidrico *restricao);
    void setDefluenciaMinima(RestricaoDefluenciaMinima *restricao);
    void setLimiteVariaveis(RestricaoLimiteVariaveis *restricao);
    Restricao();
};

#endif
