


#include"snr.hpp"

double SxR::SDR(const double* X, const double* y, const int length){
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


double SxR::segmental_SxR(
	const double*X,
	const double*y,
	double (*func)(const double*, const double*, const int),
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

