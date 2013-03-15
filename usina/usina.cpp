#ifndef usina_h
#define usina_h

#include <iostream>
#include "geracao_energia.cpp"

using namespace std;

class Usina {
  public:
    int id_usina;
    int id_subsistema;
    vector<GeracaoEnergia> geracoes;

    GeracaoEnergia* obter_geracao_energia(int periodo);
    double total_energia_gerada();

};

double Usina::total_energia_gerada() {
  double total;

  for (int i = 0; i < this->geracoes.size(); ++i) {
    total += this->geracoes.at(i).quantidade;
  }

  return total;
}

GeracaoEnergia* Usina::obter_geracao_energia(int periodo) {
  for (int i = 0; i < geracoes.size(); i++)
  { 
    if(geracoes.at(i).periodo == periodo) {
      return &geracoes.at(i);
    }
  }

  GeracaoEnergia *g = new GeracaoEnergia();

  g->periodo = periodo;
  g->quantidade = 0;

  return g;
}
#endif