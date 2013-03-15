#ifndef otimizacao_despacho_hidrotermico_h
#define otimizacao_despacho_hidrotermico_h

#include <iostream>
#include "carregador_dados.cpp"
#include "otimizacao_despacho_hidrotermico_globals.h"
#include "restricoes/restricao.h"
#include "hill_climbing.cpp"


using namespace std;

class OtimizacaoDespachoHidrotermico {
  public:
    string log;
    string errors;
    CarregadorDados carregadorDados;
    Restricao restricoes;
    PlanoProducao plano_producao;
    // array errors;

  
    void carregarDados(string tipo, int serie);
    void ativarRestricoes(bool balancoHidrico, bool atendimentoDemanda, bool defluenciaMinima, bool limiteVariaveis);
    void executar_hill_climbing(int operacao_atomica, int numero_maximo_iteracoes, int numero_maximo_perturbacao);
    void validarPlanoProducao();
};


void OtimizacaoDespachoHidrotermico::carregarDados(string tipo, int serie) {
  cout << "Tipo: " << tipo << " Numero: " << serie << "\n";

  stringstream ss;

  ss << serie;

  string termicas =  "despachohidrotermico/dados/" + tipo + "/Termicas.txt";
  string geracaoTermicas =  "despachohidrotermico/dados/" + tipo + "/planoproducao/termoSaida_" + ss.str() + ".txt";
  string hidreletricas =  "despachohidrotermico/dados/" + tipo + "/Hidreletricas.txt";
  string geracaoHidreletricas =  "despachohidrotermico/dados/" + tipo + "/planoproducao/hidroSaida_" + ss.str() + ".txt";
  string subsistemas =  "despachohidrotermico/dados/" + tipo + "/Subsistemas.txt";
  string deficits =  "despachohidrotermico/dados/" + tipo + "/planoproducao/DeficitsSaida_" + ss.str() + ".txt";
  string demanda =  "despachohidrotermico/dados/" + tipo + "/planoproducao/Demanda.txt";
  string intercambios =  "despachohidrotermico/dados/" + tipo + "/planoproducao/IntercambiosSaida_" + ss.str() + ".txt";

  carregadorDados.carregar_nome_arquivos(termicas, geracaoTermicas, hidreletricas, geracaoHidreletricas, subsistemas, deficits,
                          demanda, intercambios); //Realiza função do construtor do CarregadorDados.php

  plano_producao.termicas = carregadorDados.carregar_usinas_termicas();

  plano_producao.hidreletricas = carregadorDados.carregar_usinas_hidreletricas();

  plano_producao.subsistemas = carregadorDados.carregar_subsistema();

  //OtimizacaoDespachoHidrotermicoGlobals::get_instance()->hidreletricas = plano_producao.hidreletricas;

}

void OtimizacaoDespachoHidrotermico::executar_hill_climbing(int operacao_atomica, int numero_maximo_iteracoes, int numero_maximo_perturbacao) {
  this->ativarRestricoes(true, true, true, true);
  HillClimbing hc(this->plano_producao, numero_maximo_iteracoes, numero_maximo_perturbacao);
  this->plano_producao = hc.execute(operacao_atomica);

}

void OtimizacaoDespachoHidrotermico::ativarRestricoes(bool balancoHidrico, bool atendimentoDemanda, bool defluenciaMinima, bool limiteVariaveis) {
  plano_producao.ativarRestricoes(balancoHidrico, atendimentoDemanda, defluenciaMinima, limiteVariaveis);
}

void OtimizacaoDespachoHidrotermico::validarPlanoProducao() {
  bool debug = false;

  errors = "";
  if (plano_producao.restricoes.atendimento_demanda->checkConstraint())
    cout << "Sem violações\n";
  else {
    cout << "Com violação\n";
    if (false && debug)
      plano_producao.restricoes.atendimento_demanda->printMessageConstraintBroken();
  }

  if (plano_producao.restricoes.balanco_hidrico->checkConstraint())
    cout << "Sem violações\n";
  else {
    cout << "Com violação\n";
    if (debug)
      plano_producao.restricoes.balanco_hidrico->printMessageConstraintBroken();
  }

  if (plano_producao.restricoes.defluencia_minima->checkConstraint())
    cout << "Sem violações\n";
  else {
    cout << "Com violação\n";
    if (debug)
      plano_producao.restricoes.defluencia_minima->printMessageConstraintBroken();
  }

  if (plano_producao.restricoes.limite_variaveis->checkConstraint())
    cout << "Sem violações\n";
  else {
    cout << "Com violação\n";
    if (debug)
      plano_producao.restricoes.limite_variaveis->printMessageConstraintBroken();
  }
}

#endif
