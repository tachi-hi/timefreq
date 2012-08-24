/*=================================================================
 (C) 2009, 2012 Hideyuki Tachibana, tachi-hi @ GitHub
 FFT 
==================================================================*/

#pragma once

#include<complex>
#include<cstdio>
#include<vector>
#include<typeinfo>

class FFT{
 public:
	FFT(){}
	~FFT(){}
	void init(const int length);

	void forward (const double *input, std::complex<double> *output) {fft_(input, output, true); }
	void inverse (const std::complex<double> *input, double *output) {fft_(input, output, false); }

 private:	
	template<class S, class T> void fft_ (const S *input, T *output, const bool flag);

	std::vector<double> COS;
	std::vector<double> SIN;
	std::vector<std::complex<double> > buffer;
	int length;
};


