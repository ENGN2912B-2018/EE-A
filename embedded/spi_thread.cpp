#include <iostream>
#include <thread>
#include <bcm2835.h>
#include "circularfifo_memory_sequential_consistent.hpp"
#include <bitset>

using namespace std;
using namespace memory_sequential_consistent;

void thread_spi(CircularFifo<char, 1000> *queue){
  //bcm setup
  bcm2835_init();
  //Setup SPI pins
  bcm2835_spi_begin();
  //Set CS pins polarity to low
  bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
  //Set SPI clock speed - http://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e
  bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);
  //Set SPI data mode - http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga8dd7bb496565324800130100e6bf6d86
  bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);
  //Set with CS pin to use for next transfers
  bcm2835_spi_chipSelect(BCM2835_SPI_CS0);


  //useful defines
  #define WRITE_SINGLE 0x00 //set r/w bit low and multibyte bit low - mask address with 0000 0000
  #define READ_SINGLE 0x80 //set r/w bit high and multibyte bit low - mask address with 1000 0000
  #define READ_MULTI 0xC0 //set r/w bit high and multibyte bit high - mask address with 1100 0000

  //================================================================================================================================
  // set up background for data read
  //================================================================================================================================

  //check if in correct power mode, if not: set
  #define POWER_CTL 0x2D
  #define MEASURE 0x08
  char read_powerctl[] = {READ_SINGLE|POWER_CTL, 0x00};
  bcm2835_spi_transfern(&read_powerctl[0], 2);

  if(read_powerctl[1]==0x00){
    char write_powerctl[] = {WRITE_SINGLE|POWER_CTL, MEASURE};
    bcm2835_spi_transfern(&write_powerctl[0], 2);
  }

  //data reg
  #define DATA 0x32

  // loop through data read and push
  cout<<"pre transfer"<<endl;
  for(int i = 0; i<10; i++){

    char read_xyz[] = {READ_MULTI|DATA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bcm2835_spi_transfern(&read_xyz[0], 7);

    //serially push x, y, z onto the q
    for(int n = 1; n<=6; n++){
      queue->push(read_xyz[n]);
    }

    int x = (read_xyz[1]<<2)|read_xyz[2];
    int y = (read_xyz[3]<<2)|read_xyz[4];
    int z = (read_xyz[5]<<2)|read_xyz[6];

    cout<<x<<" "<<y<<" "<<z<<endl;
  }
}


int main(){
  // init queue
  CircularFifo<char, 1000> queue;

  // create thread and pass buffer and length to it
  thread thread_obj(thread_spi, &queue);

  //check the output
  thread_obj.join();

  cout<<"post transfer"<<endl;
  while(1){
    char m;
    char data[6];

    for(int i = 0; i<6; i++){
      // allocate all chars to data array
      if (false == queue.pop(m)) {cout<<"q empty!"<<endl; exit(0);}
      else{
        data[i] = m;
      }

    }
      //reconstruct 10 bit ints and cout
      int x = (data[0]<<2)|data[1];
      int y = (data[2]<<2)|data[3];
      int z = (data[4]<<2)|data[5];

      cout<<x<<" "<<y<<" "<<z<<endl;


  }


  }
