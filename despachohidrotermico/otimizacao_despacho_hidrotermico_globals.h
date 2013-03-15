#ifndef otimizacao_global_h
#define otimizacao_global_h

#include <iostream>
#include "plano_producao.h"

using namespace std;

class OtimizacaoDespachoHidrotermicoGlobals {

public:
  const static int NUM_PERIODOS = 60;
  const static double TAXA_DESCONTO = 0.00948879;
  const static int LIMIAR_ERRO_BALANCO_HIDRICO = 16;
  static OtimizacaoDespachoHidrotermicoGlobals* get_instance();
  static void atualizar_plano_producao(PlanoProducao plano_producao);

  vector<int> cascata74;
  vector<UsinaHidreletrica> hidreletricas;

  static vector<UsinaTermica> obter_usinas_termicas(vector<UsinaTermica> t, int id_subsistema);
  
  static vector<UsinaHidreletrica> obter_usinas_hidreletricas(vector<UsinaHidreletrica> h, int id_subsistema);

  static vector<UsinaHidreletrica> ordernar_hidreletricas_tamanho_reservatorio(vector<UsinaHidreletrica> h, bool com_jusantes);

  static vector<UsinaTermica> obter_termicas_com_prioridade_desativacao(vector<UsinaTermica> t, int periodo);

  static vector<UsinaTermica> ordenar_termicas_por_custo(vector<UsinaTermica> t, int periodo);

private:
  OtimizacaoDespachoHidrotermicoGlobals();
  static OtimizacaoDespachoHidrotermicoGlobals* instance;

};



#endif