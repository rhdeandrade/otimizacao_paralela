#ifndef otimizacao_despacho_hidrotermico_globals_cpp
#define otimizacao_despacho_hidrotermico_globals_cpp

#include "otimizacao_despacho_hidrotermico_globals.h"

OtimizacaoDespachoHidrotermicoGlobals* OtimizacaoDespachoHidrotermicoGlobals::instance;

OtimizacaoDespachoHidrotermicoGlobals::OtimizacaoDespachoHidrotermicoGlobals() {
  cascata74.push_back(74);
  cascata74.push_back(76);
  cascata74.push_back(77);
  cascata74.push_back(71);
  cascata74.push_back(72);
  cascata74.push_back(73);
  cascata74.push_back(77);
  cascata74.push_back(78);
  cascata74.push_back(82);
}


OtimizacaoDespachoHidrotermicoGlobals* OtimizacaoDespachoHidrotermicoGlobals::get_instance() {
  if (instance == NULL) {
    instance = new OtimizacaoDespachoHidrotermicoGlobals();
  }
  return instance;
}


void OtimizacaoDespachoHidrotermicoGlobals::atualizar_plano_producao(PlanoProducao plano_producao) {
  for (int i = 1; i <= 60; i++) {
    for (int j = 0; j < plano_producao.subsistemas.size(); j++) {
      
      double total_geracao_hidreletricas = 0;
      double total_geracao_termicas = 0;
      double total_intercambio = 0;
      
      vector<UsinaTermica> termicas = OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_termicas(plano_producao.termicas, plano_producao.subsistemas.at(j).id_subsistema);

      for (int k = 0; k < termicas.size(); k++) {

        GeracaoEnergia *geracao = termicas.at(k).obter_geracao_energia(i);
        total_geracao_termicas += geracao->quantidade;

      }

      vector<UsinaHidreletrica> hidreletricas = OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_hidreletricas(plano_producao.hidreletricas, plano_producao.subsistemas.at(j).id_subsistema);

      for (int k = 0; k < hidreletricas.size(); k++) {
        hidreletricas.at(k).atualizar_balanco_hidrico(OtimizacaoDespachoHidrotermicoGlobals::get_instance()->cascata74, OtimizacaoDespachoHidrotermicoGlobals::get_instance()->hidreletricas, i);
        GeracaoEnergia* geracao = hidreletricas.at(k).obter_geracao_energia(i);
        total_geracao_hidreletricas += geracao->quantidade;
      }

      Intercambio* intercambio = plano_producao.subsistemas.at(j).obter_intercambio_energia(i);
      double total_enviado = intercambio->total_energia_enviada();

      double total_recebido = 0.0;

      for (int k = 0; k < plano_producao.subsistemas.size(); k++) {
        Intercambio* inter = plano_producao.subsistemas.at(k).obter_intercambio_energia(i);
        total_recebido += inter->total_energia_recebida(plano_producao.subsistemas.at(j).id_subsistema);
      }

      total_intercambio = total_recebido - total_enviado;

      DemandaEnergia* demanda = plano_producao.subsistemas.at(j).obter_demanda_energia(i);

      Deficit* deficit = plano_producao.subsistemas.at(j).obter_deficit_subsistema(i);

      double resultado = total_geracao_termicas + total_geracao_hidreletricas + total_intercambio;

      if (demanda) {
        resultado = demanda->quantidade - resultado;
      }

      if (deficit)      
        if (resultado > 0) {
          deficit->deficit = resultado;
        }
        else {
          deficit->deficit = 0;
        }
    }
  }

}

vector<UsinaTermica> OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_termicas(vector<UsinaTermica> t, int id_subsistema) {
  vector<UsinaTermica> termicas;

  for (int i = 0; i < t.size(); i++) {
    if (t.at(i).id_subsistema == id_subsistema) {
      termicas.push_back(t.at(i));
    }
  }

 return termicas;
}

vector<UsinaHidreletrica> OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_hidreletricas(vector<UsinaHidreletrica> h, int id_subsistema) {
  vector<UsinaHidreletrica> hidreletricas;

  for (int i = 0; i < h.size(); i++) {
    if (h.at(i).id_subsistema == id_subsistema) {
      hidreletricas.push_back(h.at(i));
    }
  }

 return hidreletricas;
}

vector<UsinaHidreletrica> OtimizacaoDespachoHidrotermicoGlobals::ordernar_hidreletricas_tamanho_reservatorio(vector<UsinaHidreletrica> h, bool com_jusantes) {
  
  for (int i = 1; i < h.size(); i++) {
    int j = i;
    while((h.at(j).reservatorio.obter_tamanho() > h.at(j - 1).reservatorio.obter_tamanho())) {

      UsinaHidreletrica* aux = &h.at(j);
      UsinaHidreletrica* usina_j = &h.at(j);
      UsinaHidreletrica* usina_j_1 = &h.at(j-1);

      *usina_j = *usina_j_1;
      *usina_j_1 = *aux;
      j--;

      if (j == 0) {
        break;
      }

    }
  }
  if (!com_jusantes) 
    return h;

  return h;
}


vector<UsinaTermica> OtimizacaoDespachoHidrotermicoGlobals::obter_termicas_com_prioridade_desativacao(vector<UsinaTermica> t, int periodo) {
  vector<UsinaTermica> urgente;
  vector<UsinaTermica> normal;

  t = OtimizacaoDespachoHidrotermicoGlobals::ordenar_termicas_por_custo(t, periodo);

  for (int i = 0; i < t.size(); i++) {
    if (find(t.at(i).periodos_desativacao_obrigatorio.begin(), t.at(i).periodos_desativacao_obrigatorio.end(), periodo) != t.at(i).periodos_desativacao_obrigatorio.end()) {
      urgente.push_back(t.at(i));
    }
    else {
      normal.push_back(t.at(i));
    }
  }

  urgente.insert(urgente.end(), normal.begin(), normal.end());

  return urgente;
}

vector<UsinaTermica> OtimizacaoDespachoHidrotermicoGlobals::ordenar_termicas_por_custo(vector<UsinaTermica> t, int periodo) {
  for (int i = 1; i < t.size(); i++) {
    int j = i;
    while(t.at(j).custo_termica_mega_watt_medio(periodo) < t.at(j - 1).custo_termica_mega_watt_medio(periodo)) {
      UsinaTermica* aux = &t.at(j);
      UsinaTermica* usina_j = &t.at(j);
      UsinaTermica* usina_j_1 = &t.at(j-1);
      *usina_j = *usina_j_1;
      *usina_j_1 = *aux;
      j--;

      if(j == 0) {
        break;
      }
    }
  }

  return t;
}

#endif