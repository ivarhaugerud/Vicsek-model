#include <iostream>
#include <cmath>
#include <armadillo>
#include <cstdio>
#include <random>
#include <ctime>
#include <string>
#include <typeinfo>

#include "MainClass.hpp"

using namespace std;
using namespace arma;

//runs for a single configuration which can be specified in command line
//used for example for making plot of spins with black down and white up

int main(int argc, char const *argv[])
  {
    int number = atoi(argv[1]);
    double velocity = atof(argv[2]);
    double radius = atof(argv[3]);
    double noise = atof(argv[4]);
    double density = atof(argv[5]);
    double delta_t = atof(argv[6]);
    int amount_of_data = atoi(argv[7]);
    int nr_steps = atoi(argv[8]);

    string boundary_condition = "hard";
    string save_name  = "today";

    MainClass vicsek(number, velocity, radius, noise, density, delta_t,
                     boundary_condition, nr_steps, save_name, amount_of_data);

    vicsek.initialize_random();
    //vicsek.initialize_gaussian_velocity(velocity, 0.75);
    vicsek.run_with_positions();

    return 0;
  }
