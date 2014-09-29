/* =============================================================================
  Constant Q Transform sample
(c) 2009, 2012 Hideyuki Tachibana, tachi-hi @ GitHub
================================================================================*/

#include<iostream>
#include<cstdio>
#include<string>
#include<vector>

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
	bool chroma_flag = false;
	enum{
		FORMAT_gnuplot,
		FORMAT_matrix,
		FORMAT_csv,
	} format_flag = FORMAT_gnuplot;

	/*********************************************************************************************************/
	while (1) {
//		int this_option_optind = optind ? optind : 1;
		int option_index = 0;
		static struct option long_options[] = {
			{"help",    no_argument,            0,   'h'},
			{"input",   required_argument,      0,   'i'},
			{"srate",   required_argument,      0,   's'},
			{"fshift",  required_argument,      0,   'f'},
			{"startfreq",required_argument,     0,   'q'},
			{"nnyq",    required_argument,      0,   'n'},
			{"resol",   required_argument,      0,   'r'},
			{"sigma",   required_argument,      0,   'g'},
			{"Q",       required_argument,      0,   'Q'},
			{"Format",  required_argument,      0,   'F'},
			{"chroma",  no_argument,            0,   'c'},
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
			cerr << "  --Format\t output format; gnuplot (default), matrix, csv." << endl;
			cerr << "  --chroma\t change output format to chroma vector" << endl;
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
		case'F':{
			string tmp (optarg);
			if (tmp == "matrix"){
				format_flag = FORMAT_matrix;
			}else if (tmp == "gnuplot"){
				format_flag = FORMAT_gnuplot;
			}else if (tmp == "csv"){
				format_flag = FORMAT_csv;
			}else{
				format_flag = FORMAT_gnuplot;							
			}
			break;
		}
        case'c':{chroma_flag = true; break;}
		default:
			printf("Error: getopt returned character code %c (0%o) \n", c, c);
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
    std::vector<double> spec(n_nyq);
    std::vector<double> chroma(resol * 12);
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

        // display the progress 
		if (count % 100 == 0) 
			fprintf(stderr, "%f[s]\n", (double)count * frame_shift / sampling_rate);

		// transform
		cq->exec(in, &(spec[0]));

        // chroma
        if(chroma_flag){
            for (int i = 0; i < resol * 12; ++i){
                chroma[i] = 0;
            }
            for (int i = 0; i < n_nyq; ++i){
                chroma[ i % (resol * 12) ] += spec[i];
            }
        }

        // alias
        const std::vector<double> &output = chroma_flag ? chroma : spec;
        
		// save
		for(unsigned int i = 0; i < output.size(); ++i){		
		    if(format_flag == FORMAT_gnuplot)
		    {
				    const double tmp1 = (double)count * frame_shift / sampling_rate;
				    const double tmp2 = start_freq * pow(2., static_cast<double>(i)/12./resol);
				    fprintf(file, "%f %f %e\n", tmp1, tmp2, output[i]);
		    }
		    else if (format_flag == FORMAT_matrix)
		    {
				    fprintf(file, "%f%s", output[i], i < output.size() - 1 ? " " : "");
		    }
		    else if (format_flag == FORMAT_csv)
		    {
				    fprintf(file, "%f%s", output[i], i < output.size() - 1 ? "," : "");
		    }
        }
		fprintf(file, "\n");
	}
//	fclose(file);

	delete[] in;
	delete input;
	delete cq;
	return 0;
}

