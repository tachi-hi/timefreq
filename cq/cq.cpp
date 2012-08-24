/* =============================================================================
  Constant Q Transform using Gabor bases
(c) 2009, 2012 Hideyuki Tachibana, tachi-hi @ GitHub
================================================================================*/

#include "cq.hpp"


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

template void CQ::exec(const double*, double*);
template void CQ::exec(const double*, std::complex<double>*);


