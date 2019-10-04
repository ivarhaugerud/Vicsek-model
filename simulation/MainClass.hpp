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
  //general variables
  int N;
  double v0;
  double R;
  double eta;
  double rho;
  double L;
  double L_half;
  int steps;
  double dt;
  double average_order;

  //variables for each step
  double sin_sum;
  double cos_sum;
  double vel_sum;
  int nr_neighbours;
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

  //create class
  MainClass();
  MainClass(int number, double velocity, double radius, double noise, double density, double delta_t,
            string boundary_condition, int nr_steps, string save_name, int amount_of_data);

  //initialize with random position and same velocity or gaussian velocity
  void initialize_random();                    
  void initialize_gaussian_velocity(double mu, double sigma);

  void step();                                //perform a step
  void BCs(int j);                            //check boundary conditions
  void run();                                 //run a simulation, and write averages to file
  void run_with_positions();                  //run and write all positions to file
  void equiliebrate(int equiliebrate_steps);  //equilibrate the system given a # steps

  int sign(double x);                         //get the sign of a double
  double calc_order();                        //calculate order parameter
  void write_state(ofstream& OutputFile);     //write all positions and velocities to file
  void write_something(double something);     //write a single variable to file

  //change parameters without initializing a new class
  void change_eta(double new_eta);
  void change_radius(double new_radius);
  void change_density(double new_density);
};

#endif
