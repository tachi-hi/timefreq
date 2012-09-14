#include<cstdio>
#include<cmath>
#include"snr.hpp"
using namespace std;
using namespace SxR;

const int N = 1000000;

int main(int argc, char **argv){

	double *b = new double[N];
	double *c = new double[N];
	double *d = new double[N];
	for(int i = 0; i < N; i++){
		b[i] = sin((double)i * 2 * M_PI * 400 / 16000);
		c[i] = b[i] + sin((double)i * 2 * M_PI * 482 / 16000);
		d[i] = b[i] + 0.5 * sin((double)i * 2 * M_PI * 917 / 16000);
	}

	printf("---------------------------------------------\n");
	printf("ideal: Inf \t%12.8f [dB]\n", SDR(b,b,N));
	printf("ideal: 0.00\t%12.8f [dB]\n", SDR(c,b,N));
	printf("ideal: 0.00\t%12.8f [dB]\n", SDR(b,c,N));
	printf("ideal: 6.02\t%12.8f [dB]\n", SDR(d,b,N));

	printf("---------------------------------------------\n");
	printf("ideal:error\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,10));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,100));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,1000));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,10000));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,100000));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(b,c,SDR,N,1000));

	printf("---------------------------------------------\n");
	printf("ideal:error\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,10));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,100));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,1000));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,10000));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,100000));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,134));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,101));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,1002));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,SDR,N,10001));
	printf("ideal:error occurs here and halt\n");
	double a = segmental_SxR(d,b,SDR,N,1001);
	printf("---------------------------------------------\n");

	delete[] b;
	delete[] c;
	delete[] d;
}

