#ifndef carregador_dados_h
#define carregador_dados_h

#include <boost/algorithm/string.hpp>
#include <boost/lexical_cast.hpp>
#include <string>
#include <iostream>
#include "../util/file_handler.cpp"
#include "../usina/usina_termica.cpp"
#include "../usina/usina_hidreletrica.cpp"
#include "../usina/subsistema.cpp"

using namespace std;
using namespace boost;

class CarregadorDados {
  public:
    string arquivoDadosTermicas;
    string arquivoGeracoesTermicas;
    string arquivoDadosHidreletricas;
    string arquivoGeracoesHidreletricas;
    string arquivoDadosSubsistemas;
    string arquivoDeficitsSubsistemas;
    string arquivoDemandasSubsistemas;
    string arquivoIntercambiosSubsistemas;

    CarregadorDados();

    void carregar_nome_arquivos(string arquivoDadosTermicas, string arquivoGeracoesTermicas, 
                    string arquivoDadosHidreletricas, string arquivoGeracoesHidreletricas,
                    string arquivoDadosSubsistemas, string arquivoDeficitsSubsistemas,
                    string arquivoDemandasSubsistemas, string arquivoIntercambiosSubsistemas); //Realiza função do construtor do CarregadorDados.php

    vector<UsinaTermica> carregar_usinas_termicas();

    vector<GeracaoEnergia> carregar_geracoes_usinas_termicas(int id_usina, string arquivo);

    vector<UsinaHidreletrica> carregar_usinas_hidreletricas();

    UsinaHidreletrica carregar_historico_operacao_reservatorio(UsinaHidreletrica usina);

    void carregar_montantes(vector<UsinaHidreletrica> *usinas);

    vector<Subsistema> carregar_subsistema();
    vector<Deficit> carregar_deficits_subsistemas(int id_subsistema);
    vector<DemandaEnergia> carregar_demandas_subsistema(int id_subsistema);
    vector<Intercambio> carregar_intercambios_subsistema(int id_subsistema);

};

CarregadorDados::CarregadorDados() {
}


void CarregadorDados::carregar_nome_arquivos(string arquivoDadosTermicas, string arquivoGeracoesTermicas, 
                    string arquivoDadosHidreletricas, string arquivoGeracoesHidreletricas,
                    string arquivoDadosSubsistemas, string arquivoDeficitsSubsistemas,
                    string arquivoDemandasSubsistemas, string arquivoIntercambiosSubsistemas) {

  this->arquivoDadosTermicas = arquivoDadosTermicas;
  this->arquivoGeracoesTermicas = arquivoGeracoesTermicas;
  this->arquivoDadosHidreletricas = arquivoDadosHidreletricas;
  this->arquivoGeracoesHidreletricas = arquivoGeracoesHidreletricas;
  this->arquivoDadosSubsistemas = arquivoDadosSubsistemas;
  this->arquivoDeficitsSubsistemas = arquivoDeficitsSubsistemas;
  this->arquivoDemandasSubsistemas = arquivoDemandasSubsistemas;
  this->arquivoIntercambiosSubsistemas = arquivoIntercambiosSubsistemas;

}

vector<UsinaTermica> CarregadorDados::carregar_usinas_termicas() {
  FileHandler file_handler;
  string delimitador(" ");
  vector<UsinaTermica> termicas;

  vector<string> usinas_termicas = file_handler.open_file(this->arquivoDadosTermicas);
  int i;
  

  for(i = 0; i < usinas_termicas.size(); i++) {
    vector<string> tokens;
    UsinaTermica usina_termica;

    string value(usinas_termicas.at(i).data());
        
    split(tokens, value, is_any_of(delimitador));

    usina_termica.id_usina = (int) lexical_cast<double>(tokens.at(0).data());
    
    usina_termica.quantidade_geracao_max = lexical_cast<double>(tokens.at(1).data());
    
    usina_termica.quantidade_geracao_min = lexical_cast<double>(tokens.at(2).data());
    
    usina_termica.coeficiente_custo_termica_a0 = lexical_cast<double>(tokens.at(3).data());
    
    usina_termica.coeficiente_custo_termica_a1 = lexical_cast<double>(tokens.at(4).data());
    
    usina_termica.coeficiente_custo_termica_a2 = lexical_cast<double>(tokens.at(5).data());
    
    usina_termica.id_subsistema = (int) lexical_cast<double>(tokens.at(6).data());

    //usina_termica.gerações
    usina_termica.geracoes = this->carregar_geracoes_usinas_termicas(usina_termica.id_usina, this->arquivoGeracoesTermicas);

    termicas.push_back(usina_termica);
    
  }

  return termicas;
}

vector<GeracaoEnergia> CarregadorDados::carregar_geracoes_usinas_termicas(int id_usina, string arquivo) {
  FileHandler file_handler;
  vector<GeracaoEnergia> geracoes;
  vector<string> dados_arquivo = file_handler.open_file(arquivo);
  string delimitador(" ");
  
  int i = 0;
  while(i <= dados_arquivo.size()) {
    vector<string> tokens;
    string value(dados_arquivo.at(i).data());
    split(tokens, value, is_any_of(delimitador));

    if(((int) lexical_cast<double>(tokens.at(0).data())) == id_usina) {
      int periodo = 1;
      while (periodo <= 60) { //Esse é o número de períodos definidos na classe OtimizacaoDespachoHidrotermicoGlobals do pj PHP
        i++;
        string value(dados_arquivo.at(i).data());
        split(tokens, value, is_any_of(delimitador));
        GeracaoEnergia geracao;
        geracao.periodo = periodo;
        periodo++;
        geracao.quantidade = lexical_cast<double>(tokens.at(0).data());

        geracoes.push_back(geracao);
      }

      return geracoes;
    }
    else {
      i += 60;
    }

    i++; //Pra mim essa linha não faz sentido, pq não fazer i += 61 na linha de cima então? Apenas copiei do cogido php

  }
  return geracoes;
}

vector<UsinaHidreletrica> CarregadorDados::carregar_usinas_hidreletricas() {
  FileHandler file_handler;
  vector<UsinaHidreletrica> usinas;
  vector<string> dados_arquivo = file_handler.open_file(this->arquivoDadosHidreletricas);

  vector<string> tokens;
  string delemitador(" ");

  for (int i = 0; i < dados_arquivo.size(); i++) {
    string linha = dados_arquivo.at(i).data();
    split(tokens, linha, is_any_of(delemitador));

    UsinaHidreletrica usina;
    usina.id_usina = (int) lexical_cast<double>(tokens.at(0).data());
    usina.jusante = (int) lexical_cast<double>(tokens.at(1).data());
    usina.reservatorio.volume_maximo = lexical_cast<double>(tokens.at(2).data());
    usina.reservatorio.volume_minimo = lexical_cast<double>(tokens.at(3).data());
    
    usina.coeficiente_cota_montante_a0 = lexical_cast<double>(tokens.at(4).data());
    usina.coeficiente_cota_montante_a1 = lexical_cast<double>(tokens.at(5).data());
    usina.coeficiente_cota_montante_a2 = lexical_cast<double>(tokens.at(6).data());
    usina.coeficiente_cota_montante_a3 = lexical_cast<double>(tokens.at(7).data());
    usina.coeficiente_cota_montante_a4 = lexical_cast<double>(tokens.at(8).data());

    usina.coeficiente_cota_jusante_a0 = lexical_cast<double>(tokens.at(9).data());
    usina.coeficiente_cota_jusante_a1 = lexical_cast<double>(tokens.at(10).data());
    usina.coeficiente_cota_jusante_a2 = lexical_cast<double>(tokens.at(11).data());
    usina.coeficiente_cota_jusante_a3 = lexical_cast<double>(tokens.at(12).data());
    usina.coeficiente_cota_jusante_a4 = lexical_cast<double>(tokens.at(13).data());

    usina.tipo_perda_hidraulica = lexical_cast<double>(tokens.at(14).data());
    usina.valor_perda_hidraulica = lexical_cast<double>(tokens.at(15).data());
    usina.potencia_efetiva = lexical_cast<double>(tokens.at(16).data());
    usina.produtividade_media = lexical_cast<double>(tokens.at(17).data());
    usina.id_subsistema = (int) lexical_cast<double>(tokens.at(18).data());
    
    usina.reservatorio.defluencia_minima = lexical_cast<double>(tokens.at(19).data());
    usina.reservatorio.maximo_vazao_turbinada = lexical_cast<double>(tokens.at(20).data());

    usina = carregar_historico_operacao_reservatorio(usina);
    
    usinas.push_back(usina);

  }

  carregar_montantes(&usinas);



  return usinas;

}

UsinaHidreletrica CarregadorDados::carregar_historico_operacao_reservatorio(UsinaHidreletrica usina) {
  FileHandler file_handler;

  vector<string> dados_arquivo = file_handler.open_file(arquivoGeracoesHidreletricas);
  
  vector<string> tokens;
  string delimitador(" ");

  int i = 0;
  while(i < dados_arquivo.size()) {
    string value = dados_arquivo.at(i).data();
    split(tokens, value, is_any_of(delimitador));

    if (((int)lexical_cast<double>(tokens.at(0).data())) == usina.id_usina) {
      int periodo = 1;
      while(periodo <= 60) {
        i++;
        value = dados_arquivo.at(i).data();
        split(tokens, value, is_any_of(delimitador));
        HistoricoOperacaoReservatorio historico;

        historico.periodo = periodo;
        historico.volume = lexical_cast<double>(tokens.at(0).data());
        historico.vazao_turbinada = lexical_cast<double>(tokens.at(1).data());
        historico.vazao_vertida = lexical_cast<double>(tokens.at(2).data());
        historico.afluencia_natural = lexical_cast<double>(tokens.at(4).data());
        usina.reservatorio.historico.push_back(historico);

        GeracaoEnergia geracao;
        geracao.periodo = periodo;
        geracao.quantidade = lexical_cast<double>(tokens.at(3).data());
        usina.geracoes.push_back(geracao);

        periodo++;
      }
      return usina;
      
    }
    else {
      i = i + 60;
    } 
    i++;
  }
  return usina;

}

void CarregadorDados::carregar_montantes(vector<UsinaHidreletrica> *usinas) {
  for (int i = 0; i < usinas->size(); i++) {
    for (int j = 0; j < usinas->size(); j++) {
      if(usinas->at(i).jusante == usinas->at(j).id_usina) {
        usinas->at(j).montantes.push_back(usinas->at(i).id_usina);
      }
    }
  }

}

vector<Subsistema> CarregadorDados::carregar_subsistema() {
  vector<Subsistema> subsistemas;
  FileHandler file_handler;

  vector<string> dados_arquivo = file_handler.open_file(arquivoDadosSubsistemas);
  vector<string> tokens;
  string delimitador(" ");

  for(int i = 0; i < dados_arquivo.size(); i++) {
    string value = dados_arquivo.at(i).data();
    split(tokens, value, is_any_of(delimitador));

    Subsistema subsistema;
    subsistema.id_subsistema = (int) lexical_cast<double>(tokens.at(0).data());
    subsistema.coeficiente_custo_deficit_a0 = lexical_cast<double>(tokens.at(1).data());
    subsistema.coeficiente_custo_deficit_a1 = lexical_cast<double>(tokens.at(2).data());
    subsistema.coeficiente_custo_deficit_a2 = lexical_cast<double>(tokens.at(3).data());
    subsistema.demanda = lexical_cast<double>(tokens.at(4).data());
    subsistema.intercambio_minimo = lexical_cast<double>(tokens.at(5).data());
    subsistema.intercambio_maximo = lexical_cast<double>(tokens.at(6).data());
    subsistema.deficits = carregar_deficits_subsistemas(subsistema.id_subsistema);
    subsistema.demandas = carregar_demandas_subsistema(subsistema.id_subsistema);
    subsistema.intercambios = carregar_intercambios_subsistema(subsistema.id_subsistema);

    subsistemas.push_back(subsistema);

  }



  return subsistemas;

}

vector<Deficit> CarregadorDados::carregar_deficits_subsistemas(int id_subsistema) {
  vector<Deficit> deficits;
  FileHandler file_handler;

  vector<string> dados_arquivo = file_handler.open_file(arquivoDeficitsSubsistemas);
  vector<string> tokens;
  string delimitador(" ");

  int posicao_susbsistema = id_subsistema - 1;

  for(int i = 0; i < dados_arquivo.size(); i++) {
    string value = dados_arquivo.at(i).data();
    split(tokens, value, is_any_of(delimitador));

    if (tokens.size() == 5) {

      Deficit deficit;
      deficit.periodo = ++i;
      deficit.deficit = lexical_cast<double>(tokens.at(posicao_susbsistema).data());

      deficits.push_back(deficit);

    }

  }

  return deficits;
}

vector<DemandaEnergia> CarregadorDados::carregar_demandas_subsistema(int id_subsistema) {

  vector<DemandaEnergia> demandas;
  FileHandler file_handler;

  vector<string> dados_arquivo = file_handler.open_file(arquivoDemandasSubsistemas);
  vector<string> tokens;
  string delimitador(" ");

  int posicao_susbsistema = id_subsistema - 1;

  for(int i = 0; i < dados_arquivo.size(); i++) {
    string value = dados_arquivo.at(i).data();
    split(tokens, value, is_any_of(delimitador));

    if (tokens.size() == 5) {

      DemandaEnergia demanda;
      demanda.periodo = ++i;

      demanda.quantidade = lexical_cast<double>(tokens.at(posicao_susbsistema).data());

      demandas.push_back(demanda);

    }

  }

  return demandas;
}

vector<Intercambio> CarregadorDados::carregar_intercambios_subsistema(int id_subsistema) {

  vector<Intercambio> intercambios;
  FileHandler file_handler;

  vector<string> dados_arquivo = file_handler.open_file(arquivoIntercambiosSubsistemas);
  vector<string> tokens;
  string delimitador(" ");

  int posicao_susbsistema = id_subsistema - 1;

  for(int i = 0; i < dados_arquivo.size(); i++) {
    string value = dados_arquivo.at(i).data();
    split(tokens, value, is_any_of(delimitador));

    if (tokens.size() == 5) {

      Intercambio intercambio;
      intercambio.periodo = ++i;
      intercambio.quantidade_subsistema_1 = lexical_cast<double>(tokens.at(0).data());
      intercambio.quantidade_subsistema_2 = lexical_cast<double>(tokens.at(1).data());
      intercambio.quantidade_subsistema_3 = lexical_cast<double>(tokens.at(2).data());
      intercambio.quantidade_subsistema_4 = lexical_cast<double>(tokens.at(3).data());
      intercambio.quantidade_subsistema_5 = lexical_cast<double>(tokens.at(4).data());

      intercambios.push_back(intercambio);

    }

  }

  return intercambios;

}

#endif