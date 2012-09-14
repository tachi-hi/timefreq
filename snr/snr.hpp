
#pragma once
#include<cstdlib>
#include<cassert>
#include<cmath>

namespace SxR{
	double SDR(const double* X, const double* y, const int length);
	double segmental_SxR(const double*X, const double*y, double (*func)(const double*, const double*, const int), const int length, const int step );
	double KahanSum(const double* a, const int length);
}

