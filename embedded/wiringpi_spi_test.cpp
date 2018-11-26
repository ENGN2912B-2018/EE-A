#include <iostream>
#include <wiringPi.h>
#include <wiringPiSPI.h>
#include<wiringPiI2C.h>
#include <bitset>

using namespace std;

int main(){
  // init wiringPi
  wiringPiSetup();
  //using spi channel 0 - raspberry pi has 2 channels (0 & 1)
  #define CHANNEL 0
  #define DATA_RATE 1000

  //run setup and check for failure
  if (wiringPiSPISetup (CHANNEL, DATA_RATE) < 0){
  cout<<"SPI Setup failed... exiting"<<endl;
  exit(1);
  }

  // read data, need a buffer that will be written on write and overwritten on read


  //need to set up the acc for proper operation - this means picking registers and writing to them
  // structure of a write is 2 bytes, where first byte is write select, 2nd bit ignore, and 6 bit address
  // second byte is the byte to be written to addressed register

  //set up some useful stuff
  #define WRITE_SINGLE 0x00 //set r/w bit low and multibyte bit low - mask address with 0000 0000
  #define READ_SINGLE 0x80 //set r/w bit high and multibyte bit low - mask address with 1000 0000
  #define READ_MULTI 0xC0 //set r/w bit high and multibyte bit high - mask address with 1100 0000

  //read test
  #define ID 0x00
  unsigned char read_id[] = {READ_SINGLE|ID,0x00};
  int size = 2;
  wiringPiSPIDataRW(CHANNEL,read_id,size);
  bitset<8> b1(read_id[0]),b2(read_id[1]);
  cout<<b1<<b2<
  <endl;



  // set 4 wire SPI
  // #define DATA_FORMAT 0x31
  //
  // //read data format reg to check what the mode is currently
  // unsigned char read_format[] = {READ_SINGLE|DATA_FORMAT, 0x00}; //want an empty second byte to be overwritten
  // int size=2;//2 bytes for write
  // //
  // // for(int i=0;i<10;i++){
  // //   cout<<"reading..."<<endl;
  //   wiringPiSPIDataRW(CHANNEL,read_format,size);
  //
  //   //print the read data to check
  //   cout<<"read in (bitset): "<<endl;
  //   bitset<8> f1(read_format[0]), f2(read_format[1]);
  //   cout<<f1<<f2<<endl;
  // // }

  // cout<<bitset<8>(READ_SINGLE|DATA_FORMAT)<<endl;

  //take the acc out of sleep mode

  //set fifo mode
  #define FIFO_CTL 0x38
  #define MODE_1 0x5F //01 0 11111 sets mode: fifo, no trigger, 32 samples to watermark

  unsigned char set_fifo[] = {WRITE_SINGLE|FIFO_CTL,MODE_1};
  int size=2;//2 bytes for write

  //now send it over
  cout<<"writing..."<<endl;
  wiringPiSPIDataRW(CHANNEL,set_fifo,size);

  //then read the register to check our changes have taken
  unsigned char read_fifo[] = {READ_SINGLE|FIFO_CTL, 0x00}; //want an empty second byte to be overwritten
  size=2;//2 bytes for write

    cout<<"reading..."<<endl;
    wiringPiSPIDataRW(CHANNEL,read_fifo,size);

    //print the read data to check
    cout<<"read in (bitset): "<<endl;
    bitset<8> b1(read_fifo[0]), b2(read_fifo[1]);
    cout<<b1<<b2<<endl;






  return 0;
}
