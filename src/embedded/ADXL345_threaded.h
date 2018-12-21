#include <bcm2835.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "circularfifo_memory_sequential_consistent.hpp"
#include <complex>


using namespace std;
using namespace memory_sequential_consistent;

class ADXL345{
public:
  //constructor
  ADXL345(int num_samples);

  //destructor
  ~ADXL345();


  //data methods
  void start(); //starts recording to the buffer and low passes
  float** read(int n); //pulls a n long chunk of samples
  float* calibrate(int n);

  //variables
  int qcounter = 0;
  bool start_gate = 0;


  char* buffer;
  CircularFifo<float, 500000> queue; // queue for multithreading

  thread thread_obj; //thread obj


};
