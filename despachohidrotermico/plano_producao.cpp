#ifndef plano_producao_cpp
#define plano_producao_cpp


#include "plano_producao.h"
#include "otimizacao_despacho_hidrotermico_globals.cpp"
#include "../usina/usina_termica.cpp"
#include "../usina/usina_hidreletrica.cpp"
#include "../usina/subsistema.cpp"
#include "restricoes/restricao.cpp"



PlanoProducao::PlanoProducao(PlanoProducao *p) {
  this->termicas.swap(p->termicas);
  this->hidreletricas.swap(p->hidreletricas);
  this->subsistemas.swap(p->subsistemas);
}

PlanoProducao::PlanoProducao() {}


PlanoProducao PlanoProducao::perturbation(int operacao_atomica, int counter) {
  //Implementado apenas operacao atomica 4, que é a usada atualmente
  if (operacao_atomica == 4)  {
    return this->executar(this, counter);
  }
}

double PlanoProducao::planejar_maximizacao_energia_hidraulica(vector<UsinaHidreletrica> h, int periodo) {
  double resultado = 0.0;
  vector<int> cascata74 = OtimizacaoDespachoHidrotermicoGlobals::get_instance()->cascata74;

  h = OtimizacaoDespachoHidrotermicoGlobals::ordernar_hidreletricas_tamanho_reservatorio(h, false);

  for (int i = 0; i < h.size(); ++i) {
    if (find(cascata74.begin(), cascata74.end(), h.at(i).id_usina) != cascata74.end()) {
      continue;
    }
    else {
      if (h.at(i).reservatorio.obter_tamanho() > 0) {
        resultado += h.at(i).maximizar_producao_energia(periodo, UsinaHidreletrica::TIPO_MAXIMIZACAO_RESERVATORIO, true);
      }
      else {
        resultado += h.at(i).maximizar_producao_energia(periodo, UsinaHidreletrica::TIPO_MAXIMIZACAO_AFLUENCIA_NATURAL, true);
      }

      GeracaoEnergia* geracao = h.at(i).obter_geracao_energia(periodo);

      resultado -= geracao->quantidade;
    }
  }

  return resultado;
}

double PlanoProducao::minimizar_energia_termica(vector<UsinaTermica> t, int periodo, double* energia_hidraulica_sobrando) {
  double resultado = 0.0;

  t = OtimizacaoDespachoHidrotermicoGlobals::obter_termicas_com_prioridade_desativacao(t, periodo);

  for (int i = 0; i < t.size(); i++) {
    GeracaoEnergia* geracao = t.at(i).obter_geracao_energia(periodo);
    double saldo = *energia_hidraulica_sobrando - geracao->quantidade;

    if (saldo > 0) {
      double quantidade = t.at(i).iniciar_processo_desativacao(periodo);
      *energia_hidraulica_sobrando -= quantidade;

      resultado += quantidade;
    }
  }  

  return resultado;
}

double PlanoProducao::produzir_energia_hidraulica(vector<UsinaHidreletrica> hidreletricas, int periodo, double total_energia_termica) {
  double total_energia_produzida;

  hidreletricas = OtimizacaoDespachoHidrotermicoGlobals::ordernar_hidreletricas_tamanho_reservatorio(hidreletricas, false);

  vector<int> cascata74 = OtimizacaoDespachoHidrotermicoGlobals::get_instance()->cascata74;

  for (int i = 0; i < hidreletricas.size(); i++) {
    if (find(cascata74.begin(), cascata74.end(), hidreletricas.at(i).id_usina) != cascata74.end()) {
      continue;
    }

    if (total_energia_termica > total_energia_produzida) {
      if (hidreletricas.at(i).reservatorio.obter_tamanho() > 0) {
        total_energia_produzida += hidreletricas.at(i).maximizar_producao_energia(periodo, UsinaHidreletrica::TIPO_MAXIMIZACAO_RESERVATORIO, false);
      }
      else {
        total_energia_produzida += hidreletricas.at(i).maximizar_producao_energia(periodo, UsinaHidreletrica::TIPO_MAXIMIZACAO_AFLUENCIA_NATURAL, false);
      }
    }
  }

  return total_energia_produzida;
}

PlanoProducao PlanoProducao::executar(PlanoProducao p, int counter) {
  for (int i = 0; i < p.subsistemas.size(); i++) {
    vector<UsinaHidreletrica> hidreletricas = OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_hidreletricas(p.hidreletricas, p.subsistemas.at(i).id_subsistema);
    vector<UsinaTermica> termicas = OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_termicas(p.termicas, p.subsistemas.at(i).id_subsistema);
    double total_energia_hidraulica_sobrando = planejar_maximizacao_energia_hidraulica(p.hidreletricas, counter);
  
    double total_energia_termica_desligada = minimizar_energia_termica(p.termicas, counter, &total_energia_hidraulica_sobrando);

    double total_energia_hidraulica_produzida = produzir_energia_hidraulica(hidreletricas, counter, total_energia_termica_desligada);

  }

  OtimizacaoDespachoHidrotermicoGlobals::atualizar_plano_producao(p);

  return p;
}

void PlanoProducao::ativarRestricoes(bool balancoHidrico, bool atendimentoDemanda, bool defluenciaMinima, bool limiteVariaveis) {
  if(atendimentoDemanda)
    this->restricoes.setAtendimentoDemanda(new RestricaoAtendimentoDemanda(subsistemas, hidreletricas, termicas));
  if(balancoHidrico)
    this->restricoes.setBalancoHidrico(new RestricaoBalancoHidrico(hidreletricas));
  if(defluenciaMinima)
    this->restricoes.setDefluenciaMinima(new RestricaoDefluenciaMinima(hidreletricas));
  if(limiteVariaveis)
    this->restricoes.setLimiteVariaveis(new RestricaoLimiteVariaveis(hidreletricas, termicas));
}

double PlanoProducao::calcularValorPresente(int periodo) {
  double result = 1 + OtimizacaoDespachoHidrotermicoGlobals::TAXA_DESCONTO;
  result = pow(result, periodo);
  result = 1 / result;
  return result;

}

double PlanoProducao::objectiveFunctionValue() {
  double custo = 0;

  for (int i = 0; i < OtimizacaoDespachoHidrotermicoGlobals::NUM_PERIODOS; i++) {
    double custoTermica = 0;
    double custoDeficit = 0;
    for (int j = 0; j < this->termicas.size(); j++) {
      custoTermica += this->termicas.at(j).custo_termica_mega_watt_medio(i);
    }

    for (int j = 0; j < this->subsistemas.size(); j++) {
      if (this->subsistemas.at(j).id_subsistema != 5)
        custoDeficit += this->subsistemas.at(j).custoDeficit(i);
    }

    double result = custoTermica + custoDeficit;
    result *= this->calcularValorPresente(i);

    custo += result;
  }

  return custo;
}

#endif