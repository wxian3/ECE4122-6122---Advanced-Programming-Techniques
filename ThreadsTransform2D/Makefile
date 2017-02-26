# Makefile for ECE8893, 2D FFT assignment

CXX      = /usr/bin/g++
CXXFLAGS = -Wall -g

threadDFT2d:	threadDFT2d.o Complex.o InputImage.o
	$(CXX) -g -o threadDFT2d threadDFT2d.o Complex.o InputImage.o -lpthread

clean:
	@rm *.o threadDFT2d

