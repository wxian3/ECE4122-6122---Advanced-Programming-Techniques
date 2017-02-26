// ECE4122/6122 RSA Encryption/Decryption assignment
// Fall Semester 2015

#include <iostream>
#include "RSA_Algorithm.h"

using namespace std;

int main()
{
  // Instantiate the one and only RSA_Algorithm object
  RSA_Algorithm RSA;
  
  
  // Loop from sz = 32 to 1024 inclusive
  // for each size choose 10 different key pairs
  // For each key pair choose 10 differnt plaintext 
  // messages making sure it is smaller than n.
  // If not smaller then n then choose another
  // For eacm message encrypt it using the public key (n,e).
  // After encryption, decrypt the ciphertext using the private
  // key (n,d) and verify it matches the original message.

  mpz_class c, m, result;

  // Loop from sz = 32 to 1024 inclusive
  for(int sz = 32; sz < 1025; sz *= 2)
			{
				// Generate 100 Random Keys for each size
				for(int i = 0; i < 100; ++i)
				{
					RSA.GenerateRandomKeyPair(sz);
					// Generate 100 Random Messages for each key
          RSA.PrintNDE();
					for(int j = 0; j < 100; ++j)
					{
						// Create a message from random bits:
						m = RSA.rng.get_z_bits(sz * 2);
            // messages making sure it is smaller than n.
						while(m > RSA.n) m = RSA.rng.get_z_bits(sz * 2);

						// Encrypt message 'm'
						c = RSA.Encrypt(m);

            // Decrypt it into 'result'
						result = RSA.Decrypt(c);

						/* verify the decrypted message is identical to the original plaintext message.
						if((m - result) == 0)
						{
							cout << "Passed" << endl;
						}
            */
            RSA.PrintM(m);
            RSA.PrintC(c);
          }
				}
			}
}
  
