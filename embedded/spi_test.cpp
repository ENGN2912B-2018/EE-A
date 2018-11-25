#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include <bitset>

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


  //need to set up the acc for proper operation - this means picking registers and writing to them
  // structure of a write is 2 bytes, where first byte is write select, 2nd bit ignore, and 6 bit address
  // second byte is the byte to be written to addressed register

  //set up some useful defines
  #define WRITE_SINGLE 0x00 //set r/w bit low and multibyte bit low - mask address with 0000 0000
  #define READ_SINGLE 0x80 //set r/w bit high and multibyte bit low - mask address with 1000 0000
  #define READ_MULTI 0xC0 //set r/w bit high and multibyte bit high - mask address with 1100 0000


  // set 4 wire SPI

  //take the acc out of sleep mode

  //set fifo mode
  #define FIFO_CTL 0x38
  #define MODE_1 0x5F //01 0 11111 sets mode: fifo, no trigger, 32 samples to watermark

  //check stuff using bitset
  unsigned int send = ((WRITE_SINGLE|FIFO_CTL)<<8)|MODE_1;
  bitset<16> bits(send);
  cout<<bits<<endl;

  char buffer[] = {WRITE_SINGLE|FIFO_CTL,MODE_1};
  int size=2;//2 bytes for write

  //quick test again
  bitset<8> b1(buffer[0]), b2(buffer[1]);
  cout<<b1<<b2<<endl;




  // wiringPiSPIDataRW (buffer, size) ;

  //then read the register to check our changes have taken






  return 0;
}
