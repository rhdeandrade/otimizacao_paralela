#ifndef intercambio_h
#define intercambio_h

#include <iostream>

using namespace std;

class Intercambio {
public:
  int periodo;
  double quantidade_subsistema_1;
  double quantidade_subsistema_2;
  double quantidade_subsistema_3;
  double quantidade_subsistema_4;
  double quantidade_subsistema_5;

  double total_energia_enviada();
  double total_energia_recebida(int id_subsistema);
};

double Intercambio::total_energia_enviada() {
  double resultado = this->quantidade_subsistema_1 + this->quantidade_subsistema_2 + this->quantidade_subsistema_3 + this->quantidade_subsistema_4 + this->quantidade_subsistema_5;
  return resultado;
}

double Intercambio::total_energia_recebida(int id_subsistema) {
  if (id_subsistema == 1)
    return  this->quantidade_subsistema_1;
  else if (id_subsistema == 2)
    return this->quantidade_subsistema_2;
  else if (id_subsistema == 3)
    return this->quantidade_subsistema_3;
  else if (id_subsistema == 4)
    return this->quantidade_subsistema_4;
  else if (id_subsistema == 5)
    return this->quantidade_subsistema_5;

  return 0.0;
}


#endif