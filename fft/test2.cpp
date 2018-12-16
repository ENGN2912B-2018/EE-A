#include <liquid/liquid.h>
#include <complex>

int main() {
    // options
    unsigned int h_len=21;  // filter order
    float h[h_len];         // filter coefficients

    // ... initialize filter coefficients ...

    // create filter object
    firfilt_crcf q = firfilt_crcf_create(h,h_len);

    liquid_float_complex x;    // input sample
    liquid_float_complex y;    // output sample

    // execute filter (repeat as necessary)
    {
        firfilt_crcf_push(q, x);    // push input sample
        firfilt_crcf_execute(q,&y); // compute output
    }

    // destroy filter object
    firfilt_crcf_destroy(q);
}
