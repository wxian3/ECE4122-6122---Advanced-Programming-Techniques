// RSA Assignment for ECE4122/6122 Fall 2015

#include <iostream>
#include <fcntl.h>
#include <stdlib.h>
#include <unistd.h>

#include "RSA_Algorithm.h"

using namespace std;

// Implement the RSA_Algorithm methods here

// Constructor
RSA_Algorithm::RSA_Algorithm()
  : rng(gmp_randinit_default)
{
  // get a random seed for the random number generator
  int dr = open("/dev/random", O_RDONLY);
  if (dr < 0)
    {
      cout << "Can't open /dev/random, exiting" << endl;
      exit(0);
    }
  unsigned long drValue;
  read(dr, (char*)&drValue, sizeof(drValue));
  //cout << "drValue " << drValue << endl;
  rng.seed(drValue);
// No need to init n, d, or e.
}

// the RSA_Algorithm methods:

// Generate the public and private keys
void RSA_Algorithm:: GenerateRandomKeyPair(size_t sz)
{
	bool prime = false;
	bool divisor = false;
	mpz_class PNUM, result, q, p, phiN;
	mpz_class one = mpz_class(1);

	// Compute p & q
	/*
	for(int i = 0; i < 2; ++i)
	{
		while(!prime)
		{
			PNUM = rng.get_z_bits(sz);
			int isPrime = mpz_probab_prime_p(PNUM.get_mpz_t(), 100);
			if(isPrime == 2) break;
		}
		if(i == 0) p = PNUM;
		if(i == 1) q = PNUM;
	}
	*/
	
	
	PNUM = rng.get_z_bits(sz);
	mpz_nextprime(p.get_mpz_t(), PNUM.get_mpz_t());
	PNUM = rng.get_z_bits(sz);
	mpz_nextprime(q.get_mpz_t(), PNUM.get_mpz_t());
	
	n = p * q;
	phiN = (p - 1) * (q - 1);

	while(!divisor)
	{
		d = rng.get_z_bits(sz * 2);
		mpz_gcd(result.get_mpz_t(), d.get_mpz_t(), phiN.get_mpz_t());
		if((d < phiN) && (result == one)) break;
	}

	// Compute e:
	mpz_invert(e.get_mpz_t(), d.get_mpz_t(), phiN.get_mpz_t());

}
// Encrypt Function
mpz_class RSA_Algorithm::Encrypt(mpz_class m)
{
	//Encrypt: c = (m ^ e) mod n
  mpz_class c;
	mpz_powm(c.get_mpz_t(), 	// Resulting Encrypted Message
				m.get_mpz_t(), 	// Message to be Encrypted
				e.get_mpz_t(), 	// Key Pair (n, e) private
				n.get_mpz_t());	// Key Pair (n, e) shared
  return c;
}

// Decrypt Function
mpz_class RSA_Algorithm::Decrypt(mpz_class c)
{
	//Decrypt: m = (c ^ d) mod n
  mpz_class result;
	mpz_powm(result.get_mpz_t(),	// Resulting Decrypted Message 
				c.get_mpz_t(),	// Encryted Message
				d.get_mpz_t(),	//  
				n.get_mpz_t());
  return result;
}


void RSA_Algorithm::PrintND()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << endl;
}

void RSA_Algorithm::PrintNE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " e " << e << endl;
}

void RSA_Algorithm::PrintNDE()
{ // Do not change this, right format for the grading script
  cout << "n " << n << " d " << d << " e " << e << endl;
}

void RSA_Algorithm::PrintM(mpz_class M)
{ // Do not change this, right format for the grading script
  cout << "M " << M << endl;
}

void RSA_Algorithm::PrintC(mpz_class C)
{ // Do not change this, right format for the grading script
  cout << "C " << C << endl;
}




