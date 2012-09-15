
/* =============================================================================
  SDR, SIR, spectral distance
(c) 2012 Hideyuki Tachibana, tachi-hi @ GitHub
================================================================================*/

#pragma once
#include<cstdlib>
#include<cassert>
#include<cmath>

namespace SxR{
	typedef const double* Signal;

	double SDR(Signal X, Signal y, const int length);
	double SIR(Signal x, Signal y, Signal z, const int length);

	double segmental_SxR(Signal X, Signal y, 
		double (*func)(Signal, Signal, const int),
		const int length, const int step );

	double segmental_SxR(Signal X, Signal y, Signal z,
		double (*func)(Signal, Signal, Signal, const int),
		const int length, const int step );

	double KahanSum(const double* a, const int length);
}

// Spectral Distance
namespace specDist{
	typedef const double* Spectrum;
	double LSD(Spectrum, Spectrum, const int nyquist);
	double IS (Spectrum, Spectrum, const int nyquist);
	double GKL(Spectrum, Spectrum, const int nyquist);
}


