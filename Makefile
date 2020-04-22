CXX = g++ -std=c++17
CXXFLAGS = -g # -DNO_DEFAULT_MAPPING -DBLOCKING_MODE -DFF_BOUNDED_BUFFER
LDFLAGS = -fopenmp -O3
TARGETS = gol_seq gol_par gol_omp
OUTS = output_seq.txt output_par.txt output_omp.txt

.PHONY: all clean cleanall
.SUFFIXES: .cpp

%: %.cpp
	$(CXX) $(CXXFLAGS) -o $@ $< $(LDFLAGS)

all : $(TARGETS)
clean :
	rm -f $(TARGETS)
cleanall : clean
	\rm -f *.o *~ *.txt
