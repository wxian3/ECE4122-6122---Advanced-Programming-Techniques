CXXFLAGS = -c -g
LDFLAGS  = -lgmpxx -lgmp
#CXXFLAGS = -c -g -I$(HOME)/Software/gmp-5.0.2
#LDFLAGS  = -L$(HOME)/Software/gmp-4.2.2/.libs -lgmp++
CXX = g++

RSA:	RSA.o RSA_Algorithm.o
	$(CXX) -o RSA RSA.o RSA_Algorithm.o $(LDFLAGS)

RSA.o:	RSA.cc
	$(CXX) $(CXXFLAGS) RSA.cc

BreakRSA:	BreakRSA.o RSA_Algorithm.o
	$(CXX) -o BreakRSA BreakRSA.o RSA_Algorithm.o $(LDFLAGS)

BreakRSA.o:	BreakRSA.cc
	$(CXX) $(CXXFLAGS) BreakRSA.cc

# RSA_Algorithm:		RSA_Algorithm.o RSA.o
# 	$(CXX) -o RSA_Algorithm RSA_Algorithm.o RSA.o $(LDFLAGS)

RSA_Algorithm.o:	RSA_Algorithm.cc
	$(CXX) $(CXXFLAGS) RSA_Algorithm.cc

clean:
	rm -f *.o SA_Algorithm RSA


