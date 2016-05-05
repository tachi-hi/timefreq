PythonLIBS = `pkg-config --cflags --libs python2`
PyUblas = -I/usr/local/lib/python2.7/dist-packages/pyublas/include

all:
	make --dir=stft_src

py: python.cpp
	g++ --std=c++0x -Wall -c -O3 ${PythonLIBS} ${PyUblas} -fPIC $< -lboost_python
