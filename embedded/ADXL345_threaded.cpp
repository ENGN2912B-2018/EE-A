#include "ADXL345_threaded.h"
#include "circularfifo_memory_sequential_consistent.hpp"
#include <thread>
#include <complex>
#include <vector>
#include <liquid/liquid.h>
#include <fstream>


// some nice useful defines
#define WRITE_SINGLE 0x00 //set r/w bit low and multibyte bit low - mask address with 0000 0000
#define READ_SINGLE 0x80 //set r/w bit high and multibyte bit low - mask address with 1000 0000
#define READ_MULTI 0xC0 //set r/w bit high and multibyte bit high - mask address with 1100 0000

using namespace std;
using namespace memory_sequential_consistent;


//multithreading method that pushes to the queue
void thread_spi(CircularFifo<float, 1000> *queue, firfilt_crcf qx, firfilt_crcf qy, firfilt_crcf qz){

  //check if in correct power mode, if not: set
  #define POWER_CTL 0x2D
  #define MEASURE 0x08
  char read_powerctl[] = {READ_SINGLE|POWER_CTL, 0x00};
  bcm2835_spi_transfern(&read_powerctl[0], 2);

  if(read_powerctl[1]==0x00){
    char write_powerctl[] = {WRITE_SINGLE|POWER_CTL, MEASURE};
    bcm2835_spi_transfern(&write_powerctl[0], 2);
  }

  //collect data and push to the queue

  #define DATA 0x32
  // loop through data read and push
  for(int i = 0; i<1000; i++){

    //read spi
    char read_xyz[] = {READ_MULTI|DATA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bcm2835_spi_transfern(&read_xyz[0], 7);

    //reconstruct the data correctly into floats
    int x_raw = (read_xyz[2]<<8)|read_xyz[1];
    int y_raw = (read_xyz[4]<<8)|read_xyz[3];
    int z_raw = (read_xyz[6]<<8)|read_xyz[5];

    bool xb = x_raw & 0x8000;
    bool yb = y_raw & 0x8000;
    bool zb = z_raw & 0x8000;

    float scale_factor = 3.9*(9.8/1000);

    //data in m/s/s
    float x_m = ((float)x_raw - 65536*(int)xb) * scale_factor;
    float y_m = ((float)y_raw - 65536*(int)yb) * scale_factor;
    float z_m = ((float)z_raw - 65536*(int)zb) * scale_factor;

    //low pass the output before q push
    // complex<float> inx;
    // complex<float> outx;
    // complex<float> iny;
    // complex<float> outy;
    // complex<float> inz;
    // complex<float> outz;
    //
    // // //filter x
    // firfilt_crcf_push(qx, inx);    // push input sample
    // firfilt_crcf_execute(qx,&outx); // compute output
    //
    // //filter x
    // firfilt_crcf_push(qy, iny);    // push input sample
    // firfilt_crcf_execute(qy,&outy); // compute output
    //
    // //filter x
    // firfilt_crcf_push(qz, inz);    // push input sample
    // firfilt_crcf_execute(qz,&outz); // compute output


    //serially push x, y, z onto the q
    queue->push(x_m);
    queue->push(y_m);
    queue->push(z_m);

    // cout<<i<<": "<<x_m<<", "<<y_m<<", "<<z_m<<endl;


  }
}

//constructor
ADXL345::ADXL345(){
  cout<<"constructor"<<endl;
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

  //set up filter

  //read in taps
  // ifstream taps_data("ftaps2.csv");
  // int taps_length = 325;
  // float taps[taps_length];
  // char x[10000];
  //
  // for(int i=0; i<taps_length; i++){
  //   taps_data.getline(x,10000,',');
  //   float y = atof(x);
  //   taps[i] = y;
  //   // cout<<i<<": "<<y<<endl;
  // }
  //
  // //instantiate filter with taps
  // qx = firfilt_crcf_create(taps,taps_length);
  // qy = firfilt_crcf_create(taps,taps_length);
  // qz = firfilt_crcf_create(taps,taps_length);

  //start collection thread
  cout<<"starting thread"<<endl;
  thread_obj = thread(thread_spi, &queue, qx, qy, qz);
  // thread_obj.detach();

}

ADXL345::~ADXL345(){
  thread_obj.join();
}


//start method inits a buffer and turns on the accelerometer
void ADXL345::start(){

  // create thread and pass buffer and length to it
  // thread thread_obj(thread_spi, &queue);

  // check the output
  // thread_obj.join();
}

float** ADXL345::read(int n){

  // necessary variables
  float** out_data = new float*[n];

  for(int i=0; i<n; i++){
    //assign float array
    out_data[i] = new float[3];

    //read x, y, z from q
    for(int j=0; j<3; j++){
      float m; //tmp float for q pop

      if (false == queue.pop(m)) {cout<<"q empty!"<<endl; break;}
      else{
        out_data[i][j] = m;
      }
    }

  }


  return out_data;
}



int main(){
  //instantiate accelerometer class
  cout<<"class up"<<endl;
  ADXL345 acc;

  int read_size = 10;

  //turn on the accelerometer and start adding to the queue
  // acc.start();

  //wait a second
  for(int i=0;i<10000000; i++){

  }

  float **results = acc.read(read_size);

  // //loop through the results a couple times to test reading
  for(int n = 0; n<read_size; n++){
      cout<<results[n][0]<<" "<<results[n][1]<<" "<<results[n][2]<<endl;
  }




}
