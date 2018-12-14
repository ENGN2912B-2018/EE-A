#include <bcm2835.h>
#include <iostream>
#include <bitset>

using namespace std;

int main(){

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

  //check fifo status, samples should be piling
  #define FIFO_STATUS 0x39
  char read_fifostatus[] = {READ_SINGLE|FIFO_STATUS, 0x00};
  bcm2835_spi_transfern(&read_fifostatus[0], 2);
  bitset<8> status(read_fifostatus[1]);
  cout<<"status: "<<status<<endl;

  //================================================================================================================================
  // data read
  //================================================================================================================================

  #define DATA 0x32

  while(1){
    char read_xyz[] = {READ_MULTI|DATA, 0x00, 0x00, 0x00, 0x00, 0x00, 0x00};
    bcm2835_spi_transfern(&read_xyz[0], 7);

    // bitset<13> xb((read_xyz[1]<<5)|read_xyz[2]),
    //           yb((read_xyz[3]<<5)|read_xyz[4]),
    //           zb((read_xyz[5]<<5)|read_xyz[6]);
    //
    // cout<<"x: "<<xb<<endl;
    // cout<<"y: "<<yb<<endl;
    // cout<<"z: "<<zb<<endl;

    int x  = (read_xyz[1]<<2)|read_xyz[2];
    int y  = (read_xyz[3]<<2)|read_xyz[4];
    int z  = (read_xyz[5]<<2)|read_xyz[6];

    // convert to m/s^2



    cout<<"x: "<<x<<"  |  ";
    cout<<"y: "<<y<<"  |  ";
    cout<<"z: "<<z<<endl;
  }
  return 0;
}
