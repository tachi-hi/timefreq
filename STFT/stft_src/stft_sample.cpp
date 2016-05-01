
#include "STFT.hpp"

int main(int argc, char **argv){
    const int n_frame = 200;
    const int n_freq = 300;
    const int frame_length = 256;
    const int frame_shift = frame_length / 4;

    //allocation
    twoDimArray<double> abs_spec(n_frame, n_freq);
    twoDimArray<std::complex<double> > phase_spec(n_frame, n_freq);

    // exec stft
    STFT stft(frame_length, frame_shift, n_frame);

    return 0;
}
