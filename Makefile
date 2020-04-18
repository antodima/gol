CXX = /usr/bin/g++
CXXFLAGS = -std=c++14
LDFLAGS = -fopenmp -O3

OBJS = gol_seq gol_par gol_omp
SRCS = gol_seq.cpp gol_par.cpp gol_omp.cpp
OUTS = output_seq.txt output_par.txt output_omp.txt

clean:
	rm -f $(OBJS)

cleanall:
	rm -f $(OBJS) $(OUTS)
