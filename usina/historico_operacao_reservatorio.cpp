#ifndef historico_operacao_reservatorio_h
#define historico_operacao_reservatorio_h

class HistoricoOperacaoReservatorio {
  public:
    int periodo;
    double volume;
    double vazao_turbinada;
    double vazao_vertida;
    double afluencia_natural; 
    HistoricoOperacaoReservatorio();
};

HistoricoOperacaoReservatorio::HistoricoOperacaoReservatorio() {
  
}
#endif