#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>

using namespace std;

int main(){
  // init wiringPi
  wiringPiSetup();
  //using spi channel 0 - raspberry pi has 2 channels (0 & 1)
  #define CHANNEL 0
  #define DATA_RATE 1000000

  //run setup and check for failure
  if (wiringPiSPISetup (CHANNEL, DATA_RATE) < 0){
  cout<<"SPI Setup failed... exiting"<<endl;
  exit(1);
  }

  // read data, need a buffer that will be written on write and overwritten on read
  char* buffer;
  wiringPiSPIDataRW (buffer, size) ;

  //need to set up the acc for proper operation - this means picking registers and writing to them
  // structure of a write is 2 bytes, where first byte is write select, 2nd bit ignore, and 6 bit address
  // second byte is the byte to be written to addressed register

  //set up some useful defines
  #define WRITE_SINGLE 0x40 //set write bit high and multibyte bit low - mask address with 0100 0000

  //set 4 wire SPI

  //take the acc out of sleep mode

  //set fifo mode
  #define FIFO_CTL 0x38
  #define MODE_1 0x5F //01 0 11111 sets mode: fifo, no trigger, 32 samples to watermark








  return 0;
}
