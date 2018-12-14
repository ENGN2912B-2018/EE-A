#include "numercial.hpp"

/*
 * Computes numerical integration by finding area under curve
 * using a trapezoidal Riemann Sum.

 * sample - array of the data samples
 * step - interval over which consecutively samples were taken
 */
float * numerical_int(float* sample, float step, int len) {
  float * ret = (float *) malloc(len*sizeof(float));

  for(int x = 0; x < len-1; x++) {
    ret[x] = 0.5*step*(sample[x] + sample[x+1]);
  }

  return ret;
}
