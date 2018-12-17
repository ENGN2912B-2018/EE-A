#include <fstream>
#include "kalman/kalman_.hpp"
#include "embedded/ADXL345_threaded.h"
#include "embedded/circularfifo_hazard_platform_dependent.hpp"
#include "embedded/circularfifo_memory_relaxed_acquire_release.hpp"
#include "embedded/circularfifo_memory_sequential_consistent.hpp"

using namespace std;

int main() {
  // Files for output
  std::fstream accf("testing output files/acceleration.csv", std::fstream::out);
  std::fstream velf("testing output files/velocity.csv", std::fstream::out);
  std::fstream posf("testing output files/position.csv", std::fstream::out);
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

  // Instantiate accelerometer class
  ADXL345 acc;

  //read_size (can be set lower than desired number of samples for batch ops  - if you need extra processing close to realtime)
  int read_size = 1;
  int num_samples = 0;
  cout << "Please enter desired number of samples (maximum is 500000): ";
  cin >> num_samples;

  // Calibrate
  float* calibration = acc.calibrate(100);
  cout << calibration[0] << ", " << calibration[1] << ", " << calibration[2] << endl;

  //turn on the accelerometer and start adding to the queue
  acc.start();

  VectorXd vel_sum(3);
  VectorXd pos_sum(3);
  VectorXd vel(3);          // Temporary container for x,y,z velocity
  VectorXd vel_prev(3);     // Temporary container for x,y,z velocity
  VectorXd pos(3);          // Temporary container for x,y,z position
  VectorXd z(3);            // Temporary container for x,y,z acceleration
  VectorXd x_prev(3);       // Temporary container for x,y,z acceleration
  VectorXd x_(3);           // Temporary container for x,y,z acceleration

  int iter = 0;                // for testing; tracks iterations
  int test_len = 30000;        // for testing; max number of iterations
  char * buff = new char[10];  // for testing;

  vel << 0, 0, 0;
  vel_prev << 0, 0, 0;
  x_prev << 0, 0, 0;
  vel_sum << 0, 0, 0;
  pos_sum << 0, 0, 0;

  while(iter <= num_samples) {

     // Read in acceleration data from buffer
     float **results = acc.read(read_size);
     z(1) = results[0][0];
     z(2) = results[0][1];
     z(3) = results[0][2];

     kalman.update(z);
     x_ = kalman.state();

     vel = 0.5*step*(x_prev+x_);
     pos = 0.5*step*(vel_prev+vel);

     vel_sum += vel;
     pos_sum += pos;

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
    x_prev = x_;
    vel_prev = vel;

    iter++; // Increment
  }

  return 0;
}
