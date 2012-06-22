//
// (c) 2008, 2009, 2012 Hideyuki Tachibana (tachi-hi @ GitHub)
//

#pragma once
#include <iostream>
#include <cstdlib>
#include "wav.hpp"

class wavo{
public:
	wavo(const char*, int, int);
	~wavo();
	void set(double*, int, int);
	void write(int n)
	{
		if( header.bits_per_sample == 8 )
			write_<unsigned char>(n);
		else
			write_<signed short>(n);
	}
	void write_header(void);
	wav::Header header;
  
private:
	wav::Signal signal;
	FILE *fp;
	template<typename T> void write_(int);
	int signal_length;
	int buffer_size;
};

wavo::wavo(const char *fn, int ch, int buffer_size){
  fp = fopen(fn, "wb");
  if( !fp ){
    std::cerr << "cannot open " << fn << "!" <<std::endl;
    exit(1);
  }
  signal_length = 0;
  char tmp[sizeof(wav::Header)];
  fwrite(tmp, 1, sizeof(wav::Header), fp);
  signal.init(ch, buffer_size);
  header.n_channel = ch;
  this->buffer_size = buffer_size;
}
wavo::~wavo(){
  write_header();
  fclose(fp);
}

void wavo::set(double *x, int ch, int n){
  for(int t = 0; t < n; t++){
    if(x[t] > 1.0){
      signal[ch].insert(1.0);
    }else if(x[t] < -1.0){
      signal[ch].insert(-1.0);
    }else{
      signal[ch].insert(x[t]);
    }
  }
}

template<typename T> void // unsigned char or signed short
wavo::write_(int n){
  for(int j = buffer_size - n; j < buffer_size; j++){
    for(int ch = 0; ch < header.n_channel; ch++){
      T tmp = wav::quantize<T>(signal[ch][j]);
      fwrite(&tmp, sizeof(T), 1, fp);
    }
  }
  signal_length += n;
}

void wavo::write_header(void){
  if(header.sampling_rate == 0){
    std::cerr << "sampling_rate is empty!" << std::endl;
    exit(1);
  }
  if(header.n_channel == 0){
    std::cerr << "n_channel is empty!" << std::endl;
    exit(1);
  }
  if(header.bits_per_sample == 0){
    std::cerr << "bit_rate is empty!" << std::endl;
    exit(1);
  }

  header.riff[0] = 'R';

  header.riff[1] = 'I';
  header.riff[2] = 'F';
  header.riff[3] = 'F';
  header.filesize = (header.bits_per_sample / 8) * signal_length * header.n_channel + 36;
  header.wavefmt[0] = 'W';
  header.wavefmt[1] = 'A';
  header.wavefmt[2] = 'V';
  header.wavefmt[3] = 'E';
  header.wavefmt[4] = 'f';
  header.wavefmt[5] = 'm';
  header.wavefmt[6] = 't';
  header.wavefmt[7] = ' ';
  header.waveformat = header.bits_per_sample;
  header.pcm = 1;
  header.n_channel = header.n_channel;
  header.sampling_rate = header.sampling_rate;
  header.bytes_per_second = (header.bits_per_sample / 8) * header.n_channel * header.sampling_rate;
  header.block_per_sample = (header.bits_per_sample / 8) * header.n_channel;
  header.bits_per_sample = header.bits_per_sample;
  header.data[0] = 'd';
  header.data[1] = 'a';
  header.data[2] = 't';
  header.data[3] = 'a';
  header.n_byte = (header.bits_per_sample / 8) * signal_length * header.n_channel;
  fseek(fp, 0, SEEK_SET);
  fwrite(&header, sizeof(header), 1, fp);

}

