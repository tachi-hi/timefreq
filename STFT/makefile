PythonLIBS = `pkg-config --cflags --libs python2`
PyUblas = -I/usr/local/lib/python2.7/dist-packages/pyublas/include



all: subdir python.o
	g++ --std=c++0x -Wall -o stft \
		-funroll-loops \
		${PythonLIBS} \
		${PyUblas} \
		-DPIC -shared -fPIC \
		python.o stft_src/STFT.o \
		-lboost_python

subdir:
	make --dir=stft_src

python.o: python.cpp
	g++ --std=c++0x -Wall -c -O3 ${PythonLIBS} ${PyUblas} -fPIC $< -lboost_python

.phony: clean
clean:
	rm -f *.o
	make --dir=stft_src clean
