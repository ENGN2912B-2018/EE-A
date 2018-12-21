#include "numerical.hpp"
#include <cstdlib>


using namespace std;
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

int main(int argc, char *argv[]) {

  int fs = 3200;
  float step = 1.0/fs;


  int iter = 0;
  int num_samples = 5;

  if(argc == 2) {
    num_samples = atoi(argv[1]);
  }

  float x_p = 0;    // previous sample
  float x_c = 0;    // current sample
  double val = 0.0;

  char num[20];

  fstream infile("./numdata.dat", ios::in);
  fstream outfile("./testing output files/numericaltest.dat", ios::out);

  while(iter <= num_samples) {
     infile.getline(num, 20, '\n');

     x_c = atof(num);
     val = val + 0.5*step*(x_p + x_c);
     outfile << val << endl;
     x_p = x_c;

     iter++; // Increment
  }
}
