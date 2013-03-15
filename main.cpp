#include <iostream>
#include <algorithm>

#include "despachohidrotermico/otimizacao_despacho_hidrotermico.cpp"
#include "despachohidrotermico/plano_producao.h"
#include "despachohidrotermico/otimizacao_despacho_hidrotermico_globals.h"
#include "util/report.cpp"
#include <ctime>
#include <omp.h>

using namespace std;

int main() {


  OtimizacaoDespachoHidrotermico odh;
  odh.carregarDados("lote5", 10);
  odh.ativarRestricoes(true, true, true, true);

  OtimizacaoDespachoHidrotermicoGlobals::atualizar_plano_producao(odh.plano_producao);

  Report::imprimir_resultados(odh.plano_producao);

  odh.validarPlanoProducao();

  int operacao_atomica = 4;
  cout << "Executando Hill Climbing\n";
  odh.executar_hill_climbing(operacao_atomica, 60, 3);

  odh.validarPlanoProducao();

  Report::imprimir_resultados(odh.plano_producao);

  return 0;
}
