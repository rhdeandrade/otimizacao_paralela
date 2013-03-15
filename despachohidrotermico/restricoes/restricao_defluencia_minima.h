#ifndef restricao_defluencia_minima_h
#define restricao_defluencia_minima_h

#include <iostream>
#include "../../usina/usina_hidreletrica.cpp"

using namespace std;

class RestricaoDefluenciaMinima {
  public:
    vector<UsinaHidreletrica> hidreletricas;
    string errors;

    RestricaoDefluenciaMinima(vector<UsinaHidreletrica> hidreletricas);
    RestricaoDefluenciaMinima();
    bool checkConstraint();
    double errorThreshold();
    void printMessageConstraintBroken();
};

#endif
