#ifndef hill_climbing_h
#define hill_climbing_h

#include <iostream>
#include "plano_producao.cpp"
#include "../util/report.cpp"
#include <omp.h>

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
  PlanoProducao pp(p);
  this->current_state = pp;
  this->maximum_iterations_number = m;
  this->maximum_perturbation_number_iteration = mp;
}

PlanoProducao HillClimbing::execute(int operacao_atomica) {
  int counter = 1;
  int para = 1;
  
  int threads = omp_get_max_threads();
  //Num threads = 4
  PlanoProducao p1(this->current_state);
  PlanoProducao p2(this->current_state);
  PlanoProducao p3(this->current_state);
  PlanoProducao p4(this->current_state);
  HillClimbing hc1(p1, this->maximum_iterations_number, this->maximum_perturbation_number_iteration);
  HillClimbing hc2(p2, this->maximum_iterations_number, this->maximum_perturbation_number_iteration);
  HillClimbing hc3(p3, this->maximum_iterations_number, this->maximum_perturbation_number_iteration);
  HillClimbing hc4(p4, this->maximum_iterations_number, this->maximum_perturbation_number_iteration);

  #pragma omp parallel shared(hc1, hc2, hc3, hc4, operacao_atomica) private(counter)
  {
    #pragma omp for
    for(counter = 1; counter < this->maximum_iterations_number; counter++) {
      int tid = omp_get_thread_num();
      if (tid == 0)
        hc1.perturbation(operacao_atomica, counter);
      else if (tid == 1)
        hc2.perturbation(operacao_atomica, counter);
      else if (tid == 2)
        hc3.perturbation(operacao_atomica, counter);
      else if (tid == 3)
        hc4.perturbation(operacao_atomica, counter);
      // Report::imprimir_resultados(this->current_state);
      // int aux;
      // cin >> aux;
      // if (this->stop_main_loop(counter)) {
      //   para = 0;
      // }
    }
  }

  if (hc1.current_state.objectiveFunctionValue() > hc2.current_state.objectiveFunctionValue() 
    && hc1.current_state.objectiveFunctionValue() > hc3.current_state.objectiveFunctionValue() 
    && hc1.current_state.objectiveFunctionValue() > hc4.current_state.objectiveFunctionValue())
    return hc1.current_state;
  else if(hc2.current_state.objectiveFunctionValue() > hc1.current_state.objectiveFunctionValue() 
    && hc2.current_state.objectiveFunctionValue() > hc3.current_state.objectiveFunctionValue() 
    && hc2.current_state.objectiveFunctionValue() > hc4.current_state.objectiveFunctionValue())
    return hc2.current_state;
  else if(hc3.current_state.objectiveFunctionValue() > hc1.current_state.objectiveFunctionValue() 
    && hc3.current_state.objectiveFunctionValue() > hc2.current_state.objectiveFunctionValue() 
    && hc3.current_state.objectiveFunctionValue() > hc4.current_state.objectiveFunctionValue())
    return hc3.current_state;
  else
    return hc4.current_state;

}

bool HillClimbing::stop_main_loop(int iteracao) {
  if (iteracao > this->maximum_iterations_number) {
    return true;
  }
  return false;
}


bool HillClimbing::changeCurrenteState() {
  double cost1;
  double cost2;
  
  //omp_set_nested(1);
  
  cost1 = this->current_state.objectiveFunctionValue();
  cost2 = this->next_state.objectiveFunctionValue();

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