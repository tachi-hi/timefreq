
/* =============================================================================
  SDR, SIR, spectral distance
(c) 2012 Hideyuki Tachibana, tachi-hi @ GitHub
================================================================================*/

#include"snr.hpp"

// ===========================================================
// Signal to Distortion Ratio
// ===========================================================
double SxR::SDR(Signal X, Signal y, const int length){
	assert(length > 1);

	double sum_Xy, sum_XX, sum_yy;
	sum_Xy = sum_XX = sum_yy = 0.;

	for(int i = 0; i < length; i++){
		sum_Xy += X[i] * y[i];
		sum_XX += X[i] * X[i];
		sum_yy += y[i] * y[i];
	}
	double SDR = 10 / log(10) * 
	log((sum_Xy * sum_Xy) / (sum_XX * sum_yy - sum_Xy * sum_Xy));

	return SDR;
}

// ===========================================================
// Signal to Interference Ratio
// ===========================================================
double SxR::SIR(Signal s, Signal x, Signal y, const int length){
	assert(length > 1);

	double xx, yy, xy, sx, sy;
	xx = yy = xy = sx = sy = 0.;
	for(int i = 0; i < length; i++){
		xx += x[i] * x[i];
		xy += x[i] * y[i];
		yy += y[i] * y[i];
		sx += s[i] * x[i];
		sy += s[i] * y[i];
	}
	double a = (yy * sx - xy * sy)/(xx * yy - xy * xy);
	double b = (xx * sy - xy * sx)/(xx * yy - xy * xy);
	double SIR = 10 / log(10) * log( (a * a * xx)/(b * b * yy) );
	return SIR;
}


// ===========================================================
double SxR::segmental_SxR(
	Signal X,
	Signal y,
	double (*func)(Signal, Signal, const int),
	const int length,
	const int step )
{
	const int m = length % step;
	const int n = length / step + 1;

	double *a = new double[n];
	for(int i = 0; i < n - 1; i++){	
		a[i] = func (X + i * step, y + i * step, step);
	}
	double ret;
	if( m == 0){
		ret = KahanSum(a, n-1) / (n-1);
	}else{
		a[n - 1] = func (X + (n - 1) * step, y + (n - 1) * step, m);
		ret = KahanSum(a, n)  / n;
	}
	return ret;
}

// ===========================================================
double SxR::segmental_SxR(
	Signal X,
	Signal y,
	Signal z,
	double (*func)(Signal, Signal, Signal, const int),
	const int length,
	const int step )
{
	const int m = length % step;
	const int n = length / step + 1;

	double *a = new double[n];
	for(int i = 0; i < n - 1; i++){	
		a[i] = func (X + i * step, y + i * step, z + i * step, step);
	}
	double ret;
	if( m == 0){
		ret = KahanSum(a, n-1) / (n-1);
	}else{
		a[n - 1] = func (X + (n - 1) * step, y + (n - 1) * step,  z + (n - 1) * step, m);
		ret = KahanSum(a, n)  / n;
	}
	return ret;
}

// ===========================================================
double SxR::KahanSum(const double* a, const int length){
	double sum = a[0];
	double c = 0.;
	for(int i = 1; i < length; i++){
		double y = a[i] - c;
		double t = sum + y;
		c = (t - sum) - y;
		sum = t;
	}
	return sum;
}

// ===========================================================
// Itakura Saito distance
// ===========================================================
double specDist::IS(Spectrum x, Spectrum y, const int nyquist){
	double IS = 0.;
	for(int i = 0; i < nyquist; i++){
		assert(x[i] > 0 && y[i] > 0);
		IS += (x[i] / y[i] - log( x[i] / y[i] ) - 1);
	}
	IS /= nyquist;
	return IS;
}

// ===========================================================
// Log Spectral Distance
// ===========================================================
double specDist::LSD(Spectrum x, Spectrum y, const int nyquist){
	double LSD = 0.;
	for(int i = 0; i < nyquist; i++){
		assert(x[i] > 0 && y[i] > 0);
		double tmp = 10. * log( x[i] / y[i] ) / log(10.);
		LSD += tmp * tmp;
	}
	LSD /= nyquist;
	return LSD;
}


// ===========================================================
// Generalized Kullback Leibler Divergence
// ===========================================================
double specDist::GKL(Spectrum x, Spectrum y, const int nyquist){
	double GKL = 0.;
	for(int i = 0; i < nyquist; i++){
		assert(x[i] > 0 && y[i] > 0);
		GKL += x[i] * log (x[i] / y[i]) - x[i] + y[i];
	}
	GKL /= nyquist;
	return GKL;
}


