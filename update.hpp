#include <vector>
#include <boost/random.hpp>
#include <alps/alea.h>
#include <cmath>
#include "UNITS.hpp"
#include "System_Parameter.hpp"
#include "bead.hpp"

#ifndef IMAGINARY_EVENT_CHAIN_UPDATE_METHODS_
#define IMAGINARY_EVENT_CHAIN_UPDATE_METHODS_

inline double two(double a){
  return a*a;
}
inline double imaginary_potential(double global_distance, double tau){
  return global_distance*global_distance*0.5/tau;
}

inline double inv_imaginary_potential(double energy, double tau){
  return sqrt(2*tau*energy);
}

void measure(std::vector<bead> *worldline, alps::Observable *measurements, double tau){
  double energy = 0.0;
  int i;
  for(i = 0; i < P; ++i){
    energy -= two((*worldline)[i].dist());
  }
  energy /= P*two(tau)*2;
  energy += 0.5/tau;
  *measurements << energy*TEMPERATURE_UNIT;
}

double weight(std::vector<bead> *worldline, double tau){
  double energy = 0.0;
  int i;
  for(i = 0; i < P; ++i){
    energy += two((*worldline)[i].dist());
  }
  energy /= 2*tau;
  return -energy;
}

void Event_chain_imag(std::vector<bead> *worldline, int *moving, const double SWEEPS, const int THERMALIZATION, double *sweep_move, double *total_move, double tau, boost::variate_generator<boost::mt19937&, boost::uniform_real<> > *random, alps::Observable *measurements){
  int next_move = (*moving-1+P)%P;
  double s = -1.0;
  for(int j = 0; j < 2; ++j){
    int target = (*moving + 2*j - 1+P) % P;
    double rand = (*random)();
    while(rand == 0) rand = (*random)();
    double Estar = -log(rand);
    double g_dist = (1-2*j)*(*worldline)[(*moving+j)%P].dist();
    if(g_dist > 0) Estar += imaginary_potential((*worldline)[(*moving+j)%P].dist(),tau);
    double tmp_s = inv_imaginary_potential(Estar,tau) - g_dist;
    if(tmp_s < s || s < 0) {
      s = tmp_s;
      next_move = target;
    }
  }
  *total_move += s;
  *sweep_move += s;
  if(*sweep_move > SWEEPS){
    // MESUREMENT //
    (*worldline)[*moving].add_position(SWEEPS - *sweep_move + s);
    (*worldline)[*moving].add_dist(SWEEPS - *sweep_move + s);
    (*worldline)[*moving].set_period();
    (*worldline)[(*moving+1)%P].add_dist(*sweep_move - s - SWEEPS);
    if(*total_move > THERMALIZATION) measure(worldline,measurements,tau);
    (*worldline)[*moving].add_position(*sweep_move - SWEEPS);
    (*worldline)[*moving].add_dist(*sweep_move - SWEEPS);
    (*worldline)[*moving].set_period();
    (*worldline)[(*moving+1)%P].add_dist(SWEEPS - *sweep_move);
    *sweep_move -=  SWEEPS;
  }else{
    (*worldline)[*moving].add_position(s);
    (*worldline)[*moving].add_dist(s);
    (*worldline)[*moving].set_period();
    (*worldline)[(*moving+1)%P].add_dist(-s);
  }
  *moving = next_move;
}
#endif
