//
// (c) 2008, 2009, 2012 Hideyuki Tachibana (tachi-hi @ GitHub)
//

#pragma once

#include<vector>

namespace wav{
	struct Header{
		char riff[4];
		unsigned int filesize;
		char wavefmt[8];
		unsigned int waveformat;
		unsigned short int pcm;
		unsigned short int n_channel;
		unsigned int sampling_rate;
		unsigned int bytes_per_second;
		unsigned short int block_per_sample;
		unsigned short int bits_per_sample;
		char data[4];
		unsigned int n_byte;
		int total_samples(void){
			return 8 * (filesize-36) / (n_channel * bits_per_sample);
		};
	};

	template<typename T> double to_double(T){ return "Error: You mustn't use this function"; }
	template<> double to_double(unsigned char x){ return static_cast<double>( x + 128 ) / 256.; }
	template<> double to_double(signed short  x){ return static_cast<double>( x )/32767.; }

	template<typename T> T quantize(double){ return (T*)"Error: You mustn't use this function"; }
	template<> unsigned char quantize<unsigned char> (double x){ return static_cast<unsigned char> ( x * 256  - 128); }
	template<> signed short  quantize<signed short>  (double x){ return static_cast<signed short>  ( x * 32767 ); }

	class MonoChannel;
	class Signal;
};

// 
class wav::MonoChannel{
 public:
  MonoChannel(){};
  ~MonoChannel(){}
	void init (int n)
	{
		buffer_size = n;
		data.resize( n );
		current_point = 0;
	}

	void insert(double x)
	{
		data[current_point] = x;
		current_point = current_point < buffer_size - 1 
											? current_point + 1
											:  0 ;
	};

	double& operator[](int i){
		return data[ i + current_point < buffer_size 
									? i + current_point 
									: i + current_point - buffer_size];
	}

 private:
		std::vector<double> data;
		int current_point;
		int buffer_size;
};

// 
class wav::Signal{
 public:
	Signal(){}
	~Signal(){}

	void init(int n_channel, int buf_size)
	{
		buffer_size = buf_size;
		data.resize( n_channel );
		for(int i = 0; i < n_channel; i++)
		{
			data[i].init( buf_size );
		}
	}

	int length(){ return buffer_size; }
	MonoChannel& operator[](int n){ return data[n]; }

 private:
	std::vector<MonoChannel> data;
	int buffer_size;
};

