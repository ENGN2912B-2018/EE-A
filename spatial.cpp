#include "kalman/kalman_.hpp"
#include "embedded/ADXL345_threaded.h"
#include "embedded/circularfifo_memory_sequential_consistent.hpp"

using namespace std;
using namespace Eigen;

int main() {
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

  VectorXd z(3);               // Temporary container for x,y,z acceleration
  VectorXd x_(3);              // Temporary container for x,y,z acceleration
  bool stop = false;

  int iter = 0;                // for testing; tracks iterations
  int test_len = 30000;        // for testing; max number of iterations
  char * buff = new char[10];  // for testing;

  while(iter <= num_samples) {

     // Read in acceleration data from buffer
     float **results = acc.read(read_size);
     z(1) = results[0][0];
     z(2) = results[0][1];
     z(3) = results[0][2];

     kalman.update(z);
     x_ = kalman.state();

    // Increment
    iter++;
  }

  return 0;
}
