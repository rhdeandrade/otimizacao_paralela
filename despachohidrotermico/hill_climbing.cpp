#ifndef hill_climbing_h
#define hill_climbing_h

#include <iostream>
#include "plano_producao.cpp"
#include "../util/report.cpp"

using namespace std;

class HillClimbing {
  public:
    PlanoProducao current_state;
    PlanoProducao next_state;

    int maximum_iterations_number;
    int maximum_perturbation_number_iteration;

    HillClimbing(PlanoProducao p, int m, int mp);
    PlanoProducao execute(int operacao_atomica);
    void perturbation(int operacao_atomica, int counter);
    bool stop_main_loop(int iteracao);
    bool changeCurrenteState();
};

HillClimbing::HillClimbing(PlanoProducao p, int m, int mp) {
  this->current_state = p;
  this->maximum_iterations_number = m;
  this->maximum_perturbation_number_iteration = mp;
}

PlanoProducao HillClimbing::execute(int operacao_atomica) {
  int counter = 1;
  int para = 1;
  while (para) {
    this->perturbation(operacao_atomica, counter);
    // Report::imprimir_resultados(this->current_state);
    // int aux;
    // cin >> aux;
    if (this->stop_main_loop(counter)) {
      para = 0;
    }

    counter++;
  }
  
  return this->current_state;
}

bool HillClimbing::stop_main_loop(int iteracao) {
  if (iteracao > this->maximum_iterations_number) {
    return true;
  }
  return false;
}


bool HillClimbing::changeCurrenteState() {
  double cost1 = this->current_state.objectiveFunctionValue();
  double cost2 = this->next_state.objectiveFunctionValue();

  //cout << cost1 << " " << cost2 << "\n";
  if (cost1 > cost2)
    return true;
  return false;
}

void HillClimbing::perturbation(int operacao_atomica, int counter) {
  PlanoProducao p(this->current_state);

  this->next_state = p;

  //cout << this->next_state.objectiveFunctionValue() << "\n";

  for (int i = 0; i < this->maximum_perturbation_number_iteration; i++) {
    this->next_state = this->next_state.perturbation(operacao_atomica, counter);

    //cout << this->next_state.objectiveFunctionValue() << "\n";    

    if(this->changeCurrenteState()) {
      //cout << "Mudando status" << "\n";
      PlanoProducao n_state(this->next_state);
      //cout << this->current_state.objectiveFunctionValue() << "\n";
      this->current_state = n_state;
      //cout << this->current_state.objectiveFunctionValue() << "\n";
    }

    counter++;
  }

}

#endif