#include<cstdio>
#include<cmath>
#include"snr.hpp"
using namespace std;
using namespace SxR;

const int N = 1000000;

void TEST(double a, double x){
	printf("ideal: %10.5f [dB] \t actual: %10.5f [dB] \t error: %10.5f [dB] \t %s\n", a, x, abs(a - x), abs(a - x) < 1e-2 ? "OK" : "Error");
}
void TEST(const char* a, double x){
	printf("ideal: %14s \t actual: %10.5f [dB]\n", a, x);
}

int main(int argc, char **argv){

	double *b = new double[N];
	double *c = new double[N];
	double *d = new double[N];
	double *e = new double[N];
	for(int i = 0; i < N; i++){
		b[i] = sin((double)i * 2 * M_PI * 400 / 16000);
		c[i] = b[i] + sin((double)i * 2 * M_PI * 450 / 16000);
		d[i] = b[i] + 0.5 * sin((double)i * 2 * M_PI * 930 / 16000);
		e[i] = sin((double)i * 2 * M_PI * 930 / 16000);
	}

	printf("--------------------------------------------------------------------------------\n");
	printf("Test on SDR function\n");
	printf("--------------------------------------------------------------------------------\n");
	TEST("infinity",        SDR(b,b,N));
	TEST(20/log(10)*log(1), SDR(c,b,N));
	TEST(20/log(10)*log(1), SDR(b,c,N));
	TEST(20/log(10)*log(2), SDR(d,b,N));

	printf("--------------------------------------------------------------------------------\n");
	printf("Test on SIR function\n");
	printf("--------------------------------------------------------------------------------\n");
	TEST("infinity",        SIR(b,b,e,N));
	TEST("big value",       SIR(c,b,e,N));
	TEST(20/log(10)*log(2), SIR(d,b,e,N));

	printf("--------------------------------------------------------------------------------\n");
	printf("ideal:error\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,10));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,100));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,1000));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,10000));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(c,b,SDR,N,100000));
	printf("ideal: 0.00\t%12.8f [dB]\n", segmental_SxR(b,c,SDR,N,1000));

	printf("--------------------------------------------------------------------------------\n");
	printf("ideal:error\t%12.8f [dB]\n", segmental_SxR(c,b,e,SIR,N,10));
	printf("ideal: big \t%12.8f [dB]\n", segmental_SxR(c,b,e,SIR,N,1000));
	printf("ideal: 6.02\t%12.8f [dB]\n", segmental_SxR(d,b,e,SIR,N,1000));


	printf("--------------------------------------------------------------------------------\n");
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
	printf("%f", a);
	printf("--------------------------------------------------------------------------------\n");

	delete[] b;
	delete[] c;
	delete[] d;
	delete[] e;
}

