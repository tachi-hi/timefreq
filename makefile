
# (c) 2012 Hideyuki Tachibana (tachi-hi @ GitHub)


CXX = g++
CXXFLAGS = -O3 -Wall  -pg -g
#-fopenmp -std=c++0x
LDLIBS = -lm -L. -ltimefreq
#LDLIBS = -lm

# SAMPLESRC = $(shell ls ./*/*_sample.cpp)
# SAMPLEOBJS = $(shell ls ./*/*_sample.cpp | sed "s/\.cpp/\.o/g")
# SAMPLEEXE = $(shell ls ./*/*_sample.cpp | sed "s/\.cpp//g")
SAMPLEobjs = 2Darray/2Darray_sample.O cq/cq_sample.O
SAMPLEEXE = 2Darray/2Darray_sample cq/cq_sample

TESTSRC = $(shell ls ./*/*_test.cpp)
TESTOBJS = $(shell ls ./*/*_test.cpp | sed "s/\.cpp/\.o/g")
TESTEXE = $(shell ls ./*/*_test.cpp | sed "s/\.cpp//g")

OBJS =  ./fft/fft.o ./cq/cq.o ./snr/snr.o
LIB = libtimefreq.a

RM = rm -f

all: $(OBJS) $(LIB) $(SAMPLEEXE) $(TESTEXE)
	make -C STFT

$(TESTEXE): $(TESTOBJS)
$(SAMPLEEXE): $(SAMPLEOBJS)

%: %.o $(LIB)
	$(CXX) -o $@ $< $(LDLIBS)

$.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -o $@ -c $<

$(LIB): $(OBJS)
	ar rcsv $@ $(OBJS)

# clean #
.phony: clean
clean:
	$(RM) ./*/*.o $(SAMPLEOBJS) $(TESTOBJS) *.a
	make clean -C STFT

