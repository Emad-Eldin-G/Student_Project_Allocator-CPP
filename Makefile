# the C++ compiler
CXX     = g++

# options to pass to the compiler
CXXFLAGS = -O0 -g3 -std=c++17

all: GenAlloc

GenAlloc: main.cpp Alloc.o
	$(CXX) $(CXXFLAGS) main.cpp Alloc.o -o GenAlloc

Alloc.o: Alloc.cpp Alloc.h
	$(CXX) $(CXXFLAGS) -c Alloc.cpp -o Alloc.o

clean:
	rm -f *~ *.o *.stackdump GenAlloc
