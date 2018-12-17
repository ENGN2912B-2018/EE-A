#include <bcm2835.h>
#include <iostream>
#include <bitset>

using namespace std;

int main(){

  bcm2835_init();
  //Setup SPI pins
	bcm2835_spi_begin();

	//Set CS pins polarity to low
	bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS0, 0);
	// bcm2835_spi_setChipSelectPolarity(BCM2835_SPI_CS1, 0);

	//Set SPI clock speed - http://www.airspayce.com/mikem/bcm2835/group__constants.html#gaf2e0ca069b8caef24602a02e8a00884e
	bcm2835_spi_setClockDivider(BCM2835_SPI_CLOCK_DIVIDER_256);

	//Set SPI data mode - http://www.airspayce.com/mikem/bcm2835/group__constants.html#ga8dd7bb496565324800130100e6bf6d86
	bcm2835_spi_setDataMode(BCM2835_SPI_MODE3);

	//Set with CS pin to use for next transfers
	bcm2835_spi_chipSelect(BCM2835_SPI_CS0);

//================================================================================================================================
// check the device id
//================================================================================================================================

cout<<"testing read on device id reg..."<<endl;

  //set up some useful stuff
  #define WRITE_SINGLE 0x00 //set r/w bit low and multibyte bit low - mask address with 0000 0000
  #define READ_SINGLE 0x80 //set r/w bit high and multibyte bit low - mask address with 1000 0000
  #define READ_MULTI 0xC0 //set r/w bit high and multibyte bit high - mask address with 1100 0000

  //check the device id to make sure things are working
  #define ID 0x00
  char read_id[] = {READ_SINGLE|ID,0x00};
  int size = 2;

  // Transfer  bytes
	bcm2835_spi_transfern(&read_id[0], size);			//data_buffer used for tx and rx

  // check result
  bitset<8> b1(read_id[0]), b2(read_id[1]);
  cout<<"device id from spi: "<<b2<<endl;
  cout<<"real device id: 11100101"<<endl;

  //================================================================================================================================
  // ok, now the real test, write to fifo_ctl reg and read to make sure the changes held
  //================================================================================================================================
  cout<<"testing write and read to fifo_ctl reg..."<<endl;

  #define FIFO_CTL 0x38
  #define MODE_1 0x5F //01 0 11111 sets mode: fifo, no trigger, 32 samples to watermark

  char set_fifo[] = {WRITE_SINGLE|FIFO_CTL, MODE_1};
  size=2;//2 bytes for write

  //now send it over
  cout<<"wrote: 01011111"<<endl;
  bcm2835_spi_transfern(&set_fifo[0], size);

  //then read the register to check our changes have taken
  char read_fifo[] = {READ_SINGLE|FIFO_CTL, 0x00}; //want an empty second byte to be overwritten
  size=2;//2 bytes for write

  bcm2835_spi_transfern(&read_fifo[0], size);

  //print the read data to check
  cout<<"read : ";
  bitset<8> bb1(read_fifo[0]), bb2(read_fifo[1]);
  cout<<bb2<<endl;


  return 0;
}
