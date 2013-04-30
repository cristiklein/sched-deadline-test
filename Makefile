CXXFLAGS=-std=c++0x -g -O3
LDLIBS=-lboost_program_options

all: processperfstat

processperfstat: processperfstat.o

