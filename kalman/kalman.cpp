// Code for Kalman filtering data from accelerometer
// The code implements the linear Kalman Fitler for a state described by
// postion and velocity.

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>

#include "kalman_.hpp"

//using namespace std;
using namespace Eigen;

/*
 int* read_sensor() {
  // Fill with commands to read data in from accelerometer
  // Use library accelerometer
}

// Potential file which takes output of Kalman
// filter from buffer and writes to file
// with 'filename'. For now do file writing
// in 'filter' function
void write_to_file(string filename) {}

void filter(int* sensor_data) {}
*/

int main() {

   // Create file stream
   std::string filename = "kalman.dat";
   std::fstream file(filename);
   // Time step (set by the accelerometer)
   float fs = 3200;
   float step = 1/fs;
   // Initial conditions (need initialized values)
   VectorXd x_init;
   MatrixXd P_init;
   // Transformation matricies
   MatrixXd A;   // Transformation matrix for prediction (F)
   MatrixXd B;   // Uncertainty matrix
   MatrixXd C;   // Transformation matrix for readings (C)
   // Covariance matricies
   MatrixXd Q;
   MatrixXd R;
   // Initialize matricies
   x_init << 0, 0;            // Assume intial position is zero
   P_init << 0, 0, 0, 0;      // Assume intial velocity is zero
   A << 1, step, 0, 1;
   B << 1/2*pow(step,2), step;
   C << 1, 1, 1, 1;
   Q << 1, 0, 0, 1;   // For testing purposes, assume zero matrix;
                      // In reality need to get value from some steady state
   R << 1, 0, 0, 1;   // No manufacturer senor sensitivity, use scaled identity matrix
   // Kalman filter
   KalmanFilter kalman(0, A, C, Q, R, P_init);
   kalman.init(0, x_init);    // Initialize state at t = 0

   // For testing, launch process to monitor key strokes (A - means stop
   // running filter)
   char let;
   bool stop = false;

   // Stop when 'a' key pressed
   while(!stop) {
     // Read data form the sensor_data
     // float * samples = read_sensor();

     // Amount of samples read from sensor
     int len = 100;

     float * velocity = (float *)malloc(sizeof(float)*len);
     float * position = (float *)malloc(sizeof(float)*len);

     velocity = numerical_int(samples, step, len);
     position = numerical_int(velocity, step, len);

     // Run Kalman filer on data and write to file
     for(int x = 0; x < len; x++) {
       kalman.update();
       file << kalman.state();
     }

     cin >> let;
     stop = let == 'a';
   }

   return 0;
}
