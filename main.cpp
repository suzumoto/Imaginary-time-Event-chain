#include <iostream>
#include <cstdio>
#include <vector>
#include <boost/random.hpp>
#include <alps/alea.h>
#include <cmath>
#include <unordered_set>
#include "UNITS.hpp"
#include "update.hpp"
#include "System_Parameter.hpp"
#include "bead.hpp"


void output(std::vector<bead> *worldline,int index);

int main(int argc, char *argv[]){
  std::vector<bead> worldline(P,bead());
  const int MCS = 100000000;
  const double SWEEPS = 20.0;
  const double THERMALIZATION = 100 * SWEEPS;
  double sweep_move = 0.0;
  double total_move = 0.0;
  int i,j,k; // iterator
  
  // Generate random number //
  boost::mt19937 eng(6390);
  boost::variate_generator<boost::mt19937&, boost::uniform_real<> > random(eng,boost::uniform_real<>());
  
  // Set temperature //
  double temperature = 40.0/TEMPERATURE_UNIT;
  double beta = 1.0 / temperature;
  double tau = beta / P;

  // indices //
  int moving = 0;

  // Observables //
  alps::ObservableSet measurements;
  measurements << alps::RealObservable("Energy");

  // Monte Carlo Steps //
  for(i = 0; i < MCS; ++i){
    Event_chain_imag(&worldline,&moving,SWEEPS,THERMALIZATION,&sweep_move,&total_move,tau,&random,&(measurements["Energy"]));
    if(i < 2000) output(&worldline,i);
  }
  std::cout << measurements["Energy"] << std::endl;
  return 0;
}

void output(std::vector<bead> *worldline, int index){
  FILE *fp;
  char filename[80];
  double position = 0.0;
  sprintf(filename,"./data/data%09d.dat",index);
  fp = fopen(filename,"w");
  int i = 0;
  for(std::vector<bead>::iterator it = (*worldline).begin(); it != (*worldline).end(); ++it){
    if(it != (*worldline).begin()) position += (*it).dist();
    fprintf(fp,"%lf %d\n",position,i);
    ++i;
  }
  fclose(fp);
}
