
# (c) 2012 Hideyuki Tachibana (tachi-hi @ GitHub)
# (c) 2016 Hideyuki Tachibana (tachi-hi @ GitHub)


CXX = g++
CXXFLAGS = -O3 -Wall  -pg -g
#-fopenmp -std=c++0x
LDLIBS = -lm -L. -lfftw3

SAMPLESRC = $(shell ls ./*_sample.cpp)
SAMPLEOBJS = $(shell ls ./*_sample.cpp | sed "s/\.cpp/\.o/g")
SAMPLEEXE = $(shell ls ./*_sample.cpp | sed "s/\.cpp//g")

RM = rm -f

all: $(SAMPLEEXE)

$(SAMPLEEXE): $(SAMPLEOBJS)

%: %.o $(LIB)
	$(CXX) -o $@ $< $(LDLIBS)

$.o: %.cpp
	$(CXX) $(CXXFLAGS) $(DEBUG) -o $@ -c $<

# clean #
.phony: clean
clean:
	$(RM) $(SAMPLEOBJS) $(SAMPLEEXE) 
