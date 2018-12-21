#include <fstream>
#include "kalman/kalman_.hpp"
#include "embedded/ADXL345_threaded.h"
#include "embedded/circularfifo_memory_sequential_consistent.hpp"

using namespace std;
using namespace Eigen;
int main() {
  // Files for output
  std::fstream accf("acceleration.csv", std::fstream::out);
  std::fstream velf("velocity.csv", std::fstream::out);
  std::fstream posf("position.csv", std::fstream::out);
  // Time step (set by the accelerometer)
  float fs = 3200;
  float step = 1/fs;
  // Initial conditions (need initialized values)
  VectorXd x_init(3);
  MatrixXd P_init(3,3);
  // Transformation matricies
  MatrixXd F(3,3);
  MatrixXd H(3,3);
  // Covariance matricies
  MatrixXd Q(3,3);
  MatrixXd R(3,3);
  // Initialize matricies
  x_init << 0, 0, 0;           // Initial acceleration is zero
  P_init << 0, 0, 0,           // Make initial acceleration assumption with absolute certainty
            0, 0, 0,
            0, 0, 0;
  F <<      1, 0, 0,
            0, 1, 0,
            0, 0, 1;
  H <<      1, 0, 0,
            0, 1, 0,
            0, 0, 1;
  Q <<      .0002, 0, 0,
            0, .0002, 0,
            0, 0, .0002;
  R <<      1, 0, 0,           // In reality need to get value from some steady state
            0, 1, 0,
            0, 0, 1;

  // Kalman filter
  KalmanFilter kalman(0, F, H, Q, R, P_init);
  kalman.init(0, x_init);

  //read_size (can be set lower than desired number of samples for batch ops  - if you need extra processing close to realtime)
  int read_size = 1;
  int num_samples = 0;
  cout << "Please enter desired number of samples (maximum is 500000): ";
  cin >> num_samples;

  //instantiate accelerometer class
  ADXL345 acc(num_samples);


  // Calibrate
  float* calibration = acc.calibrate(100);
  cout << calibration[0] << ", " << calibration[1] << ", " << calibration[2] << endl;

  //turn on the accelerometer and start adding to the queue
  acc.start();

  int iter = 0;                // for testing; tracks iterations
  int test_len = 30000;        // for testing; max number of iterations
  char * buff = new char[10];  // for testing;

  VectorXd z(3);            // Measurement data from accelerometer
  VectorXd x_(3);           // Kalman prediction (acceleration)
  VectorXd vel(3);          // Temporary container for x,y,z velocity
  VectorXd pos(3);          // Temporary container for x,y,z position
  VectorXd x_prev(3);       // Temporary container for x,y,z acceleration
  VectorXd vel_prev(3);     // Temporary container for x,y,z velocity
  VectorXd vel_sum(3);      // Sums for integration (velocity)
  VectorXd pos_sum(3);      // Sums for integration (position)

  pos << 0, 0, 0;
  vel << 0, 0, 0;
  x_prev << 0, 0, 0;
  vel_sum << 0, 0, 0;
  pos_sum << 0, 0, 0;
  vel_prev << 0, 0, 0;

  // Stop when 'a' key pressed
  while(iter <= num_samples) {

     // Read in acceleration for each direction
     float **results = acc.read(read_size);
     cout<<"read data"<<endl;
     z(0) = results[0][0];
     z(1) = results[0][1];
     z(2) = results[0][2];

     kalman.update(z);
     x_ = kalman.state();

     vel = 0.5*step*(x_  + x_prev);
     pos = 0.5*step*(vel + 2*vel_prev);
     vel_sum = vel_sum + vel;
     pos_sum = pos_sum + pos;

     // Numerical integration
     for(int i = 0; i < 3; i++){
      if(i == 2) {
        accf << x_(i) << std::endl;
        velf << vel_sum(i) << std::endl;
        posf << pos_sum(i) << std::endl;
      } else {
        accf << x_(i) << ", ";
        velf << vel_sum(i) << ", ";
        posf << pos_sum(i) << ", ";
       }
     }

    // testing
    iter++;
    x_prev = x_;
    vel_prev = vel_sum;
  }

  accf.close();
  velf.close();
  posf.close();
  return 0;
}
