// Code for Kalman filtering data from accelerometer
// The code implements the linear Kalman Fitler for a state described by
// postion and velocity.


#include "kalman_.hpp"

//using namespace std;
using namespace Eigen;

/*

// Potential file which takes output of Kalman
// filter from buffer and writes to file
// with 'filename'. For now do file writing
// in 'filter' function
void write_to_file(string filename) {}

// Filter the sensor data to get rid of some of the noise
void filter(int* sensor_data) {}
*/

// !**-->  All matricies have fixed size, eventually should change them to fixed size for Eigen!!!!!

// State is defined as position, velocity and acceleration on 3D axis
/*
 *    | x   |
 *    | y   |
 *    | z   |
 *    | x'  |
 *    | y'  |
 *    | z'  |
 *    | x'' |
 *    | y'' |
 *    | z'' |
 */

int main() {

   // Create file stream
   std::fstream file("../testing output files/kalman.csv", std::fstream::out);
   std::fstream gain("../testing output files/gain.dat", std::fstream::out);
   // Read in values from file
   std::fstream data("../embedded/trials/straight_return.csv", std::fstream::in);
   //std::fstream data("stationary_filtered.csv", std::fstream::in);
   // Time step (set by the accelerometer)
   float fs = 3200;
   float step = 1/fs;
   // Initial conditions (need initialized values)
   VectorXd x_init(9);
   MatrixXd P_init(9, 9);
   // Transformation matricies
   MatrixXd F(9,9);   // Transformation matrix for prediction (F)
   //MatrixXd B;   // Uncertainty matrix
   MatrixXd H(3,9);   // Transformation matrix for readings (C)
   // Covariance matricies
   MatrixXd Q(9,9);
   MatrixXd R(3,3);
   // Initialize matricies
   x_init << 0, 0, 0, 0, 0, 0, 0, 0, 0;          // Assume intial position is zero
   P_init << 0, 0, 0, 0, 0, 0, 0, 0, 0,          // Assume intial velocity is zero
             0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0,
             0, 0, 0, 0, 0, 0, 0, 0, 0;
   F << 1, 0, 0, step, 0, 0, 1/2*pow(step,2), 0, 0,          // Assume intial velocity is zero
        0, 1, 0, 0, step, 0, 0, 1/2*pow(step,2), 0,
        0, 0, 1, 0, 0, step, 0, 0, 1/2*pow(step,2),
        0, 0, 0, 1, 0, 0, step, 0, 0,
        0, 0, 0, 0, 1, 0, 0, step, 0,
        0, 0, 0, 0, 0, 1, 0, 0, step,
        0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1;
   H << 0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1;
   Q << (1/20)*pow(step, 5), (1/8)*pow(step, 4), (1/6)*pow(step, 3), 0, 0, 0, 0, 0, 0,    // Identity
        (1/8)*pow(step, 4), (1/3)*pow(step, 3), (1/2)*pow(step, 2), 0, 0, 0, 0, 0, 0,
        (1/6)*pow(step, 3), (1/2)*pow(step, 2), step, 0, 0, 0, 0, 0, 0,
        0, 0, 0,(1/20)*pow(step, 5), (1/8)*pow(step, 4), (1/6)*pow(step, 3), 0, 0, 0,     // Identity
        0, 0, 0,(1/8)*pow(step, 4), (1/3)*pow(step, 3), (1/2)*pow(step, 2), 0, 0, 0,
        0, 0, 0,(1/6)*pow(step, 3), (1/2)*pow(step, 2), step, 0, 0, 0,
        0, 0, 0, 0, 0, 0, (1/20)*pow(step, 5), (1/8)*pow(step, 4), (1/6)*pow(step, 3),
        0, 0, 0, 0, 0, 0, (1/8)*pow(step, 4), (1/3)*pow(step, 3), (1/2)*pow(step, 2),
        0, 0, 0, 0, 0, 0,(1/6)*pow(step, 3), (1/2)*pow(step, 2), step;
                      // In reality need to get value from some steady state
   R << 200, 70, 100,      // No manufacturer senor sensitivity, use scaled identity matrix
        40, 120, 100,
        90, 71, 180;

   // Kalman filter
   KalmanFilter kalman(0, F, H, Q, R, P_init);
   kalman.init(0, x_init);    // Initialize state at t = 0

   // For testing, launch process to monitor key strokes (A - means stop
   // running filter)
   char let;

   VectorXd z(3);           // Temporary container for x,y,z acceleration
   VectorXd x_(9);           // Temporary container for x,y,z acceleration
   bool stop = false;

   int iter = 0;                // for testing; tracks iterations
   int test_len = 30000;        // for testing; max number of iterations
   char * buff = new char[10];  // for testing;

   // Ignore x,y,z line
   // data.ignore(10, '\n');
   std::cout << "iterations: ";
   std::cin >> test_len;
   std::cout << std::endl;

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
    for(int r = 0; r < 9; r++) {
      for(int c = 0; c < 3; c++) {
        if(c == 2 && r == 8) {
           gain << K(r,c) << std::endl;
        } else {
           gain << K(r,c) << ", ";
       }
      }
    }


    for(int i = 0; i < 9; i++){
     if(i == 8) {
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

/*
  MatrixXd P = kalman.covP();

   for(int r = 0; r < 9; r++) {
     for(int c = 0; c < 9; c++) {
       if(c == 8) {
          std::cout << P(r,c) << std::endl;
       } else {
          std::cout << P(r,c) << ", ";
      }
     }
   }
 */

   data.close();
   file.close();
   gain.close();

   return 0;
}


// Increasing value of diagonal

// Amount of samples read from sensor
// int len = 100;

// float * velocity = (float *)malloc(sizeof(float)*len);
// float * position = (float *)malloc(sizeof(float)*len);

// velocity = numerical_int(samples, step, len);
// position = numerical_int(velocity, step, len);

// Run Kalman filer on data and write to file
//for(int x = 0; x < len; x++) {
/*
 for(int p = 0; p < 9; p++){
   if(p == 8) {
     std::cout << x_(p) << std::endl;
     std::cout << "newline" << std::endl;
   } else {
     std::cout << x_(p) << ", ";
    }
 }
 */
     //}
