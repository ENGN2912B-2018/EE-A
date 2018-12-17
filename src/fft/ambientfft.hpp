pragma once

// Can force gcc to use native complex data structure by including <complex.h> before fftw
#include <fstream>
#include <string>
#include "../FFTW/fftw3.h"
#include <complex>

// Read in audio; return 0 if success, 1 otherwise
double* read_audio();

// Compute fft of signal
void computefft();

// main function, reads in audio, fft, writes output to file
void ambientfft();
