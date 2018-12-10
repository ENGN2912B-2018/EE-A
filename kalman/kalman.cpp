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
   float step = 1/1000;
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
   A << 1, step, 0, 1;
   B << 1/2*pow(step,2), step;
   C << 1, 1, 1, 1;
   Q << 1, 1, 1, 1;
   R << 1, 1, 1, 1;
   P_init << 0, 0, 0, 0;
   x_init << 0, 0;
   // Kalman filter
   KalmanFilter kalman(0,A,C,Q,R,P_init);
   kalman.init(0, x_init);    // Initialize state at t = 0

   // For testing, launch process to monitor key strokes (A - means stop
   // running filter)
   char let;
   bool stop = false;
  
   while(!stop) {
     kalman.update(read_sensor());
     cin >> let;
     stop = let == 'a';
   }


   return 0;
}
