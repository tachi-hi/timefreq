/* =============================================================================
  Constant Q Transform sample
(c) 2009, 2012 Hideyuki Tachibana, tachi-hi @ GitHub
================================================================================*/

#include<iostream>
#include<cstdio>
#include<string>

//getopt
#include<getopt.h>

#include "cq.hpp"
#include "./../wavio/wavi.hpp"

using namespace std;

int main(int argc, char **argv){
	/*********************************************************************************************************/
	// Command Line Analysis
	std::string input_file_name("test.wav");
	unsigned int sampling_rate = 16000;
	unsigned int frame_shift = 512;
	double start_freq = 65.406; // 2 octave below middle C
	short n_nyq = 72; //6 octaves
	short resol = 5;
	double Q = 45;
	/*********************************************************************************************************/
	while (1) {
//		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"help",    0,      0,   'h'},
			{"input",   1,      0,   'i'},
			{"srate",   1,      0,   's'},
			{"fshift",  1,      0,   'f'},
			{"startfreq",1,     0,   'q'},
			{"nnyq",    1,      0,   'n'},
			{"resol",   1,      0,   'r'},
			{"sigma",   1,      0,   'g'},
			{"Q",       1,      0,   'Q'},
			{"a",       1,      0,   'a'},
			{0,         0,      0,    0}
		};
		int c = getopt_long(argc, argv, "hi:P:s:f:q:n:r:g:Q:", long_options, &option_index);
		if (c == -1) break;

		switch (c) {
		case'h':{
			cerr << "Constant Q Gabor Transform Sample" << endl;
			cerr << "Options:" << endl;
			cerr << "  --help\t help" << endl;
			cerr << "  --input\t input audio file" << endl;
			cerr << "  --srate\t sampling rate" << endl;
			cerr << "  --fshift\t frame shift" << endl;
			cerr << "  --startfreq\t lowest frequency of the spectrogram" << endl;
			cerr << "  --nnyq\t number of frequency bands of the spectrogram" << endl;
			cerr << "  --resol\t frequency resolugion" << endl;
			cerr << "  --Q\t Q" << endl;
			cerr << "(c) 2012 Hideyuki Tachibana, tachi-hi @ github" << endl;
			exit(1);
		}
		case'i':{input_file_name = optarg;break;}
		case's':{sampling_rate = atoi(optarg); break;}
		case'f':{frame_shift = atoi(optarg); break;}
		case'q':{start_freq = atof(optarg); break;}
		case'n':{n_nyq = atoi(optarg); break;}
		case'r':{resol = atoi(optarg); break;}
		case'Q':{Q = atof(optarg); break;}
		default:
			printf("Error: getopt returned character code 0%o \n", c);
		}
	}
	n_nyq = n_nyq * resol;
	if (optind < argc) {
		cerr << "non-option ARGV-elements: " << endl;
		while (optind < argc)
			fprintf(stderr, "%s ", argv[optind++]);
		fprintf(stderr, "\n");
	}
	/*********************************************************************************************************/

	// Initialize Chroma	
	CQ *cq = new CQ();
	cq->set_sampling_rate(sampling_rate);
	cq->set_LF(start_freq);
	cq->set_ch(n_nyq);
	cq->set_freq_resolution(resol);
	cq->set_Q(Q);
	cq->filter_init();
	int frame_length = cq->get_frame_length();

	// Input file
	wavi *input = new wavi (input_file_name.c_str(), frame_length);
	if( sampling_rate != input->header.sampling_rate ){
		printf("error, invalid sampling rate\n");
		exit(1);
	}

	input->read(frame_length / 2 - frame_shift);
	double *in = new double [frame_length];
	double *spec = new double [n_nyq];

	FILE *file = stdout;

	int last_count = 0;
	for (int count = 0; count < last_count + frame_length / 2 / (int)frame_shift + 1; ++count)
	{
		if(!input->eof())
			last_count++;
		else
			;

		//read from file
		input->read(frame_shift);
		input->copy(in, 0, frame_length);

		// transform
		cq->exec(in, spec);

		if (count % 10 == 0) 
			fprintf(stderr, "%f[s]\n", (double)count * frame_shift / sampling_rate);

		// save
		for(int i = 0; i < n_nyq; ++i)
		{
			fprintf(file, "%f %f %e\n", (double)count * frame_shift / sampling_rate, start_freq * pow(2., static_cast<double>(i)/12./resol), spec[i]);
		}
		fprintf(file, "\n");
	}

	fclose( file );

	delete input;
	delete cq;
	delete[] in;
	delete[] spec;
	return 0;
}

