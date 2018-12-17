// Author: Kevin Anderson
// Date: Dec 12, 2018
// Code for Kalman filtering data from accelerometer on 3 variable state


#include "../kalman_.hpp"

using namespace Eigen;

// State is defined as position, velocity and acceleration on 3D axis
/*
 *    | x'   |
 *    | y'   |
 *    | z'   |
 *    | x''  |
 *    | y''  |
 *    | z''  |
 */

int main(int argc, char * argv[]) {

  std::string inputfile = "../../embedded/trials/";
  if(argc == 2) {
    inputfile += argv[1];
  } else {
    inputfile += "stationary.csv";
  }

   // Create file stream
   std::fstream gain("../testing output files/gain.dat", std::fstream::out);
   std::fstream file("../testing output files/kalman.csv", std::fstream::out);
   // Read in values from file
   std::fstream data(inputfile, std::fstream::in);
   //std::fstream data("stationary_filtered.csv", std::fstream::in);
   // Time step (set by the accelerometer)
   float fs = 3200;
   float step = 1/fs;
   // Initial conditions (need initialized values)
   VectorXd x_init(6);
   MatrixXd P_init(6,6);
   // Transformation matricies
   MatrixXd F(6,6);   // Transformation matrix for prediction (F)
   //MatrixXd B;   // Uncertainty matrix
   MatrixXd H(3,6);   // Transformation matrix for readings (C)
   // Covariance matricies
   MatrixXd Q(6,6);
   MatrixXd R(3,3);
   // Initialize matricies
   x_init << 0, 0, 0, 0, 0, 0;     // Assume intial position is zero
   P_init << 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0;
   F << 1, 0, 0, step, 0, 0,          // Assume intial velocity is zero
        0, 1, 0, 0, step, 0,
        0, 0, 1, 0, 0, step,
        0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1;

   H << 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 1;
   Q << 10, 0, 0, 0, 0, 0,
        0, 10, 0, 0, 0, 0,
        0, 0, 10, 0, 0, 0,
        0, 0, 0, 10, 0, 0,
        0, 0, 0, 0, 10, 0,
        0, 0, 0, 0, 0, 10;
                      // In reality need to get value from some steady state
   R << 8, 0, 0,
        0, 8, 0,
        0, 0, 8;

   // Kalman filter
   KalmanFilter kalman(step, F, H, Q, R, P_init);
   kalman.init(0, x_init);    // Initialize state at t = 0

   // For testing, launch process to monitor key strokes (A - means stop
   // running filter)
   char let;

   VectorXd z(3);           // Temporary container for x,y,z acceleration
   VectorXd x_(3);           // Temporary container for x,y,z acceleration
   bool stop = false;

   int n = 6;
   int m = 3;
   int iter = 0;                // for testing; tracks iterations
   int test_len = 30000;        // for testing; max number of iterations
   char * buff = new char[10];  // for testing;

   char filtered;
   std::cout << "is file filtered (y/n): ";
   std::cin >> filtered;
   std::cout << "iterations: ";
   std::cin >> test_len;

   // Ignore x,y,z line
   if(filtered = 'n') {
     data.ignore(10, '\n');
   }
   MatrixXd K;

   // Stop when 'a' key pressed
   while(iter <= test_len) {

     // Read in acceleration for each direction
     data.getline(buff, 20, ',');
     z(0) = atof(buff);
     data.getline(buff, 20, ',');
     z(1) = atof(buff);
     data.getline(buff, 20, '\n');
     z(2) = atof(buff);

    kalman.update(z);
    x_ = kalman.state();

    K = kalman.gain();
    for(int r = 0; r < n; r++) {
      for(int c = 0; c < m; c++) {
        if(c == m-1) {
           gain << K(r,c) << std::endl;
           if(r == n-1) {
             gain <<std::endl;
           }
        } else {
           gain << K(r,c) << ", ";
       }
      }
    }

    for(int i = 0; i < n; i++){
     if(i == n-1) {
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
