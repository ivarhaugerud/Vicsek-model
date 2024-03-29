#include "MainClass.hpp"

MainClass::MainClass()
{}

  MainClass::MainClass(int number, double velocity, double radius, double noise, double density, double delta_t,
                  string boundary_condition, int nr_steps, string save_name, int amount_of_data)
  {
    //computational parameters
    N = number;
    v0 = velocity;
    R = radius;
    eta = noise;
    rho = density;
    L = sqrt(N/rho);
    L_half = L/2;
    steps = nr_steps;
    dt = delta_t;
    average_order = 0;

    //define arrays and vectors
    state = Mat<double>(N+1, 6); // 5 = x & y -pos,angle,x-y velocity, velocity

    //for saving data
    filename = save_name;
    data_lines = amount_of_data;

    //type of BC
    BC = boundary_condition;

    //random number generator
    std::mt19937 generator (1); //initialize seed with clock
    std::uniform_real_distribution<double> zero_to_one_distribution(0.0, 1.0);
  }

  void MainClass::initialize_random()
  {
    for (int j = 0; j < N; j++)
      {
        state(j, 0) = (zero_to_one_distribution(generator)-0.5)*L;
        state(j, 1) = (zero_to_one_distribution(generator)-0.5)*L;
        state(j, 2) = zero_to_one_distribution(generator)*twopi;
        state(j, 3) = v0*cos( state(j,2) );
        state(j, 4) = v0*sin( state(j,2) );
        state(j, 5) = v0;
      }
  }

  void MainClass::initialize_gaussian_velocity(double mu, double sigma)
  {
    double vel = 0;
    std::normal_distribution<double> normal_distribution(mu, sigma);
    for (int j = 0; j < N; j++)
      {
        vel = normal_distribution(generator);
        state(j, 3) = vel*cos( state(j,2) );
        state(j, 4) = vel*sin( state(j,2) );
        state(j, 5) = vel;
      }
  }

  void MainClass::step()
  {
    for (int j = 0; j < N; j++)
      {
      //update position
      state(j,0) += state(j,5)*dt*state(j,3);
      state(j,1) += state(j,5)*dt*state(j,4);
      }
    //BCs(); 

    for (int j = 0; j < N; j++)
    {
      if (abs(state(j,0)) > L_half or abs(state(j,1)) > L_half)
      {BCs(j);}

      else 
      {
        sin_sum = 0;
        cos_sum = 0;
    
      for (int i = 0; i < N; i++)
        {
          if ( sqrt( (state(j,0)-state(i,0))*(state(j,0)-state(i,0)) + (state(j,1)-state(i,1))*(state(j,1)-state(i,1)) ) < R)
          {
            sin_sum += sin(state(i,2));
            cos_sum += cos(state(i,2));
          }
        }
      state(j, 2) = atan2(sin_sum, cos_sum) + (zero_to_one_distribution(generator)-0.5)*eta;
      state(j, 3) = v0*cos( state(j,2) );
      state(j, 4) = v0*sin( state(j,2) );
       }
    }
  }

  void MainClass::BCs(int j)
  {
    if (BC == "periodic")
    {
      if ( abs(state(j,0)) > L_half)
      { state(j,0) -=  L*sign(state(j,0));}
      if ( abs(state(j,1)) > L_half)
      { state(j,1) -=  L*sign(state(j,1));}
    }


    if (BC == "hard")
    {
      if (state(j,0) > L_half)
      { state(j,0)  =  L_half - state(j,5)*dt;
        state(j, 3) *= (-1);
        state(j, 2) = atan2(state(j,4), state(j,3));
      }

      else if (state(j,0) < -L_half)
      { state(j,0)  =  -L_half + state(j,5)*dt;
        state(j, 3) *= (-1);
        state(j, 2) = atan2(state(j,4), state(j,3));
      }

      else if ( state(j,1) > L_half)
      { state(j,1)  = L_half - state(j,5)*dt;
        state(j, 4) *= (-1);
        state(j, 2) = atan2(state(j,4), state(j,3));
      }

      else if ( state(j,1) < -L_half)
      { state(j,1)  = -L_half + state(j,5)*dt;
        state(j, 4) *= (-1);
        state(j, 2) = atan2(state(j,4), state(j,3));
      }
    }
  }

  void MainClass::run()
  {
    int denominator = steps/data_lines;
    double average_order = 0;

    for (int t = 0; t < steps; t++)
    {
      if (t % denominator == 0)
      {
        average_order += calc_order();
      }
      step();
    }
    write_something(average_order/data_lines);
  }

  void MainClass::run_with_positions()
  {
    int denominator = steps/data_lines;
    int counter = 0;

    for (int t = 0; t < steps; t++)
    {
      if (t % denominator == 0)
      {
        ofstream outfile("../data/" +  filename + to_string(counter) + ".txt");
        if (!outfile.is_open())
          cout<<"Could not open file" << endl;
        write_state(outfile);
        counter += 1;
      }
      step();
    }
  }

  void MainClass::equiliebrate(int equiliebrate_steps)
  {for (int t = 0; t < equiliebrate_steps; t++)
    {step();}}

  int MainClass::sign(double x)
  {
    return (x > 0) - (x < 0);
  }

  void MainClass::write_state(ofstream& OutputFile)
  {
    for (int j = 0; j < N; j++)
    {
      OutputFile << state(j, 0) << " " << state(j, 1)  << " " << state(j, 2)  << " " << state(j, 3)  << " " << state(j, 4) << " " << state(j,5) << "\n";
    }
  }

double MainClass::calc_order()
  {
    double total_velocity_x = 0;
    double total_velocity_y = 0;
    for (int j=0; j < N; j++)
    {
      total_velocity_x += state(j, 3);
      total_velocity_y += state(j, 4);
    }
    return sqrt(total_velocity_x*total_velocity_x + total_velocity_y*total_velocity_y)/(N*v0);
  }

  void MainClass::write_something(double something)
  {
    ofstream outfile("../data/" +  filename + ".txt", std::ios_base::app);
    if (!outfile.is_open())
      cout<<"Could not open file" << endl;
    outfile << L << " " << R << " " << rho << " " << v0 << " " << eta << " " << something << "\n";
  }

  void MainClass::change_eta(double new_eta)
  {
    eta = new_eta;
  }

  void MainClass::change_radius(double new_radius)
  {
    R = new_radius;
  }

  void MainClass::change_density(double new_density)
  {
    rho = new_density;
    L = sqrt(N/rho);
  }
