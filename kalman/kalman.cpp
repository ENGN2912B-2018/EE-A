// Code for Kalman filtering data from accelerometer
// The code implements the linear Kalman Fitler for a state described by
// postion and velocity.

#include <string>
#include <fstream>
#include <vector>
#include <cmath>

#include "kalmanfilter.hpp"

using namespace std;

int* read_sensor() {
  // Fill with commands to read data in from accelerometer
  // Use library accelerometer
}

void filter(int* sensor_data) {

  // Create file stream
  string filename = "kalman.dat";
  fstream file(filename);

  // Kalman filter
  KalmanFilter kalman;

  // Time step (set by the accelerometer)
  float step;

  // Initial conditions (need initialized values)
  vector<float> x_init;
  matrix<float, 0, false> P_init;

  // Vectors to initialize the transformation matrices
  vector<float> f{1, step, 0, 1};
  vector<float> h;    // Needs to be initialized
  vector<float> b{1/2*pow(step,2), step};

  Matrix<float, 0, false> F(2, 2, f); // Transformation matrix for prediction
  Matrix<float, 0, false> H(2, 2, h);    // Transformation matrix for readings
  Matrix<float, 0, false> B(2, 1, b); // Uncertainty matrix

// Initialize Kalman filter

  // These two calls must to be called first in this order
  kalman.setDim(2, 2, 4, 4, 4);  // The last three parameters are not correct
  kalman.init(x_init, P_init)



}


// Potential file which takes output of Kalman
// filter from buffer and writes to file
// with 'filename'. For now do file writing
// in 'filter' function
void write_to_file(string filename) {

}
