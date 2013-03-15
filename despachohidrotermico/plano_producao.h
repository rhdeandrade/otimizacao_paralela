#ifndef plano_producao_h
#define plano_producao_h

using namespace std;
using namespace boost;

#include "restricoes/restricao.h"

class PlanoProducao {
  public:
    vector<UsinaTermica> termicas;
    vector<UsinaHidreletrica> hidreletricas;
    vector<Subsistema> subsistemas;
    //OtimizacaoDespachoHidrotermicoGlobals* globals;
    Restricao restricoes;
    
    PlanoProducao(PlanoProducao *p);
    PlanoProducao();
    
    PlanoProducao perturbation(int operacao_atomica, int counter);
    PlanoProducao executar(PlanoProducao p, int counter);
    double planejar_maximizacao_energia_hidraulica(vector<UsinaHidreletrica> h, int periodo);
    double minimizar_energia_termica(vector<UsinaTermica> t, int periodo, double* energia_termica_sobrando);
    void ativarRestricoes(bool balancoHidrico, bool atendimentoDemanda, bool defluenciaMinima, bool limiteVariaveis);
    double produzir_energia_hidraulica(vector<UsinaHidreletrica> hidreletricas, int periodo, double total_energia_termica);
    double calcularValorPresente(int periodo);
    double objectiveFunctionValue();
};



#endif
