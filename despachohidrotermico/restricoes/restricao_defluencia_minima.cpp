#ifndef restricao_defluencia_minima_cpp
#define restricao_defluencia_minima_cpp

#include "restricao_defluencia_minima.h"

RestricaoDefluenciaMinima::RestricaoDefluenciaMinima(vector<UsinaHidreletrica> hidreletricas) {
  this->hidreletricas = hidreletricas;
}

RestricaoDefluenciaMinima::RestricaoDefluenciaMinima() {
}

bool RestricaoDefluenciaMinima::checkConstraint()
{
  cout << "Validando Defluencia Minima... ";
  bool constraintOk = true;
  errors = "";

  for(int i = 1; i <= OtimizacaoDespachoHidrotermicoGlobals::NUM_PERIODOS; i++)
  {
    for (int l = 0; l < this->hidreletricas.size(); l++) 
    {
      UsinaHidreletrica hidreletrica = this->hidreletricas[l];
    
      HistoricoOperacaoReservatorio* historico = hidreletrica.reservatorio.obter_historico_reservatorio(i, hidreletrica.reservatorio.volume_maximo);

      double result = (hidreletrica.reservatorio.defluencia_minima - (historico->vazao_vertida + historico->vazao_turbinada));

      if(result > this->errorThreshold())
      {
        /*
        $obj = new stdClass();
        $obj->idUsina = $hidreletrica->idUsina;
        $obj->periodo = $i;
        $obj->vazao_vertida = $historico->vazaoVertida;
        $obj->vazao_turbinada = $historico->vazaoTurbinada;
        $obj->vazaoTotal = bcadd($historico->vazao_turbinada, $historico->vazao_vertida);
        $obj->defluencia_minima = $hidreletrica->reservatorio->defluenciaMinima;
        $obj->diferenca = bcsub($hidreletrica->reservatorio->defluencia_minima, bcadd($historico->vazao_turbinada, $historico->vazao_vertida));
        $this->error[] = $obj;
        */
        this->errors.append("Erro na defluencia minima\n");
        constraintOk = false;
      }
    }
  }

  return constraintOk;
}

double RestricaoDefluenciaMinima::errorThreshold()
{
  return 1;
}

void RestricaoDefluenciaMinima::printMessageConstraintBroken() {
  cout << errors;
}
#endif
