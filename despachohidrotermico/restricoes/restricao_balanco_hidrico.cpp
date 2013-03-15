#ifndef restricao_balanco_hidrico_cpp
#define restricao_balanco_hidrico_cpp

#include "restricao_balanco_hidrico.h"

RestricaoBalancoHidrico::RestricaoBalancoHidrico(vector<UsinaHidreletrica> hidreletricas) {
  this->hidreletricas = hidreletricas;
}

bool RestricaoBalancoHidrico::checkConstraint() {
  cout << "Validando Balanco Hidrico... ";
  bool constraintOk = true;
  errors = "";
  // o balanco hidrico começa no periodo dois porque utiliza valores do periodo anterior
  for(int i = 1; i <= OtimizacaoDespachoHidrotermicoGlobals::NUM_PERIODOS; i++)
  {
    for (int l = 0; l < this->hidreletricas.size(); l++) 
    {
      UsinaHidreletrica hidreletrica = this->hidreletricas[l];

      // volume atual do reservatorio
      HistoricoOperacaoReservatorio* historicoOperacao = hidreletrica.reservatorio.obter_historico_reservatorio(i, hidreletrica.reservatorio.volume_maximo);

      // obtem o volume anterior do reservatorio.
      HistoricoOperacaoReservatorio* historicoOperacaoAnterior = hidreletrica.reservatorio.obter_historico_reservatorio(i - 1, hidreletrica.reservatorio.volume_maximo);

      // converte os volumes para de hectometro cubico para metro cubico por segundo
      Conversor c;
      double volume = c.hectometro_metro_cubico(historicoOperacao->volume, i);
      double volumeAnterior = c.hectometro_metro_cubico(historicoOperacaoAnterior->volume, i);

      double vazao_total = hidreletrica.carregar_vazao_montante(this->hidreletricas, i);
      double afluencia_natural = hidreletrica.carregar_afluencia_montante(this->hidreletricas, i);

      //TODO: caso especial da usina 73
      if(hidreletrica.id_usina == 73)
        afluencia_natural = 0;

      double result = (volume - (((((volumeAnterior + vazao_total) - historicoOperacao->vazao_turbinada) - historicoOperacao->vazao_vertida) + historicoOperacao->afluencia_natural) - afluencia_natural));
      if(__gnu_cxx::abs(result) > this->errorThreshold())
      {
        /*
        $obj = new stdClass();
        $obj->id_usina = $hidreletrica->idUsina;
        $obj->periodo = $i;
        $obj->volume = $volume;
        $obj->volumeAnterior = $volumeAnterior;
        $obj->vazaoMontante = $montantes->vazaoTotal;
        $obj->vazao_turbinada = $historicoOperacao->vazaoTurbinada;
        $obj->vazao_vertida = $historicoOperacao->vazaoVertida;
        $obj->afluencia_natural = $historicoOperacao->afluenciaNatural;
        $obj->afluenciaMontante = $montantes->afluencia_natural;
        $obj->totalErro = bcsub($volume, bcsub(bcadd(bcsub(bcsub(bcadd($volumeAnterior, $montantes->vazaoTotal), $historicoOperacao->vazao_turbinada), $historicoOperacao->vazao_vertida), $historicoOperacao->afluencia_natural), $montantes->afluenciaNatural));
        $this->error[] = $obj; 
        */
        this->errors.append("Erro no balanco hidrico\n");
        constraintOk = false;
      }
    }
  }

  return constraintOk;
}

double RestricaoBalancoHidrico::errorThreshold()
{
  return OtimizacaoDespachoHidrotermicoGlobals::LIMIAR_ERRO_BALANCO_HIDRICO;
}

void RestricaoBalancoHidrico::printMessageConstraintBroken() {
  cout << errors;
}
#endif
