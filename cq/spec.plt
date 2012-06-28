#
# usage:
#   gnuplot spec.plt
# 

set term png
set output "test.png"
set palette defined (0 "grey100", 0.05 "gray70", 0.1 "gray50", 1 "gray0")
unset colorbox
set pm3d at s map corners2color c1

set log y
set xrange[0:25]
set yrange[65.4:(130.8*32)]

set ytics (130.8/2, 130.8, 130.8*2, 130.8*4, 130.8*8, 130.8*16, 130.8*32, 130.8*64, 130.8*128)
set xlabel "Time [s]"
set ylabel "Frequency [Hz]"

splot "test.dat" usi 1:2:3 w pm3d notitle

