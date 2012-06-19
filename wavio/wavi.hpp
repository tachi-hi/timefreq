//
// (c) 2008, 2009, 2012 Hideyuki Tachibana (tachi-hi @ GitHub)
//

#pragma once
#include <iostream>
#include <cstdlib>
#include <cstdio>
#include <string>

#include "wav.hpp"

class wavi{
 public:
	wavi(const char*, int);
	~wavi();

	void read(int n)
	{
		if(header.bits_per_sample == 8)
			read_<unsigned char>(n);
		else if (header.bits_per_sample == 16)
			read_<signed short>(n);
		else
			printf("error"),exit(1);
	}

	void copy(double* x, int ch, int n)
	{
		for(int i = 0; i < n; i++)
			x[i] = signal[ch][i];
	}

	bool eof(){ return feof(fp); }
	wav::Header header;

 private:
	wav::Signal signal;
	std::string filename;
	FILE *fp;
	int current_seek;
	template<typename T> void read_(int n);
};

wavi::wavi(const char *fn, int buffer_size){
	filename = fn;
	fp = fopen(filename.c_str(), "rb");
	if( !fp ){
		std::cerr << "cannot open " << filename << "!" <<std::endl;
		exit(1);
	}
	if ( fread(&header, sizeof(header), 1, fp) != 1 ) {std::cerr << "error" << std::endl; exit(1);}
	current_seek = 0;
	signal.init( header.n_channel, buffer_size );
}

wavi::~wavi(){
	fclose(fp);
}

template<typename T> void // T: unsigned char or signed short
wavi::read_(int n){
	T tmp;
	double tmp_double;
	fseek(fp, sizeof(wav::Header) + current_seek, SEEK_SET);
	for(int i = 0; i < n ; i++){
		for(int ch = 0; ch < header.n_channel; ch++){
			if( !this->eof() ){
				if ( fread(&tmp, sizeof(T), 1, fp) != 1 )
					signal[ch].insert( 0.0 );
				tmp_double = wav::to_double(tmp);
				signal[ch].insert( tmp_double );
			}else{
				signal[ch].insert( 0.0 );
			}
		}
	}
	current_seek += n * header.n_channel * sizeof(T);
}


