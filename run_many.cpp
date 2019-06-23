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

int main(int argc, char const *argv[])
  {
    int number = 250;
    double velocity = 1;
    double radius = 0.1;
    double noise = 0.1;
    double density = 100;
    double delta_t = 0.01;
    int amount_of_data = 100;
    int nr_steps = 10000;

    string boundary_condition = "periodic";
    string save_name  = "new_run_100";


    double min = 0;
    double max = 2.5;
    double step = 0.05;

    MainClass vicsek(number, velocity, radius, noise, density, delta_t,
                     boundary_condition, nr_steps, save_name, amount_of_data);

    for (double changing = min; changing < max; changing += step)
    {
      vicsek.change_eta(changing);
      vicsek.initialize_random();
      vicsek.equiliebrate(5000);
      vicsek.run();
    }

    return 0;
  }
