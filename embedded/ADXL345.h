#include <bcm2835.h>
#include <iostream>
#include <vector>
#include <string>
#include <thread>
#include "circularfifo_memory_sequential_consistent.hpp"


using namespace std;
using namespace memory_sequential_consistent;

class ADXL345{
public:
  //constructor
  ADXL345();

  //data methods
  void start(); //starts recording to the buffer
  int** read(int n);


private:
  char* buffer;
  // void thread_spi(CircularFifo<char*, 1000> *queue);
  CircularFifo<char, 1000> queue;
  // CircularFifo<char*, 1000> queue;
  // thread thread_obj;

};
