#pragma once

#include "kalman_lib/kalman/kfilter.hpp"

// Author: Kevin Anderson
// Date:   December 8, 2018
// This is implementation of the Kalman fitler (linear) based off of the
// header file kfilter.hpp for a state described by position and velocity from
// accelerometer data. It provides implementation for all virtual functions in
// the superarent (EKFilter) and parent (KFilter) class.

using namespace Kalman;

// No optimations
template<typename T, K_UINT_32 BEG, bool OQ = false,
         bool OVR = false, bool DBG = false>
class Kalman : public KFilter<T, BEG, OQ, OVR, DBG> {
public:

  Kalman() {}
  ~Kalman() {
    // delete all member vairables
    /*
    delete x__;
    delete B;
    delete x;
    delete u;
    delete z;
    delete dz;
    delete A;
    delete W;
    delete Q;
    delete H;
    delete V;
    delete R;
    delete n;
    delete nu;
    delete nw;
    delete m;
    delete nv;
    */
  }

protected:

  // Matrix pre-creators
  /*
  void makeBaseA() {}

  void makeBaseB() {}

  void makeBaseW() {}

  void makeBaseQ() {}

  void makeBaseH() {}

  void makeBaseV() {}

  void makeBaseR() {}
  */

  // Matrix creators
  /*
  void makeA() {}

  void makeB() {}

  void makeW() {}

  void makeQ() {}

  void makeH() {}

  void makeV() {}

  void makeDZ() {}
  */


private:
/*
  void makeProcess() {}

  void makeMeasure() {}

  void sizeUpdate() {}
  */


}
