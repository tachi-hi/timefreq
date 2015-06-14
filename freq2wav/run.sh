#!/bin/bash

# sample
cat sample_freq.dat | ./freq2wav --o sample.wav

# another sample
for i in {1..32}; do echo ${i} ${i}00; done > another_sample_freq.dat
cat another_sample_freq.dat | ./freq2wav --o another_sample.wav

