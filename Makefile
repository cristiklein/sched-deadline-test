CXXFLAGS=-std=c++0x -g -O3
LDLIBS=-lboost_program_options

all: processperfstat setcpufrac

processperfstat: processperfstat.o

processperfstat.o: processperfstat.cc doMath.hh

setcpufrac: setcpufrac.o dl_syscalls.o

setcpufrac.o: setcpufrac.cc dl_syscalls.h

clean:
	rm -rf *.o setcpufrac processperfstat

