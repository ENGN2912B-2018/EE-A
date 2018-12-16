// Author: Kevin Anderson
// Date: Dec 12, 2018
// Code for Kalman filtering data from accelerometer on 3 variable state


#include "../kalman_.hpp"

//using namespace std;
using namespace Eigen;

// !**-->  All matricies have fixed size, eventually should change them to fixed size for Eigen!!!!!

// State is defined as position, velocity and acceleration on 3D axis
/*
 *    | x''   |
 *    | y''   |
 *    | z''   |
 */

int main() {

   // Create file stream
   std::fstream file("../testing output files/kalman.csv", std::fstream::out);
   std::fstream gain("../testing output files/gain.dat", std::fstream::out);
   // Read in values from file
   std::fstream data("../../embedded/trials/stationary.csv", std::fstream::in);
   //std::fstream data("stationary_filtered.csv", std::fstream::in);
   // Time step (set by the accelerometer)
   float fs = 3200;
   float step = 1/fs;
   // Initial conditions (need initialized values)
   VectorXd x_init(3);
   MatrixXd P_init(3,3);
   // Transformation matricies
   MatrixXd F(3,3);   // Transformation matrix for prediction (F)
   //MatrixXd B;   // Uncertainty matrix
   MatrixXd H(3,3);   // Transformation matrix for readings (C)
   // Covariance matricies
   MatrixXd Q(3,3);
   MatrixXd R(3,3);
   // Initialize matricies
   x_init << 0, 0, 0;          // Assume intial position is zero
   P_init << 0, 0, 0,           // Assume intial velocity is zero
             0, 0, 0,
             0, 0, 0;
   F << 1, 0, 0,           // Assume intial velocity is zero
        0, 1, 0,
        0, 0, 1;
   H << 1, 0, 0,
        0, 1, 0,
        0, 0, 1;
   Q << 5, 0, 0,
        0, 5, 0,
        0, 0, 5;
                      // In reality need to get value from some steady state
   R << 8, 0, 0,
        0, 8, 0,
        0, 0, 8;

   // Kalman filter
   KalmanFilter kalman(0, F, H, Q, R, P_init);
   kalman.init(0, x_init);    // Initialize state at t = 0

   // For testing, launch process to monitor key strokes (A - means stop
   // running filter)
   char let;

   VectorXd z(3);           // Temporary container for x,y,z acceleration
   VectorXd x_(3);           // Temporary container for x,y,z acceleration
   bool stop = false;

   int iter = 0;                // for testing; tracks iterations
   int test_len = 30000;        // for testing; max number of iterations
   char * buff = new char[10];  // for testing;

   char filtered;
   std::cout << "is file filtered (y/n): ";
   std::cin >> filtered;

   // Ignore x,y,z line
   if(filtered == 'n') {
     data.ignore(10, '\n');
   }
   std::cout << "iterations: ";
   std::cin >> test_len;

   MatrixXd K;

   // Stop when 'a' key pressed
   while(iter <= test_len) {

     // Read in acceleration for each direction
     data.getline(buff, 10, ',');
     z(0) = atof(buff);
     data.getline(buff, 10, ',');
     z(1) = atof(buff);
     data.getline(buff, 10, '\n');
     z(2) = atof(buff);

     kalman.update(z);
     x_ = kalman.state();

    K = kalman.gain();
    for(int r = 0; r < 3; r++) {
      for(int c = 0; c < 3; c++) {
        if(c == 2 && r == 2) {
           gain << K(r,c) << std::endl;
        } else {
           gain << K(r,c) << ", ";
       }
      }
    }

    for(int i = 0; i < 3; i++){
     if(i == 2) {
       file << x_(i) << std::endl;
     } else {
       file << x_(i) << ", ";
      }
    }

     // testing
     iter++;
     //std::cin >> let;
     stop = let == 'a' || iter == test_len;
   }

   data.close();
   file.close();
   gain.close();
   return 0;
}
