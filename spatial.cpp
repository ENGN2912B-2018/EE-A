#include "kalman/kalman_.hpp"
#include "embedded/ADXL345_threaded.h"
#include "embedded/circularfifo_hazard_platform_dependent.hpp"
#include "embedded/circularfifo_memory_relaxed_acquire_release.hpp"
#include "embedded/circularfifo_memory_sequential_consistent.hpp"

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



  return 0;
}
