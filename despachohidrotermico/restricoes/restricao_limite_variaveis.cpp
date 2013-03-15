#ifndef restricao_limite_variaveis_cpp
#define restricao_limite_variaveis_cpp

#include "restricao_limite_variaveis.h"

RestricaoLimiteVariaveis::RestricaoLimiteVariaveis(vector<UsinaHidreletrica> hidreletricas, vector<UsinaTermica> termicas) {
  this->hidreletricas = hidreletricas;
  this->termicas = termicas;
}

RestricaoLimiteVariaveis::RestricaoLimiteVariaveis() {
}

bool RestricaoLimiteVariaveis::checkConstraint()
{
  cout << "Validando Limite Variaveis... ";
  bool constraintOk = true;
  errors = "";

  // passa por todos o periodos
  for(int i = 1; i <= OtimizacaoDespachoHidrotermicoGlobals::NUM_PERIODOS; i++)
  {
    // passa por todas as hidreletricas
    for (int l = 0; l < this->hidreletricas.size(); l++) 
    {
      UsinaHidreletrica hidreletrica = this->hidreletricas[l];

      if(RestricaoLimiteVariaveis::capacidadeArmazenamentoReservatorio(hidreletrica, i))
        constraintOk = false;

      if(RestricaoLimiteVariaveis::capacidadeVazaoTurbinadaReservatorio(hidreletrica, i))
        constraintOk = false;

      if(RestricaoLimiteVariaveis::capacidadeVazaoVertidaReservatorio(hidreletrica, i))
        constraintOk = false;
    }
    for (int l = 0; l < this->termicas.size(); l++) 
    {
      UsinaTermica termica = this->termicas[l];
      if(RestricaoLimiteVariaveis::capacidadeGeracaoEnergiaTermica(termica, i))
        constraintOk = false;
    }
  }

  return constraintOk;
}
/**
 * Limite da capacidade de armazenamento do reservatório
 * Se violou a restrição retorna true. Caso contrário false 
 * @return Boolean
 */
bool RestricaoLimiteVariaveis::capacidadeArmazenamentoReservatorio(UsinaHidreletrica hidreletrica, int periodo)
{
  HistoricoOperacaoReservatorio* historico = hidreletrica.reservatorio.obter_historico_reservatorio(periodo, hidreletrica.reservatorio.volume_maximo);

  if(hidreletrica.reservatorio.volume_maximo < historico->volume)
  {
    this->errors.append("Hidreletrica: Volume maior que volume maximo\n");
    return true;
  }

  if(hidreletrica.reservatorio.volume_minimo > historico->volume)
  {
    this->errors.append("Hidreletrica: Volume menor que volume minimo\n");
    return true;
  }

  return false;
} 

/**
 * Limite da capacidade de vazão turbinada do reservatório
 * Se violou a restrição retorna true. Caso contrário false 
 * @return Boolean
 */
bool RestricaoLimiteVariaveis::capacidadeVazaoTurbinadaReservatorio(UsinaHidreletrica hidreletrica, int periodo)
{
  HistoricoOperacaoReservatorio* historico = hidreletrica.reservatorio.obter_historico_reservatorio(periodo, hidreletrica.reservatorio.volume_maximo);

  if(hidreletrica.reservatorio.maximo_vazao_turbinada < historico->vazao_turbinada)
  {
    this->errors.append("Hidreletrica: Vazao turbinada maior que vazao maxima turbinada\n");
    return true;
  }

  if(hidreletrica.reservatorio.minimo_vazao_turbinada > historico->vazao_turbinada) 
  {
    this->errors.append("Hidreletrica: Vazao turbinada menor que vazao minima turbinada\n");
    return true;
  }

  return false;
}

/**
 * Limite da capacidade de vazão vertida do reservatório
 * Se violou a restrição retorna true. Caso contrário false 
 * @return Boolean
 */
bool RestricaoLimiteVariaveis::capacidadeVazaoVertidaReservatorio(UsinaHidreletrica hidreletrica, int periodo)
{
  HistoricoOperacaoReservatorio* historico = hidreletrica.reservatorio.obter_historico_reservatorio(periodo, hidreletrica.reservatorio.volume_maximo);

  if(hidreletrica.reservatorio.maximo_vazao_vertida < historico->vazao_vertida)
  {
    this->errors.append("Hidreletrica: Vazao vertida maior que vazao maxima vertida\n");
    return true;
  }

  if(historico->vazao_turbinada < 0)
  {
    this->errors.append("Hidreletrica: Vazao vertida menor que vazao minima vertida\n");
    return true;
  }

  return false;
}

/**
 * Limite da capacidade de geração de energia das térmicas
 * Se violou a restrição retorna true. Caso contrário false 
 * @return Boolean
 */
bool RestricaoLimiteVariaveis::capacidadeGeracaoEnergiaTermica(UsinaTermica termica, int periodo)
{
  GeracaoEnergia* geracao = termica.obter_geracao_energia(periodo);

  if(termica.quantidade_geracao_max < geracao->quantidade)
  {
    this->errors.append("Termica: Quantidade de geração maior que a quantidade maxima\n");
    return true;
  }

  if(termica.quantidade_geracao_min > geracao->quantidade)
  {
    this->errors.append("Termica: Quantidade de geração menor que a quantidade minima\n");
    return true;
  }

  return false;
}

double RestricaoLimiteVariaveis::errorThreshold()
{
  return 0;
}

void RestricaoLimiteVariaveis::printMessageConstraintBroken() {
  cout << errors;
}
#endif
