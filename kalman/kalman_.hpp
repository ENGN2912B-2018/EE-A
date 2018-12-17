/**
* Kalman filter implementation using Eigen. Based on the following
* introductory paper:
*
*     http://www.cs.unc.edu/~welch/media/pdf/kalman_intro.pdf
*
* @author: Hayk Martirosyan
* @date: 2014.11.15
*/

/*
 * Header file containing class for Kalman filter based information above
 *
 * Adopted for state specified with position ande velocity
 *
 */

#pragma once

#include <string>
#include <fstream>
#include <iostream>
#include <vector>
#include <cmath>
#include "numerical.hpp"
#include "../Eigen/Dense"

class KalmanFilter {
public:

  /**
  * Create a blank estimator.
  */
  KalmanFilter() {}

  /**
  * Create a Kalman filter with the specified matrices.
  *   F - System dynamics matrix
  *   H - Output matrix
  *   Q - Process noise covariance
  *   R - Measurement noise covariance
  *   P - Estimate error covariance
  */
  KalmanFilter(
      double dt,
      const Eigen::MatrixXd& F,
      const Eigen::MatrixXd& H,
      const Eigen::MatrixXd& Q,   // Covariance matrix describing noise (uncertainty from the environment)
      const Eigen::MatrixXd& R,   // Sensor noise
      const Eigen::MatrixXd& P
  ) : F(F), H(H), Q(Q), R(R), P0(P),
    m(H.rows()), n(F.rows()), dt(dt), initialized(false),
    I(n, n), x_hat(n), x_hat_new(n)
  {
    I.setIdentity();
  }


  /**
  * Initialize the filter with initial states as zero.
  */
  void init() {
    x_hat.setZero();
    P = P0;
    t0 = 0;
    t = t0;
    initialized = true;
  }

  /**
  * Initialize the filter with a guess for initial states.
  */
  void init(double t0, const Eigen::VectorXd& x0) {
    x_hat = x0;
    P = P0;
    this->t0 = t0;
    t = t0;
    initialized = true;
  }


  /**
  * Update the estimated state based on measured values. The
  * time step is assumed to remain constant.
  */
  void update(const Eigen::VectorXd& y) {

    if(!initialized)
      throw std::runtime_error("Filter is not initialized!");

    x_hat_new = F * x_hat;
    P = F*P*F.transpose() + Q;
    K = P*H.transpose()*(H*P*H.transpose() + R).inverse();
    x_hat_new += K * (y - H*x_hat_new);

/*
    Eigen::VectorXd temp =  K * (y - H*x_hat_new);
    for(int i = 0; i < 9; i++) {
      if(i == 8) {
        std::cout << temp(i) << std::endl;
      } else {
        std::cout << temp(i) << ", ";
       }
    }
*/

    P = (I - K*H)*P;
    x_hat = x_hat_new;
    t += dt;
  }

  /**
  * Update the estimated state based on measured values,
  * using the given time step and dynamics matrix.
  */
  void update(const Eigen::VectorXd& y, double dt, const Eigen::MatrixXd F) {
    this->F = F;
    this->dt = dt;
    update(y);
  }

  /**
  * Return the current state and time.
  */
  Eigen::VectorXd state() {
    return this->x_hat;
  };

  /*
   * Return the covariance matrix
   */
   Eigen::MatrixXd covP() {
     return this->P;
   }

   /*
    * Return Kalman gain
    */
    Eigen::MatrixXd gain() {
      return this->K;
    }

   /*
    * Return the current time
    */
   double time() {
     return t;
   };
private:

  // Matrices for computation
  Eigen::MatrixXd F, H, Q, R, P, K, P0;

  // System dimensions
  int m, n;

  // Initial and current time
  double t0, t;

  // Discrete time step
  double dt;

  // Is the filter initialized?
  bool initialized;

  // n-size identity
  Eigen::MatrixXd I;

  // Estimated states
  Eigen::VectorXd x_hat, x_hat_new;
};
