#include<iostream>
#include<complex>
#include"fft.hpp"
using namespace std;

int main(void){
	cout << "This is an easy test" << endl;
	FFT fft;
	fft.init(16);

	// input data 適当です
	double input[] = {3.141592,1e-3,1.2345,1192.,1e6,128.,-710.,794.,-1867.,4096.,10.24,-123.,2.23606,-1.,0.,2012.};

	for(int i = 0; i < 16; i++){
		cout << input[i] << " ";
	}
	cout << endl;

	// apply fft
	complex<double> output[16];
	double inv[16];

	fft.forward(input, output);
	fft.inverse(output, inv);

	// evaluate error
	double err = 0.;
	for(int i = 0; i < 16; i++){
		cout << inv[i] << " ";
		err += abs(input[i] - inv[i]);
	}
	cout << endl;
	
	cout << "Difference: " << err << " .........." << (err < 1e-5 ? "OK" : "Error" ) <<endl;

	return 0;
}

