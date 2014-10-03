#!/usr/bin/bash

#
# This script converts all the .wav files 
# in the current directory into CQT scalogram.
#
# example:
#    file1.wav  file2.wav  file3.wav
# ->
#    file1.png  file2.png  file3.png
#
# Note:
# This script requires "sox" and "gnuplot"
#


for i in `ls *.wav`
do
	samplingRate=`soxi -r $i`
	shift=`expr $samplingRate / 100`

	echo Input File = $i
	echo Sampling Rate = $samplingRate
	echo Frame Shift = $shift

	./cq_sample --input $i --srate $samplingRate --fshift $shift --startfreq 110 --Q 80 --resol 10 > test.dat
	gnuplot spec.plt

	if [ ${i%.wav}.png != test.png ]
	then
		mv test.png ${i%.wav}.png
	fi
done

