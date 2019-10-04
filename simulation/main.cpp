#include "MainClass.hpp"

MainClass::MainClass()
{}

  MainClass::MainClass(int number, double velocity, double radius, double noise, double density,
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

    //define arrays and vectors
    state = Mat<int>(N, 5); // 5 = x & y -pos,angle,x-y velocity
    expectationvalues = vec(5, fill::zeros);

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
        state(j, 0) = ((zero_to_one_distribution(generator)-0.5)*L;
        state(j, 1) = ((zero_to_one_distribution(generator)-0.5)*L;
        state(j, 2) = (zero_to_one_distribution(generator)*2*pi;
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
      angle_sum = 0;
      for (int i = 0; j < N; i++)
        {
        if (i != j)
          if ( sqrt((state(j,0)-state(i,0))*(state(j,0)-state(i,0)) + (state(j,1)-state(i,1))*(state(j,1)-state(i,1)) ) < R)
          {
            angle_sum += state(i,2);
          }
        }
        state(j, 2) = angle_sum % (2*pi);
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
          if ( abs(state(j,0)) < L_half)
          { state(j,0) -=  L*sign(state(j,0));}

          if ( abs(state(j,1)) < L_half)
          { state(j,1) -=  L*sign(state(j,1))}
        }
    }
  }

  void MainClass::run()
  {
    ofstream outfile_x("data/" +  filename +"_x" + "".txt");
    if (!outfile_x.is_open())
      cout<<"Could not open file" << endl;

    ofstream outfile_y("data/" +  filename +"_y" + "".txt");
    if (!outfile_y.is_open())
      cout<<"Could not open file" << endl;

    for (t = 0; t < steps; t++)
    {
      step();
      write(outfile_x, "x");
      write(outfile_y, "y");
    }
  }

  int MainClass::sign(double x)
  {
    return (x > 0) - (x < 0);
  }

  void MainClass::write(ofstream& OutputFile, string coordinate)
  {
    if (coordinate == "x")
    {
      for (int j = 0; j < N; j++)
      {
        OutputFile << state(j, 0) << " ";
      }
    }

    if (coordinate == "y")
    {
      for (int j = 0; j < N; j++)
      {
        OutputFile << state(j, 1) << " ";
      }
    }

    OutputFile << " \n";
  }
