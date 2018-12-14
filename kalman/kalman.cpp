// Code for Kalman filtering data from accelerometer
// The code implements the linear Kalman Fitler for a state described by
// postion and velocity.


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

// !**-->  All matricies have fixed size, eventually should change them to fixed size
// for Eigen!!!!!

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

   // Read in values from file
   std::fstream data("../embedded/trials/stationary.csv", std::fstream::in);
   // Create file stream
   std::fstream file("kalman.csv", std::fstream::out);
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
   Q << 1, 0, 0, 0, 0, 0, 0, 0, 0,    // Identity
        0, 1, 0, 0, 0, 0, 0, 0, 0,
        0, 0, 1, 0, 0, 0, 0, 0, 0,
        0, 0, 0, 1, 0, 0, 0, 0, 0,
        0, 0, 0, 0, 1, 0, 0, 0, 0,
        0, 0, 0, 0, 0, 1, 0, 0, 0,
        0, 0, 0, 0, 0, 0, 1, 0, 0,
        0, 0, 0, 0, 0, 0, 0, 1, 0,
        0, 0, 0, 0, 0, 0, 0, 0, 1;
                      // In reality need to get value from some steady state
   R << 1, 0, 0,      // No manufacturer senor sensitivity, use scaled identity matrix
        0, 1, 0,
        0, 0, 1;

   // Kalman filter
   KalmanFilter kalman(0, F, H, Q, R, P_init);
   kalman.init(0, x_init);    // Initialize state at t = 0

   // For testing, launch process to monitor key strokes (A - means stop
   // running filter)
   char let;
   bool stop = false;

   int iter = 0;             // For testing; tracks iterations
   int test_len = 20000;      // for testing; max number of iterations
   VectorXd z(3);           // Temporary container for x,y,z acceleration
   VectorXd x_(9);           // Temporary container for x,y,z acceleration
   char * buff = new char[10];

   // Ignore x,y,z line
   data.ignore(10, '\n');

   // Stop when 'a' key pressed
   while(iter <= test_len) {
     // Read data form the sensor_data
     // float * samples = read_sensor();

     // Read in acceleration for each direction
     data.getline(buff, 10, ',');
     z(0) = atof(buff);
     data.getline(buff, 10, ',');
     z(1) = atof(buff);
     data.getline(buff, 10, '\n');
     z(2) = atof(buff);

     // Amount of samples read from sensor
     // int len = 100;

     // float * velocity = (float *)malloc(sizeof(float)*len);
     // float * position = (float *)malloc(sizeof(float)*len);

     // velocity = numerical_int(samples, step, len);
     // position = numerical_int(velocity, step, len);

     // Run Kalman filer on data and write to file
     //for(int x = 0; x < len; x++) {
       kalman.update(z);
       x_ = kalman.state();

       for(int i = 0; i < 9; i++){
         if(i == 8) {
           file << x_(i) << std::endl;
         } else {
           file << x_(i) << ", ";
          }
       }

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

     //std::cout << "print" << std::endl;

     // testing
     iter++;

     //std::cin >> let;
     stop = let == 'a' || iter == test_len;
   }

   data.close();
   file.close();

   return 0;
}
