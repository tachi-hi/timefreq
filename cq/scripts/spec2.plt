
# This is another gnuplot script

set term postscript eps enhanced color
set output "test.eps"

set palette defined (0 "black", 0.6 "dark-blue", 0.64 "blue", \
0.68 "turquoise", 0.75 "green", 0.8 "yellow", \
0.82 "orange-red", 0.88 "red", 0.92 "dark-red", 1 "dark-magenta")
unset colorbox
set pm3d at s map corners2color c1

set log y
set xrange[0:10]
set yrange[165:165*16]

set xlabel "time [s]"
set ylabel "frequency [Hz]"
set ytics(110, 220, 440, 880, 1760, 3520)

splot "test.dat" usi 1:2:(log($3)) w pm3d notitle

