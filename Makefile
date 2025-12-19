# the C++ compiler
CXX     = g++

# options to pass to the compiler
CXXFLAGS = -O0 -g3 -std=c++17

All: all
all: GenAlloc CheckAlloc

GenAlloc: GenAlloc.cpp Alloc.o
	$(CXX) $(CXXFLAGS) GenAlloc.cpp Alloc.o -o GenAlloc

CheckAlloc: CheckAlloc.cpp Alloc.o
	$(CXX) $(CXXFLAGS) CheckAlloc.cpp Alloc.o -o CheckAlloc

Alloc.o: Alloc.cpp Alloc.h
	$(CXX) $(CXXFLAGS) -c Alloc.cpp -o Alloc.o

clean:
	rm -f *~ *.o *.stackdump GenAlloc CheckAlloc
