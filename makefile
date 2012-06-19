
# (c) 2012 Hideyuki Tachibana (tachi-hi @ GitHub)


CXX = g++
CXXFLAGS = -O3 -Wall  -pg -g
#-fopenmp -std=c++0x
LDLIBS = -lm

SAMPLE = $(shell ls ./*/*_sample.cpp)
SAMPLEOBJS = $(shell ls ./*/*_sample.cpp | sed "s/\.cpp//g")
TEST = $(shell ls ./*/*_test.cpp)
TESTOBJS = $(shell ls ./*/*_test.cpp | sed "s/\.cpp//g")

RM = rm -f

all: $(SAMPLEOBJS) $(TESTOBJS)

%.o: %.cpp 
	$(CXX) $(CXXFLAGS) $(DEBUG) -c $@ $<

%: %.o 
	$(CXX) $^ -o $@ $< $(LDLIBS) 

# clean #
.phony: clean
clean:
	$(RM) ./*/*.o $(SAMPLEOBJS) $(TESTOBJS)


