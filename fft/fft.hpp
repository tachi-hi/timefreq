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
	void init(int length);

	void forward (const double *input, std::complex<double> *output) {fft_(input, output, true); }
	void inverse (const std::complex<double> *input, double *output) {fft_(input, output, false); }

 private:	
	template<class S, class T> void fft_ (const S *input, T *output, const bool flag);

	std::vector<double> COS;
	std::vector<double> SIN;
	std::vector<std::complex<double> > buffer;
	int length;
};

void FFT::init(int length)
{
	this->length = length;
	for(int i = 0; i < length; i++)
	{
		COS.push_back( cos(2. * M_PI * (double)i/length) );
		SIN.push_back( sin(2. * M_PI * (double)i/length) );
	}
	buffer.resize( length );
}

template<class S, class T>
void FFT::fft_(const S *input, T *output, const bool flag){
	if (length < 2 || (length & (length - 1)) != 0){
		fprintf(stderr, "Error in fft.hpp. Size of input data is not 2^n");
		return;
	}

	std::complex<double> *alias = typeid(T) != typeid(std::complex<double>)
		? &(buffer[0])
		: reinterpret_cast<std::complex<double>*>(output); // cheat complier when T is double.

	const double sign = flag ? 1.0 : -1.0;

	for(int i = 0; i < length; i++)
	{
		alias[i] = input[i];
	}

	std::complex<double> t;
	for(int len = length; len >= 1; len >>= 1){
		int angle = 0;
		int angle_hop = length / len;
		for(int k = 0; 
				k < (len >> 1); 
				++k, angle += angle_hop)
		{
			std::complex<double> W( COS[ angle ], sign * SIN[ angle ] );
			int p = k;
			int q = k + (len >> 1);
			for(int j = len; 
					j <= length; 
					j += len, p += len, q += len)
			{
				t = alias[p] - alias[q];
				alias[p] += alias[q];
				alias[q] = t * W;
			}
		}
	}

	int j = 1;
	int p = 0;
	for(int i = 1 ; 
			i <= length - 1 ; 
			++i, ++p)
	{
		if( i < j ){
			t = alias[p];
			alias[p] = alias[ j - 1 ];
			alias[ j - 1 ] = t;
		}
		int k;
		for(k = length >> 1; k < j; j -= k, k >>= 1)
			;
		j += k;
	}


	if(flag){
		;
	}else{
		for(int i = 0; i < length; ++i)
			output[i] = alias[i].real() / length;
	}

	return;
}

