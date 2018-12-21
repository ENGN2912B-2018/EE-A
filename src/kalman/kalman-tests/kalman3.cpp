// Author: Kevin Anderson
// Date: Dec 12, 2018
// Code for Kalman filtering data from accelerometer on 3 variable state


#include "kalman.hpp"

//using namespace std;
using namespace Eigen;

// State is defined as position, velocity and acceleration on 3D axis
/*
 *    | x''   |
 *    | y''   |
 *    | z''   |
 */

int main(int argc, char * argv[]) {

    std::string inputfile = "../../embedded/trials/";
    if(argc == 2) {
      inputfile += argv[1];
    } else {
      inputfile += "stationary.csv";
    }

    std::cout << "reading from to: " << inputfile << std::endl;
  // Files for output
  std::fstream accf("acceleration.csv", std::fstream::out);
  std::fstream velf("velocity.csv", std::fstream::out);
  std::fstream posf("position.csv", std::fstream::out);
   // Create file stream
   std::fstream file("../testing output files/kalman.csv", std::fstream::out);
   std::fstream gain("../testing output files/gain.dat", std::fstream::out);
   // Read in values from file
   std::fstream data(inputfile, std::fstream::in);
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
   Q << .0002, 0, 0,
        0, .0002, 0,
        0, 0, .0002;
   R << 1, 0, 0,    // In reality need to get value from some steady state
        0, 1, 0,
        0, 0, 1;

   // Kalman filter
   KalmanFilter kalman(0, F, H, Q, R, P_init);
   kalman.init(0, x_init);    // Initialize state at t = 0

   // For testing, launch process to monitor key strokes (A - means stop
   // running filter)
   char let;
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
     std::cout << "skipping first line" << std::endl;
   }
   std::cout << "iterations: ";
   std::cin >> test_len;

   VectorXd vel_sum(3);
   VectorXd pos_sum(3);
   VectorXd vel(3);          // Temporary container for x,y,z velocity
   VectorXd vel_prev(3);     // Temporary container for x,y,z velocity
   VectorXd pos(3);          // Temporary container for x,y,z position
   VectorXd z(3);            // Temporary container for x,y,z acceleration
   VectorXd x_prev(3);       // Temporary container for x,y,z acceleration
   VectorXd x_(3);           // Temporary container for x,y,z acceleration

   pos << 0, 0, 0;
   vel << 0, 0, 0;
   vel_prev << 0, 0, 0;
   x_prev << 0, 0, 0;
   vel_sum << 0, 0, 0;
   pos_sum << 0, 0, 0;

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

/*
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
*/

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

   data.close();
   file.close();
   gain.close();
   return 0;
}
