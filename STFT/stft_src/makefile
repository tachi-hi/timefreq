
# (c) 2012 Hideyuki Tachibana (tachi-hi @ GitHub)
# (c) 2016 Hideyuki Tachibana (tachi-hi @ GitHub)


CXX = g++
CXXFLAGS = -O3 -Wall# -g # -pg
#-fopenmp -std=c++0x
LDLIBS = -lm -L. -lfftw3

SAMPLESRC = $(shell ls ./*_sample.cpp)
SAMPLEOBJS = $(shell ls ./*_sample.cpp | sed "s/\.cpp/\.o/g")
SAMPLEEXE = $(shell ls ./*_sample.cpp | sed "s/\.cpp//g")

OBJS=STFT.o invSTFT.o

RM = rm -f

all: $(OBJS) $(SAMPLEEXE)

$(SAMPLEEXE): $(SAMPLEOBJS)

%: %.o $(LIB)
	$(CXX) -o $@ $< $(OBJS) $(LDLIBS)

%.o: %.cpp
	$(CXX) -std=c++0x $(CXXFLAGS) -DPIC -shared -fPIC -c $<

# clean #
.phony: clean
clean:
	$(RM) $(SAMPLEOBJS) $(SAMPLEEXE) $(OBJS)
