#include <bcm2835.h>
#include <iostream>
#include <bitset>
#include <fstream>

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
  cout<<"rate set to 1600 hz"<<endl;

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

  while(1){
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



    // convert to m/s^2 - default range is +-2g
    //so 0 = -2g, 2^10  = 2g
    // float newx = ((float)x/512)*(4*9.8) - (2*9.8);
    // float newy = ((float)y/512)*(4*9.8) - (2*9.8);
    // float newz = ((float)z/512)*(4*9.8) - (2*9.8);

    //write to out_file
    out_file<<x<<","<<y<<","<<z<<endl;
    cout<<x<<","<<y<<","<<z<<endl;
    // cout<<newx<<","<<newy<<","<<newz<<endl;
    // cout<<(float)x/1024<<","<<(float)y/1024<<","<<(floatz/1024<<endl;


  }
  return 0;
}
