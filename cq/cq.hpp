/* =============================================================================
  Constant Q Transform using Gabor bases
(c) 2009, 2012 Hideyuki Tachibana, tachi-hi @ GitHub

Algorithm: 
 This algorithm outputs the inner product of 
 input signal s(t) and Gabor bases g(t; freq)
 i.e., 
   S(k) = < s(t), g(t; k)>
 where 
   g(t; k) = gaussian(t; 0, v(k) ) * exp(sqrt(-1)*k*t) .


I will add some more comments on the code soon.


See also:
 [1] J. Brown, "Calculation of a Constant Q Spectral Transform,"
     Journal of Acoustic Society of America, vol. 89, no. 1, pp. 425--434, 1991.

 [2] J. C. Brown and M. S. Puckette, "An Efficient Algorithm for the Calculation of 
     a Constant Q Transform," Journal of Acoustic Society of America, vol. 92,
     no. 5, pp. 2698--2701, 1992.

================================================================================*/


#pragma once

#include <cstdio>
#include <cstdlib>
#include <cmath>
#include <complex>
#include <typeinfo>
#include "../fft/fft.hpp"

// =============================================================================
// filterbank.
// =============================================================================
template <typename T>
class FILTER_BANK{
public:
	FILTER_BANK(){};
	void alloc(int N_frame, int ch)
	{
		this -> N_frame = N_frame;
		filter_bank.resize( N_frame * ch );
	}
	~FILTER_BANK(){}
	T* operator[](int h){return &(filter_bank[N_frame * h]); }
private:
	int N_frame;
	std::vector<T> filter_bank;
};

// =============================================================================
// Constant Q gabor Transform
// =============================================================================
class CQ{
 public:
	CQ(){};
	~CQ(){};
	void filter_init();

	// set
	void set_sampling_rate( unsigned int sfrq ){ this->sampling_rate = sfrq; }
	void set_LF( double LF ){ this->LF = LF; }
	void set_ch( unsigned short ch ){ this->ch = ch; }
	// resolution: semitone / x, i.e., if x = 2, frequency resolution is half the semitone.
	void set_freq_resolution( unsigned short x ){ this->freq_resolution = x; } 
	void set_Q( double Q ){ this-> Q = Q; }

	// get
	int get_frame_length(){return frame_length;}

	// exec
	template <typename T> void exec(const double *input_singal, T *output_spectrum); // T should be double or std::complex<double>

 protected:
	// constants that are not used very frequently
	double HF() // Highest frequency
	{
		return LF * pow(2.0, static_cast<double>(ch - 1) / 12.0 / freq_resolution);
	}
	unsigned int N_lowest()
	{
		return (int)(Q / LF * N_SIGMA_time_domain * sampling_rate * (4. / 3.) / (2 * M_PI) );
	}

	// filter
	FILTER_BANK<std::complex<double> > filter; // filterbank

	// Integral range on FFT domain
	std::vector<int> integral_begin;
	std::vector<int> integral_end;

	// fft
	std::vector<FFT> fft;

	// constants
	unsigned int sampling_rate; 
	double LF; // lowest frequency of the analysis
	unsigned short ch; // number of frequency bands
	unsigned short freq_resolution;
	double Q;

	int n_split;
	static const int split_unit = 3; // spit processing by 3 octaves for efficiency.

	unsigned int frame_length;
	std::vector<std::complex<double> > spec;
	static const double N_SIGMA_time_domain = 3.; // Ignore outside of 3 sigma of Gaussian.
	static const double N_SIGMA_freq_domain = 3.; // Ignore outside of 3 sigma of Gaussian.
};


