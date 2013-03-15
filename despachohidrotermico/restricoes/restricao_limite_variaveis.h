#ifndef restricao_limite_variaveis_h
#define restricao_limite_variaveis_h

#include <iostream>
#include "../../usina/usina_termica.cpp"
#include "../../usina/usina_hidreletrica.cpp"

using namespace std;

class RestricaoLimiteVariaveis {
  public:
    vector<UsinaHidreletrica> hidreletricas;
    vector<UsinaTermica> termicas;
    string errors;

    RestricaoLimiteVariaveis(vector<UsinaHidreletrica> hidreletricas, vector<UsinaTermica> termicas);
    RestricaoLimiteVariaveis();
    bool checkConstraint();
    double errorThreshold();
    bool capacidadeArmazenamentoReservatorio(UsinaHidreletrica hidreletrica, int periodo);
    bool capacidadeVazaoTurbinadaReservatorio(UsinaHidreletrica hidreletrica, int periodo);
    bool capacidadeVazaoVertidaReservatorio(UsinaHidreletrica hidreletrica, int periodo);
    bool capacidadeGeracaoEnergiaTermica(UsinaTermica termica, int periodo);
    void printMessageConstraintBroken();
};
#endif
