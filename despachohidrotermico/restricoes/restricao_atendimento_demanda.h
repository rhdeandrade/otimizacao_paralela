#ifndef restricao_atendimento_demanda_h
#define restricao_atendimento_demanda_h

#include <iostream>
#include "../../usina/usina_termica.cpp"
#include "../../usina/usina_hidreletrica.cpp"
#include "../../usina/subsistema.cpp"
using namespace std;

class RestricaoAtendimentoDemanda {
  public:
    vector<UsinaTermica> termicas;
    vector<UsinaHidreletrica> hidreletricas;
    vector<Subsistema> subsistemas;
    string errors;

    RestricaoAtendimentoDemanda(vector<Subsistema> subsistemas, vector<UsinaHidreletrica> hidreletricas, vector<UsinaTermica> termicas);
    RestricaoAtendimentoDemanda();

    bool checkConstraint();
    double errorThreshold();
    void printMessageConstraintBroken();
};

#endif
