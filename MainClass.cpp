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

    //define arrays and vectors
    state = Mat<double>(N+1, 5); // 5 = x & y -pos,angle,x-y velocity

    //for saving data
    filename = save_name;
    data_lines = amount_of_data;

    //type of BC
    BC = boundary_condition;

    //random number generator
    std::mt19937 generator (std::clock()); //initialize seed with clock
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
      }
  }

  void MainClass::step()
  {
    for (int j = 0; j < N; j++)
      {
      //update position
      state(j,0) += v0*dt*state(j,3);
      state(j,1) += v0*dt*state(j,4);
      }

    BCs();
    for (int j = 0; j < N; j++)
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

  void MainClass::BCs()
  {
    if (BC == "periodic")
    {
      for (int j = 0; j < N; j++)
        {
          if ( abs(state(j,0)) > L_half)
          { state(j,0) -=  L*sign(state(j,0));}
          if ( abs(state(j,1)) > L_half)
          { state(j,1) -=  L*sign(state(j,1));}
        }
    }


    if (BC == "hard")
    {
      for (int j = 0; j < N; j++)
        {
          if ( abs(state(j,0)) > L_half)
          { state(j,0) -=  dt*state(j,3);
            state(j,2) += twopi/2;
          }

          if ( abs(state(j,1)) > L_half)
          { state(j,1) -= dt*state(j,4);
            state(j,2) += twopi/2;
          }
        }
    }
  }

  void MainClass::run()
  {
    int denominator = steps/data_lines;
    int counter = 0;

    cout << "BEGINING RUN:\n" << endl;
    cout << "L:   " << L << endl;
    cout << "R:   " << R << endl;
    cout << "rho: " << rho << endl;
    cout << "v0:  " << v0 << endl;
    cout << "eta: " << eta<< endl;

    for (int t = 0; t < steps; t++)
    {
      if (t % denominator == 0)
      {
        ofstream outfile("data/" +  filename + to_string(counter) + ".txt");
        if (!outfile.is_open())
          cout<<"Could not open file" << endl;
        write_state(outfile);
        counter += 1;
      }

      step();
    }
  }

  void MainClass::equiliebrate()
  {for (int t = 0; t < steps; t++)
    {step();}}

  int MainClass::sign(double x)
  {
    return (x > 0) - (x < 0);
  }

  void MainClass::write_state(ofstream& OutputFile)
  {
    for (int j = 0; j < N; j++)
    {
      OutputFile << state(j, 0) << " " << state(j, 1)  << " " << state(j, 2)  << " " << state(j, 3)  << " " << state(j, 4) << "\n";
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

  void MainClass::write_order()
  {
    ofstream outfile("data/" +  filename + ".txt", std::ios_base::app);
    if (!outfile.is_open())
      cout<<"Could not open file" << endl;
    outfile << L << " " << R << " " << rho << " " << v0 << " " << eta << " " << calc_order() << "\n";
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
