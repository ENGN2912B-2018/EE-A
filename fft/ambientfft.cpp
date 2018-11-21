/*
 * Author: Kevin Anderson
 * Date: November 15, 2018
 * Implementation of FFT of sound file
 */

using namespace std;

/*
 * Read in audio signal
 */
double* read_audio(string filename) {
  double signal[100];

  fstream file(filename, ios::in);
  file.read(signal, 100);

  return signal;
}

/*
 * Computes the FFT of signal and returns the frequency content
 */
void computefft(int N, double* signal, fftw_complex* out)  {

    // Plan for FFT algoirithm (data structure for parameters)
    fftw_plan p;

    p = fftw_plan_dft_r2c_1d(N, signal, out, FFTW_FORWARD, FFTW_ESTIMATE);

    // Ensure plan was created
    assert(p != NULL);

    // Run FFT
    fftw_execute(p);
    fftw_destroy_plan(p);

    // Free memory for input/ouput arrays
    fftw_free(in);
    fftw_free(out);
}

/*
 * Compute fft for ambient sound recording and write output to file
 */
void ambientfft() {
  // Allocate memory for input/output data (arrays)
  fftw_complex *out;

  // Output file
  fstream file("dataout.txt", ios::out);

  double signal = read_audio("../test_signal.txt");

  //in = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);
  out = (fftw_complex*) fftw_malloc(sizeof(fftw_complex) * N);

  // Compute fft
  compute(100, signal, out);

  // Write to output file
  for(int x = 0; x < sizeof(out)/sizeof(*out); x++) {
    file << out[x][0] << " " << out[x][1] << endl;
  }
  file.close();
}
