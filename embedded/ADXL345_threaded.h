#include <bcm2835.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "circularfifo_memory_sequential_consistent.hpp"
#include <complex>
#include <liquid/liquid.h>


using namespace std;
using namespace memory_sequential_consistent;

class ADXL345{
public:
  //constructor
  ADXL345();

  //destructor
  ~ADXL345();


  //data methods
  void start(); //starts recording to the buffer and low passes
  float** read(int n); //pulls a n long chunk of samples


private:
  char* buffer;
  CircularFifo<float, 1000> queue; // queue for multithreading

  thread thread_obj; //thread obj

  firfilt_crcf qx; //filter for x channel
  firfilt_crcf qy; //filter for y channel
  firfilt_crcf qz; //filter for z channel


};
