#ifndef usina_termica_h
#define usina_termica_h

#include <iostream>
#include "usina.cpp"

#include <math.h>

using namespace std;

class UsinaTermica : public Usina {
  public:
    double quantidade_geracao_max;
    double quantidade_geracao_min;
    double coeficiente_custo_termica_a2;
    double coeficiente_custo_termica_a1;
    double coeficiente_custo_termica_a0;
    double tempo_minimo_ativada;
    double tempo_minimo_desativada;
    vector<int> periodos_desativacao_obrigatorio;
    UsinaTermica();
    double custo_termica_mega_watt_medio(int periodo);
    double iniciar_processo_desativacao(int periodo);
    double status_usina(int periodo);
    double desativarUsina(int periodo, bool previsao);
    void adicionarPeriodosDesativacaoObrigatorio(int periodoBase);
    bool verificarTempoMinimoAtivacao(int periodo);
};

UsinaTermica::UsinaTermica() {
  this->coeficiente_custo_termica_a2 = 0;
  this->coeficiente_custo_termica_a0 = 0;
  this->coeficiente_custo_termica_a1 = 0;
  this->tempo_minimo_ativada = 3;
  this->tempo_minimo_desativada = 3;
}

double UsinaTermica::status_usina(int periodo) {
  GeracaoEnergia* geracao = this->obter_geracao_energia(periodo);

  if (geracao->quantidade == 0) {
    return 0;
  }
  if (geracao->quantidade == this->quantidade_geracao_min) {
    return 0; 
  }
  else {
    double resultado = geracao->quantidade * 100;
    resultado /= this->quantidade_geracao_max;
    return resultado;
  }
}

double UsinaTermica::custo_termica_mega_watt_medio(int periodo) {
  GeracaoEnergia* g = this->obter_geracao_energia(periodo);

  double resultado = pow(g->quantidade, 2);

  resultado *= this->coeficiente_custo_termica_a2;
  resultado += this->coeficiente_custo_termica_a1 * g->quantidade;
  resultado += this->coeficiente_custo_termica_a0;
  return resultado;
}

double UsinaTermica::desativarUsina(int periodo, bool previsao = false) {
  double result = 0;
  GeracaoEnergia* geracao = this->obter_geracao_energia(periodo);

  if (!previsao) {
    result = geracao->quantidade - this->quantidade_geracao_min;
    geracao->quantidade = this->quantidade_geracao_min;
  }

  return result;
}

void UsinaTermica::adicionarPeriodosDesativacaoObrigatorio(int periodoBase) {
  for (int i = 0; i < this->periodos_desativacao_obrigatorio.size(); i++)
  {
    if (this->periodos_desativacao_obrigatorio.at(i) == periodoBase) {
      this->periodos_desativacao_obrigatorio.erase(this->periodos_desativacao_obrigatorio.begin() + i);
    }
  }

  for (int i = 1; i < (int) this->tempo_minimo_desativada; ++i)
  {
    periodoBase++;
    if (periodoBase == 60)
      return;

    this->periodos_desativacao_obrigatorio.push_back(periodoBase);
  }
}

bool UsinaTermica::verificarTempoMinimoAtivacao(int periodo) {
  for (int i = 0; i < (int) this->tempo_minimo_ativada; ++i)
  {
    periodo--;
    if (periodo == 0) {
      return false;
    }

    if (this->status_usina(periodo) == 0) {
      return false;
    }
  }

  return true;
}


double UsinaTermica::iniciar_processo_desativacao(int periodo) {
  double resultado = 0.0;
  if (find(this->periodos_desativacao_obrigatorio.begin(), this->periodos_desativacao_obrigatorio.end(), periodo) != this->periodos_desativacao_obrigatorio.end()) {
    double status = this->status_usina(periodo);
    double result = this->desativarUsina(periodo);
    double novoStatus = this->status_usina(periodo);

    if (status != novoStatus) {
      this->adicionarPeriodosDesativacaoObrigatorio(periodo); 
    }

    return result;
  }
  bool previsao;
  if (this->verificarTempoMinimoAtivacao(periodo)) {
    previsao = false;
    this->adicionarPeriodosDesativacaoObrigatorio(periodo);
  }
  else {
    previsao = true;
  }

  return this->desativarUsina(periodo, previsao); 
}

#endif