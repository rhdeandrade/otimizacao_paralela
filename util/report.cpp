#ifndef resport_h
#define resport_h

class Report {
public:
  static void imprimir_resultados(PlanoProducao plano_producao);
};

void Report::imprimir_resultados(PlanoProducao plano_producao) {
  cout << "Funcao Objetivo\n";
  //print_funcao_objetivo

  double total_energia_hidraulica = 0;

  for (int i = 0; i < plano_producao.hidreletricas.size(); i++) {
    total_energia_hidraulica += plano_producao.hidreletricas.at(i).total_energia_gerada();
  }

  cout << "Total Energia Hidraulica: " << total_energia_hidraulica << "\n";

  double total_energia_termica = 0;
  for (int i = 0; i < plano_producao.termicas.size(); i++) {
    total_energia_termica += plano_producao.termicas.at(i).total_energia_gerada();
  }

  cout << "Total Energia Termica: " << total_energia_termica << "\n";

  double total_deficit = 0;

  for (int i = 0; i < plano_producao.subsistemas.size(); i++) {
    for (int j = 0; j < plano_producao.subsistemas.at(i).deficits.size(); j++) {
      total_deficit += plano_producao.subsistemas.at(i).deficits.at(j).deficit;
    }
  }

  cout << "Total Deficit: " << total_deficit << "\n";

}


#endif