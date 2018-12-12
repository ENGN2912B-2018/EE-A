#include "ADXL345.h"
#include <thread>
#include "circularfifo_memory_sequential_consistent.hpp"


// some nice useful defines
#define WRITE_SINGLE 0x00 //set r/w bit low and multibyte bit low - mask address with 0000 0000
#define READ_SINGLE 0x80 //set r/w bit high and multibyte bit low - mask address with 1000 0000
#define READ_MULTI 0xC0 //set r/w bit high and multibyte bit high - mask address with 1100 0000

using namespace std;
using namespace memory_sequential_consistent;


//multithreading method that pushes to the queue
void thread_spi(CircularFifo<int*, 1000> *queue){

  //check if in correct power mode, if not: set
  #define POWER_CTL 0x2D
  #define MEASURE 0x08
  char read_powerctl[] = {READ_SINGLE|POWER_CTL, 0x00};
  bcm2835_spi_transfern(&read_powerctl[0], 2);

  if(read_powerctl[1]==0x00){
    char write_powerctl[] = {WRITE_SINGLE|POWER_CTL, MEASURE};
    bcm2835_spi_transfern(&write_powerctl[0], 2);
  }

  //================================================================================================================================
  // data read and q push
  //================================================================================================================================

  #define DATA 0x32

  // loop through data read and push
  for(int i = 0; i<100; i++){

    char read_xyz[] = {READ_MULTI|DATA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bcm2835_spi_transfern(&read_xyz[0], 7);

    int x  = (read_xyz[1]<<2)|read_xyz[2];
    int y  = (read_xyz[3]<<2)|read_xyz[4];
    int z  = (read_xyz[5]<<2)|read_xyz[6];

    // cout<<"x: "<<x<<"  |  ";
    // cout<<"y: "<<y<<"  |  ";
    // cout<<"z: "<<z<<endl;

    int data[] = {x, y, z};

    cout<<"read before going through queue..."<<endl;
    cout<<"x: "<<data[0]<<"  |  ";
    cout<<"y: "<<data[1]<<"  |  ";
    cout<<"z: "<<data[2]<<endl;

    // char data[] = {read_xyz[1], read_xyz[2], read_xyz[3], read_xyz[4], read_xyz[5], read_xyz[6]};

    queue->push(data);
  }
}

//constructor
ADXL345::ADXL345(int data_rate, int spi_rate, int range){
  //first, set up bcm stuff for spi
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

  //set fifo mode to collect 32 samples, then stop collecting until data is read
  #define FIFO_CTL 0x38
  #define MODE_1 0x5F //01 0 11111 sets mode: fifo, no trigger, 32 samples to watermark

  char set_fifo[] = {WRITE_SINGLE|FIFO_CTL, MODE_1};
  bcm2835_spi_transfern(&set_fifo[0], 2);

  //set data rate

  //set range

}

//start method inits a buffer and turns on the accelerometer
void ADXL345::start(){

  // create thread and pass buffer and length to it
  thread thread_obj(thread_spi, &queue);
  // check the output
  thread_obj.join();
}

int** ADXL345::read(int n){

  int x, y, z;
  int** out_data = new int*[n];

  for(int i=0; i<n; i++){
    // char* m;
    int* m;
    if(false == queue.pop(m)) {cout<<"q empty!"<<endl; break;}
    else{
      int read_results[] = {m[0], m[1], m[2]};
      out_data[i] = read_results;

      cout<<"read after going through queue..."<<endl;
      cout<<"x: "<<m[0]<<"  |  ";
      cout<<"y: "<<m[1]<<"  |  ";
      cout<<"z: "<<m[2]<<endl;
    }

  }

  return out_data;
}



//functions to set custom reg values if needed

// write to arbitrary register
void ADXL345::write_reg(char reg, char data){

}

// read arbitrary register
char* ADXL345::read_reg_single(char reg){

}

char* ADXL345::read_reg_multi(char reg){
  //don't know if this is necessary, given only multi reg read is on data colleciton

}



int main(){
  ADXL345 acc(0,0,0);

  acc.start();
  int **results = acc.read(10);

  // while(1){
  //   int **results = acc.read(10);
  //
  //   //print out
  //   for(int i = 0; i<10; i++){
  //     for(int j = 0; j < 3; j++){
  //       cout<<results[i][j]<<" ";
  //     }
  //     cout<<endl;
  //   }
  // }


}
