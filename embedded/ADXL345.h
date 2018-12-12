#include <bcm2835.h>
#include <iostream>
#include <vector>
#include <string>
#include "circularfifo_memory_sequential_consistent.hpp"


using namespace std;
using namespace memory_sequential_consistent;

class ADXL345{
public:
  //constructor
  ADXL345(int data_rate, int spi_rate, int range);

  //functions to set custom reg values if needed
  void write_reg(char reg, char data);
  char* read_reg_single(char reg);
  char* read_reg_multi(char reg);

  //data methods
  void start(); //starts recording to the buffer
  int** read(int n);


private:
  char* buffer;
  // void thread_spi(CircularFifo<char*, 1000> *queue);
  CircularFifo<char, 1000> queue;
  // CircularFifo<char*, 1000> queue;

};
