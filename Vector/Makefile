# Makefile for ECE4893/8893, Lab 3
# Fall 2010
# George F. Riley

CXXFLAGS = -g
testvec:	testVec.o String.o
	$(CXX) -g -o testVec testVec.o String.o

String.o:	String.cc

testVec.o:	testVec.cc Vector.h Vector.cc

clean:
	@rm -r *.o testVec

