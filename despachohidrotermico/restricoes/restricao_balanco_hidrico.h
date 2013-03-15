#ifndef restricao_balanco_hidrico_h
#define restricao_balanco_hidrico_h

#include <iostream>
#include "../../usina/usina_hidreletrica.cpp"

using namespace std;

class RestricaoBalancoHidrico {
  public:
    vector<UsinaHidreletrica> hidreletricas;
    string errors;

    RestricaoBalancoHidrico(vector<UsinaHidreletrica> hidreletricas);
    bool checkConstraint();
    double errorThreshold();
    void printMessageConstraintBroken();
};

#endif
