#!/usr/bin/bash

# This is another sample script
# It requires sox and imagemagick.

for i in `ls ./*.wav`
do
	samplingRate=`soxi -r $i`
	shift=`expr $samplingRate / 50`

	echo Input File = $i
	echo Sampling Rate = $samplingRate
	echo Frame Shift = $shift

	./cq_sample --input $i --srate $samplingRate --fshift $shift --startfreq 110 --Q 80 --resol 5 --nnyq 72 > test.dat
	gnuplot spec2.plt

	eps2eps test.eps test2.eps
	convert -density 144 test2.eps -background white -alpha off test.png
	convert test.png test.eps
	if [ ${i%.wav}.eps != test.eps ]
	then
		mv test.eps ${i%.wav}.eps
		mv test.png ${i%.wav}.png
	fi
done

