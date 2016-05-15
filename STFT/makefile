#PythonLIBS = `pkg-config --cflags --libs python2`
#PyUblas = -I/usr/local/lib/python2.7/dist-packages/pyublas/include

PythonLIBS = `pkg-config --cflags --libs python2`
#PyUblas = -I~/.local/lib/python2.7/site-packages/pyublas/include
PyUblas = -I/usr/local/lib/python2.7/dist-packages/pyublas/include

CXXFLAG=--std=c++0x -O3 -Wall# -g

all: STFT.so

STFT.so: subdir python.o
	g++ $(CXXFLAG) -o $@ \
		-funroll-loops \
		${PythonLIBS} \
		${PyUblas} \
		-DPIC -shared -fPIC \
		python.o stft_src/STFT.o \
		-lboost_python -lfftw3

subdir:
	make --dir=stft_src

python.o: python.cpp
	g++ $(CXXFLAG) -c ${PythonLIBS} ${PyUblas} -fPIC $< -lboost_python

.phony: clean
clean:
	rm -f *.o
	rm -f STFT.so
	make --dir=stft_src clean
