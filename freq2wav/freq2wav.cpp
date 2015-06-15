#include<cstdio>
#include<string>
#include<iostream>
#include<vector>
//getopt
#include<unistd.h>
#include<getopt.h>
#include<cmath>

#include"../wavio/wavio.hpp"

using namespace std;

int main(int argc, char **argv){
  /*********************************************************************************************************/
  std::string output_file_name("_sinusoidal.wav");
  int sampling_rate = 16000;
  /*********************************************************************************************************/
  while (1) {
    int this_option_optind = optind ? optind : 1;
    int option_index = 0;
    static struct option long_options[] = {
      {"help",    0,      0,   'h'},      {"srate",   1,      0,   's'},
      {"o",       1,      0,   'o'},
      {0,         0,      0,    0} 
    };
    int c = getopt_long(argc, argv, "hs:o:",
                        long_options, &option_index);
    if (c == -1) break;
    
    switch (c) {
    case'h':{
      cerr << "=========================" << endl;
      cerr << "freq2wav" << endl;
      cerr << "=========================" << endl;
      cerr << "Options:" << endl;
      cerr << "  --help\t help" << endl;
      cerr << "  --srate\t sampling rate" << endl;
      cerr << "  --o\t output file name " << endl;
      cerr << "(c) Hideyuki Tachibana 2009." << endl;
      cerr << "https://github.com/tachi-hi" << endl;
      cerr << "=========================" << endl;
      cerr << " usage: cat yourfile.dat | freq2wav" << endl;
      cerr << "=========================" << endl;
      cerr << " format of the input file: \"time[s] frequency[Hz]\" " << endl;
      cerr << " Example:  " << endl;
      cerr << "   1.1 532.1" << endl;
      cerr << "   2.7 544.3" << endl;
      cerr << "   5.2 256.4" << endl;
      exit(1);
    }
    case's':{sampling_rate = atoi(optarg); break;}
    case'o':{output_file_name = optarg;break;}
    default:
      printf("Error: getopt returned character code 0%o \n", c);
    }
  }
  if (optind < argc) {
    cerr << "non-option ARGV-elements: " << endl;
    while (optind < argc)
      fprintf(stderr, "%s ", argv[optind++]);
    fprintf(stderr, "\n");
  }
  /*********************************************************************************************************/

  // read all the data from stdin
  char line[1024];
  vector<double> time;
  vector<double> freq;
  while(fgets(line, 1024, stdin)){
    double time_tmp, freq_tmp;
    sscanf(line, "%lf %lf", &time_tmp, &freq_tmp);
    time.push_back(time_tmp);
    freq.push_back(freq_tmp);
  }

  // synthesis
  long T_length = (long)(sampling_rate * time[time.size() - 2]);
  wavo *output = new wavo(output_file_name.c_str(), 1, T_length);
  double *signal = new double[T_length];
  double th = 0;
  double th2 = 0;
  double th3 = 0;
  double th4 = 0;
  int frame_index = 0;
  for(int t = 0; t < T_length; t++){
    double d = freq[frame_index] * 2 * M_PI / sampling_rate;
    th  += d;
    th2 += 2 * d;
    th3 += 3 * d;
    th4 += 4 * d;
    signal[t] = sin(th) + sin(th2) + sin(th3) + sin(th4);
    signal[t] /= 5;
    if(t > sampling_rate * time[frame_index]){
      frame_index ++;
    }
    // avoid overflow
    const double threshold = 2 * M_PI * 1e5; 
    if(th  > threshold) th  -= threshold;
    if(th2 > threshold) th2 -= threshold;
    if(th3 > threshold) th3 -= threshold;
    if(th4 > threshold) th4 -= threshold;
  }
  output->set(signal, 0, T_length);
  output->header.sampling_rate = sampling_rate;
  output->header.n_channel = 1;
  output->header.bits_per_sample = 16;
  output->write(T_length);

  delete []signal;
  delete output;
  return 0;
}

