
#include "STFT.hpp"
#include<complex>

void STFT::exec(
//		wavistream* input,
		double* input,
		twoDimArray<double>* abs_spec_output,
		twoDimArray<std::complex<double> >* phase_spec_output)
{
//	input->read(length - shift);

	for(int t = 0; t < n_frame; ++t){
		shortTimeFFT(&(input[this->shift * t]), (*abs_spec_output)[t], (*phase_spec_output)[t]);
	}
}


void STFT::shortTimeFFT(
//		wavistream* input,
		double* data,
		double* abs_spec_short,
		std::complex<double>* phase_spec_short)
{
//	input->read(shift);
//	input->copy(static_cast<double*>(myfft.in), 0, length);
	for(int i = 0; i < length; i++){
		static_cast<double*>(myfft.in)[i] = data[i] * window[i]; // window function
	}
	myfft.exec();
	for(int f = 0; f < n_freq; f++){
		std::complex<double> cmp_spec = static_cast<std::complex<double>*>(myfft.out)[f];
		abs_spec_short[f] = abs(cmp_spec);
		phase_spec_short[f] = cmp_spec / (abs_spec_short[f] + 1e-100);
	}
}

double sin_window (int i, int len){
	return 0.5 * (1 - cos(static_cast<double>(i) / len * 2. * M_PI));
}
