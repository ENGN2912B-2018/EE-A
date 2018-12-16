#include <bcm2835.h>
#include <iostream>
#include <bitset>
#include <fstream>
#include <complex>
#include <vector>
#include <liquid/liquid.h>

using namespace std;

int main(){

  //bcm setup

  bcm2835_init();
  //Setup SPI pins
	bcm2835_spi_begin();
	//Set CS pins polarity to low
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	//Set SPI clock speed - http://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_128); //run at 3k
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

  //check fifo status, samples should be piling
  #define FIFO_STATUS 0x39
  char read_fifostatus[] = {READ_SINGLE|FIFO_STATUS, 0x00};
  bcm2835_spi_transfern(&read_fifostatus[0], 2);
  bitset<8> status(read_fifostatus[1]);
  cout<<"status: "<<status<<endl;

  //set data rate
  #define BW_RATE 0x2C
  #define RATE 0x0F
  char set_data_rate[] = {WRITE_SINGLE|BW_RATE, RATE};
  bcm2835_spi_transfern(&set_data_rate[0], 2);
  cout<<"rate set to 3200 hz"<<endl;

  //set data format to be left justified
  #define DATA_FORMAT 0x31
  #define FORMAT 0x00  //want it to be 0000 0100 to set left justify bit
  char set_data_format[] = {WRITE_SINGLE|DATA_FORMAT, FORMAT};
  bcm2835_spi_transfern(&set_data_format[0], 2);
  cout<<"set left justified"<<endl;



  //================================================================================================================================
  // data read
  //================================================================================================================================

  #define DATA 0x32

  //open a file to be continuously written
  ofstream out_file;
  out_file.open("trial.csv", ios::trunc);

  out_file<<"x,y,z"<<endl;

  //set up an array for the output to be sent to filtered
  vector<vector<float>> output_data;

  int time_end = 10000; //# of seconds * spi rate


  for(int n = 0; n<time_end; n++){
    char read_xyz[] = {READ_MULTI|DATA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bcm2835_spi_transfern(&read_xyz[0], 7);

    // bitset<8> b1(read_xyz[2]), b2(read_xyz[1]);
    // cout<<b1<<" "<<b2<<endl;

    int x_raw = (read_xyz[2]<<8)|read_xyz[1];
    int y_raw = (read_xyz[4]<<8)|read_xyz[3];
    int z_raw = (read_xyz[6]<<8)|read_xyz[5];

    bool xb = x_raw & 0x8000;
    bool yb = y_raw & 0x8000;
    bool zb = z_raw & 0x8000;

    float scale_factor = 3.9*(9.8/1000);

    float x = ((float)x_raw - 65536*(int)xb) * scale_factor;
    float y = ((float)y_raw - 65536*(int)yb) * scale_factor;
    float z = ((float)z_raw - 65536*(int)zb) * scale_factor;

    //write to out_file
    out_file<<x<<","<<y<<","<<z<<endl;
    // cout<<x<<","<<y<<","<<z<<endl;

    // save to output vector
    vector<float> output_array = {x, y, z};

    // cout<<output_array[0]<<endl;

    output_data.push_back(output_array);

  }

  //check output data
  for(int n = 0; n<output_data.size(); n++){
    // cout<<output_data[n][0]<<endl;
  }


  // lowpass filter
  //read taps in
  ifstream taps_data("filtertaps.csv");
  int taps_length = 91;
  float taps[taps_length];
  char x[10000];

  for(int i=0; i<taps_length; i++){
    taps_data.getline(x,100001,',');
    float y = atof(x);
    taps[i] = y;
    // cout<<i<<": "<<y<<endl;
  }


  // create output array
  // complex<float> output[output_data.size()];
  vector<complex<float>> filtered(3*output_data.size());

  // create filter object
  firfilt_crcf q = firfilt_crcf_create(taps,taps_length);

  //filter x
  for(int x = 0; x<output_data.size(); x++){
    complex<float> in = output_data[x][0];    // input sample
    complex<float> out;    // output sample
    // complex<float> in;// = signal[n];    // input sample
    // complex<float> out;// = output[n];    // output sample
    // cout<<in<<endl;

    firfilt_crcf_push(q, in);    // push input sample
    firfilt_crcf_execute(q,&out); // compute output

    // cout <<out;

    filtered[x] = out;
  }

  //filter y
  for(int y = 0; y<output_data.size(); y++){
    complex<float> in = output_data[y][1];    // input sample
    complex<float> out;    // output sample
    // complex<float> in;// = signal[n];    // input sample
    // complex<float> out;// = output[n];    // output sample
    // cout<<in<<endl;

    firfilt_crcf_push(q, in);    // push input sample
    firfilt_crcf_execute(q,&out); // compute output

    // cout <<out;

    filtered[y+output_data.size()] = out;
  }

  //filter z
  for(int z = 0; z<output_data.size(); z++){
    complex<float> in = output_data[z][2];    // input sample
    complex<float> out;    // output sample
    // complex<float> in;// = signal[n];    // input sample
    // complex<float> out;// = output[n];    // output sample
    // cout<<in<<endl;

    firfilt_crcf_push(q, in);    // push input sample
    firfilt_crcf_execute(q,&out); // compute output

    // cout <<out;

    filtered[z+2*output_data.size()] = out;
  }

  // destroy filter object
  firfilt_crcf_destroy(q);

  //save
  ofstream writer("filtered.csv");
  for(int j = 0; j< output_data.size(); j++){
    // cout<<output.real()<<endl;
    writer<<filtered[j].real()<<","<<filtered[j+output_data.size()].real()<<","<<filtered[j+2*output_data.size()].real()<<endl;
  }


  return 0;
}
