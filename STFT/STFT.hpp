
#pragma once

#include<complex>
#include<vector>
#include "myfft.h"
#include "twoDimArray.h"
//#include "waviostream.hpp"

// -------------------------------------------------------------------------------------------------
double sin_window (int i, int len){
	return 0.5 * (1 - cos(static_cast<double>(i) / len * 2. * M_PI));
}

class STFT{
public:
	STFT(int length, int shift, int n_frame, double(*winfunc)(int, int) = sin_window):
		length( length ),
		shift( shift ),
		n_frame( n_frame ),
		n_freq( length / 2 + 1 ),
		myfft( MyFFT::forward, length ),
		window( length )
	{
		for(int i = 0; i < length; i++){
			window[i] = winfunc(i, length);
		}
	}
	~STFT(){};
	void exec( //wavistream*,
		double*,
		twoDimArray<double> *,
		twoDimArray<std::complex<double> >*);
	int get_n_frame(void){return n_frame;}
	int get_n_freq (void){return n_freq; }

protected:
//	void shortTimeFFT(wavistream*, double*, std::complex<double>*);
	void shortTimeFFT(double*, double*, std::complex<double>*);
	int length;
	int shift;
	int n_frame;
	int n_freq;
	MyFFT myfft;
	std::vector<double> window;
};
