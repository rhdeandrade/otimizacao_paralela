#ifndef conversor_h
#define conversor_h
#include <iostream>
#include <math.h>


using namespace std;

class Conversor {
public:
  double hectometro_metro_cubico(double valor, int periodo);
  double quantidade_segundos_mes(int periodo);
  double quantidade_horas_mes(int periodo);
  double quantidade_dias_mes(int periodo);
  double metro_cubico_hectometro(double valor, int periodo);
  int mes_corrente(int periodo);

};

double Conversor::hectometro_metro_cubico(double valor, int periodo) {
  return valor * (pow(10.0, 6.0) / quantidade_segundos_mes(periodo));
}

double Conversor::metro_cubico_hectometro(double valor, int periodo) {
  return valor / (pow(10.0, 6.0) / quantidade_segundos_mes(periodo));
}

double Conversor::quantidade_segundos_mes(int periodo) {
  return 3600 * quantidade_horas_mes(periodo);
}

double Conversor::quantidade_horas_mes(int periodo) {
  return 24 * quantidade_dias_mes(periodo);
}

double Conversor::quantidade_dias_mes(int periodo) {
  int mes_atual = mes_corrente(periodo);

  if (mes_atual == 1)
    return 31.0;
  else if (mes_atual == 2){
    time_t t = time(0);   // get time now
    struct tm * now = localtime( & t );
    if (((now->tm_year + 1900) % 4) == 0)
      return 29.0;
    return 28.0;
  }
  else if (mes_atual == 3)
    return 31.0;
  else if (mes_atual == 4)
    return 30.0;
  else if (mes_atual == 5)
    return 31.0;
  else if (mes_atual == 6)
    return 30.0;
  else if (mes_atual == 7)
    return 31.0;
  else if (mes_atual == 8)
    return 31.0;
  else if (mes_atual == 9)
    return 30.0;
  else if (mes_atual == 10)
    return 31.0;
  else if (mes_atual == 11)
    return 30.0;
  else
    return 31.0;



}

int Conversor::mes_corrente(int periodo) {
  int mod = periodo % 12;

  if(mod == 0) {
    return 12;
  }
  else {
    return mod;
  }
}


#endif