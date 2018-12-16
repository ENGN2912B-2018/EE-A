#include <complex>
#include <liquid/liquid.h>
// #include <vector>
#include <fstream>
#include <iostream>


using namespace std;

int main() {

    //create test signal and read
    ifstream data("songsegment.csv");

    int data_length = 100001;
    float signal[data_length];
    char* x = new char[100001];

    // if(data.is_open()){cout<<"open"<<endl;}
    for(int i=0; i<data_length;i++){
      data.getline(x,100001,'\n');
      float y = atof(x);
      signal[i] = y;
      // cout<<i<<": "<<y<<endl;
    }

    //read taps in
    ifstream taps_data("filtertaps.csv");
    int taps_length = 91;
    float taps[taps_length];

    for(int i=0; i<taps_length; i++){
      taps_data.getline(x,100001,',');
      float y = atof(x);
      taps[i] = y;
      // cout<<i<<": "<<y<<endl;
    }



    // create output array
    complex<float> output[data_length];

    // create filter object
    firfilt_crcf q = firfilt_crcf_create(taps,taps_length);


    for(int n = 0; n<data_length; n++){
      complex<float> in = signal[n];    // input sample
      complex<float> out;    // output sample
      // complex<float> in;// = signal[n];    // input sample
      // complex<float> out;// = output[n];    // output sample
      // cout<<in<<endl;

      firfilt_crcf_push(q, in);    // push input sample
      firfilt_crcf_execute(q,&out); // compute output



      // cout<<out<<endl;

      output[n] = out;
    }
    // destroy filter object
    firfilt_crcf_destroy(q);


    //save
    ofstream writer("filtered.csv");
    for(int j = 0; j< data_length; j++){
      // cout<<output.real()<<endl;
      writer<<output[j].real()<<endl;
    }


}
