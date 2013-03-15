#ifndef subsistema_h
#define subsistema_h

#include <iostream>
#include "demanda_energia.cpp"
#include "deficit.cpp"
#include "intercambio.cpp"

using namespace std;
using namespace boost;

class Subsistema {
  public:
    int id_subsistema;
    double coeficiente_custo_deficit_a2;
    double coeficiente_custo_deficit_a1;
    double coeficiente_custo_deficit_a0;

    double intercambio_minimo;
    double intercambio_maximo;
    double demanda;

    vector<DemandaEnergia> demandas;
    vector<Deficit> deficits;
    vector<Intercambio> intercambios;

    Intercambio* obter_intercambio_energia(int periodo);
    DemandaEnergia* obter_demanda_energia(int periodo);
    Deficit* obter_deficit_subsistema(int periodo);
    double custoDeficit(int periodo);
};

Intercambio* Subsistema::obter_intercambio_energia(int periodo) {
  for (int i = 0; i < this->intercambios.size(); i++) {
    if (this->intercambios.at(i).periodo == periodo) {
      return &this->intercambios.at(i);
    }
  }

  Intercambio* inter = new Intercambio();
  inter->periodo = 0;
  inter->quantidade_subsistema_1 = 0;
  inter->quantidade_subsistema_2 = 0;
  inter->quantidade_subsistema_3 = 0;
  inter->quantidade_subsistema_5 = 0;
  inter->quantidade_subsistema_4 = 0;
  return inter;
}

DemandaEnergia* Subsistema::obter_demanda_energia(int periodo) {
  for (int i = 0; i < this->demandas.size(); i++) {
    if (this->demandas.at(i).periodo == periodo) {
      return &this->demandas.at(i);
    }
  }

  return NULL;
}

Deficit* Subsistema::obter_deficit_subsistema(int periodo) {
  for (int i = 0; i < this->deficits.size(); i++) {
    if (this->deficits.at(i).periodo == periodo) {
      return &this->deficits.at(i);
    }
  }

  return NULL;
}


double Subsistema::custoDeficit(int periodo) {
  Deficit* deficit = obter_deficit_subsistema(periodo);

  double result = 0;
  if(deficit != NULL) {
    result = pow(deficit->deficit, 2);
    result *= this->coeficiente_custo_deficit_a2;
    result += this->coeficiente_custo_deficit_a1 * deficit->deficit;
    result += this->coeficiente_custo_deficit_a0;
  }
  return result;
}
#endif