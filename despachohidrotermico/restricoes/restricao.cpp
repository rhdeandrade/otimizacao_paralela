#ifndef restricao_cpp
#define restricao_cpp

#include "restricao.h"
#include "restricao_atendimento_demanda.cpp"
#include "restricao_balanco_hidrico.cpp"
#include "restricao_defluencia_minima.cpp"
#include "restricao_limite_variaveis.cpp"

Restricao::Restricao() {
}


void Restricao::setBalancoHidrico(RestricaoBalancoHidrico *restricao) {
  balanco_hidrico = restricao;
}

void Restricao::setAtendimentoDemanda(RestricaoAtendimentoDemanda *restricao) {
  atendimento_demanda = restricao;
}

void Restricao::setDefluenciaMinima(RestricaoDefluenciaMinima *restricao) {
  defluencia_minima = restricao;
}

void Restricao::setLimiteVariaveis(RestricaoLimiteVariaveis *restricao) {
  limite_variaveis = restricao;
}


#endif
