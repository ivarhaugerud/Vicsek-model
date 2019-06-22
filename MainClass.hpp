#ifndef MAINCLASS
#define MAINCLASS

#include <iostream>
#include <cmath>
#include <armadillo>
#include <cstdio>
#include <string>
#include <iomanip>
#include <fstream>

using namespace std;
using namespace arma;

class MainClass
{
public:
  int N;
  double v0;
  double R;
  double eta;
  double rho;
  double L;
  double L_half;
  int steps;
  double dt;

  double sin_sum;
  double cos_sum;
  double twopi = 2*acos(-1.0);

  //for writing to file
  string filename;
  int data_lines;

  //vectors
  Mat<double> state;

  //Boundary conditions
  string BC;

  //random number generators
  std::mt19937 generator;
  std::uniform_real_distribution<double> zero_to_one_distribution;

  MainClass();
  MainClass(int number, double velocity, double radius, double noise, double density, double delta_t,
            string boundary_condition, int nr_steps, string save_name, int amount_of_data);
  //the function names are self explanetory
  void initialize_random();
  void step();
  void run();
  void BCs();
  int sign(double x);
  void write_state(ofstream& OutputFile);
  double calc_order();
  void write_order();
  void equiliebrate();
  void change_eta(double new_eta);
  void change_radius(double new_radius);
  void change_density(double new_density);
};

#endif
