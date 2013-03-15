#ifndef restricao_atendimento_demanda_cpp
#define restricao_atendimento_demanda_cpp

#include "restricao_atendimento_demanda.h"

RestricaoAtendimentoDemanda::RestricaoAtendimentoDemanda() {
}

RestricaoAtendimentoDemanda::RestricaoAtendimentoDemanda(vector<Subsistema> subsistemas, vector<UsinaHidreletrica> hidreletricas, vector<UsinaTermica> termicas) {
  this->termicas = termicas;
  this->hidreletricas = hidreletricas;
  this->subsistemas = subsistemas;
}

bool RestricaoAtendimentoDemanda::checkConstraint() {
  cout << "Validando Atendimento Demanda... ";
    bool constraintOk = true;
    errors = "";

    // passa por todos o periodos
    for(int i = 1; i <= OtimizacaoDespachoHidrotermicoGlobals::NUM_PERIODOS; i++)
    {
      // passa por todos os subsistemas
      for(int j = 0; j < this->subsistemas.size(); j++)
      {
        Subsistema subsistema = this->subsistemas[j];
        double totalGeracaoHidreletricas = 0;
        double totalGeracaoTermicas = 0;
        double totalIntercambio = 0;
        
        // obtem as termicas do subsistema
        termicas = OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_termicas(this->termicas, subsistema.id_subsistema);
        // passa apenas pelas termicas do subsistema
        for(int k = 0; k < this->termicas.size(); k++)
        {
          UsinaTermica termica = this->termicas[k];
          GeracaoEnergia* geracao = termica.obter_geracao_energia(i);
          totalGeracaoTermicas = totalGeracaoTermicas + geracao->quantidade;
        }
        
        // obtem as hidreletricas do subsistema
        hidreletricas = OtimizacaoDespachoHidrotermicoGlobals::obter_usinas_hidreletricas(this->hidreletricas, subsistema.id_subsistema); 
        for (int l = 0; l < this->hidreletricas.size(); l++) 
        {
          UsinaHidreletrica hidreletrica = this->hidreletricas[l];
          GeracaoEnergia* geracao = hidreletrica.obter_geracao_energia(i);
          totalGeracaoHidreletricas = totalGeracaoHidreletricas + geracao->quantidade;
        }
        
        // obtem o total de intercambio enviado a outros subsistemas
        Intercambio* intercambio = subsistema.obter_intercambio_energia(i);
        double totalEnviado = intercambio->total_energia_enviada();
          
        // obtem o total de intercambio recebido dos outros subsistemas
        double totalRecebido = 0;
        for (int m = 0; m < this->subsistemas.size(); m++) 
        {
          Subsistema ss = subsistemas[m];
          Intercambio* intercambio = ss.obter_intercambio_energia(i);
          totalRecebido = totalRecebido + intercambio->total_energia_recebida(subsistema.id_subsistema);
        }
        
        totalIntercambio = totalRecebido - totalEnviado;
         
        DemandaEnergia* demanda = subsistema.obter_demanda_energia(i);
        Deficit* deficit = subsistema.obter_deficit_subsistema(i);
        
        double result = 0;
        result = totalGeracaoTermicas + totalGeracaoHidreletricas;
        result = result + totalIntercambio;
        if (demanda)
          result = result - demanda->quantidade;
        if (deficit)
          result = result + deficit->deficit;
        
        // quebra a restricao apenas se a quantidade de energia for menor do que a demanda
        if((__gnu_cxx::abs(result) > this->errorThreshold()) && (result < -1))
        {
        /*
          $obj= new stdClass();
          $obj->periodo = $i;
          $obj->id_subsistema = $subsistema->idSubsistema; 
          $obj->totalTermica = $totalGeracaoTermicas;
          $obj->totalHidreletrica = $totalGeracaoHidreletricas;
          $obj->totalIntercambio = $totalIntercambio;
          $obj->totalEnergiaProduzida = bcadd(bcadd($totalGeracaoTermicas, $totalGeracaoHidreletricas), $totalIntercambio); 
          $obj->demanda = $demanda->quantidade;
          $obj->deficit = $deficit->deficit;
          $obj->erroAbsoluto = abs($result);
          */
          this->errors.append("Periodo com quantidade de energia produzida menor que demanda\n");
          constraintOk = false;
        }
      }
    }

    return constraintOk;
}

double RestricaoAtendimentoDemanda::errorThreshold() {
  return 1;
} 

void RestricaoAtendimentoDemanda::printMessageConstraintBroken() {
  cout << errors;
}
#endif
