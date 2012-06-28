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

// =============================================================================
// Constant Q gabor Transform: Filter initialization
// =============================================================================
void CQ::filter_init(){
	// find the smallest 2^n which is larger than the integral range for lowest frequency
	frame_length = 1;
	while( frame_length < N_lowest() )
	{
		frame_length <<= 1;
	}

	// allocate fft
	n_split = ch / (freq_resolution * 12 * split_unit );
	fft.resize( n_split );
	for(int i = 0; i < n_split; i++){
		fft[i].init( frame_length >> ( split_unit * i ) );
	}
	spec.resize( frame_length );

	// allocate integral range
	integral_begin.resize(ch);
	integral_end.resize(ch);

	// initialize filter
	filter.alloc(
		(int)(2 * N_SIGMA_freq_domain * HF() / Q * (frame_length >> (n_split - 1) ) / sampling_rate) + 2,
		ch
	);

	for(int q = 0; q < n_split; q++)
	{
		for(int h = q * freq_resolution * split_unit * 12; 
			h < (q + 1) * freq_resolution * split_unit * 12; 
			++h)
		{
			const double cf = LF * pow( 2.,static_cast<double>(h)/freq_resolution/12.); // center frequency of each filter bank
			const double bw = cf / Q * 3. / 4. ; // bandwidth (standard deviation) of the filter bank

			// Calculate the integral range on frequency domain: N_SIGMA_freq_domain * sigma from center frequecy
			integral_begin[h] = (
				static_cast<int>(
					static_cast<double>( frame_length >> ( split_unit * q ) ) *
					std::max(
						(cf - bw * N_SIGMA_freq_domain) / sampling_rate,
						0.0
					)  // lower N_SIGMA_freq_domain sigma vs 0 Hz
				)
			);

			integral_end[h] = (
				static_cast<int>(
					static_cast<double>( frame_length >> ( split_unit * q ) ) * 
					std::min(
						(cf + bw * N_SIGMA_freq_domain) / sampling_rate,
						0.5
					) // upper N_SIGMA_freq_domain sigma vs Nyquist frequency
				)
			);

			// error case
			if (integral_end[h] < integral_begin[h])
			{
				integral_begin[h] = integral_end[h];
			}

			// Gaussian Filter on FFT domain
			for (int i = 0;
				i < integral_end[h] - integral_begin[h];
				++i) // ignore any i outside of the integral range for efficiency
			{
				// corresponding frequency of the bin
				double i_freq = 
					static_cast<double>( integral_begin[h] + i ) 
					/ (frame_length >> (split_unit * q) ) * sampling_rate;

				// angle of the complex number
				double angle = 
					M_PI * (cf - i_freq) 
					/ sampling_rate * (frame_length >> (split_unit * q) );
				filter[h][i] = std::complex<double>( cos(angle), sin(angle) );

				// amplitude: Gaussian
				double filter_of_this_bin =
					exp( - (i_freq - cf) * (i_freq - cf) / ( 2.0 * bw * bw )) 
					/ (sqrt(2.0 * M_PI) * bw);

				filter[h][i] *= filter_of_this_bin * bw * (1 << (split_unit * q));
			}
		}
	}
}

// =============================================================================
// input -> [transform] -> output.
// T is double or std::complex<double>
// =============================================================================
template<typename T>
void CQ::exec(const double *input, T *output){
	std::complex<double> tmp;
	for(int q = 0; q < n_split; ++q)
	{
		fft[q].forward(
			&(input[ frame_length/2 - (frame_length >> (split_unit * q + 1 ) ) ]),
			&(spec [ 0 ]));

		for(int h = q * freq_resolution * split_unit * 12;
			h < (q + 1) * freq_resolution * split_unit * 12;
			++h)
		{
			tmp = 0.;
			for (int i = 0;
				i < integral_end[h] - integral_begin[h];
				++i) // ignore any i outside of the integral range for efficiency
			{
				tmp += spec[ integral_begin[h] + i] * filter[h][i];
			}
			output[h] =
				typeid(T) == typeid(double)
				? abs(tmp)
				: *reinterpret_cast<T*>(&tmp); // cheat the compiler
		}
	}
}

