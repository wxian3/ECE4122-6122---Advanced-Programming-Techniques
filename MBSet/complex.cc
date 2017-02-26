//
// ECE3090 Program 3 - Complex Number Class implementation
// YOUR NAME HERE
// PLATFORM (Windows or Linux)
//

#include <iostream>
#include <string>

#include <math.h>

#include "complex.h"

using namespace std;

// Constructors
Complex::Complex()
    : real(0), imag(0), NaN(false)
{
}

Complex::Complex(bool n)
    : real(0), imag(0), NaN(n)
{
}

Complex::Complex(double r)
    : real(r), imag(0), NaN(false)
{
}

Complex::Complex(double r, double i)
    : real(r), imag(i), NaN(false)
{
}

// Operators
Complex Complex::operator+(const Complex& b) const
{
  if (NaN || b.NaN) return Complex(true); // Not a number
  return Complex(real + b.real, imag + b.imag);
}

Complex Complex::operator-(const Complex& b) const
{
  if (NaN || b.NaN) return Complex(true); // Not a number
  return Complex(real - b.real, imag - b.imag);
}

Complex Complex::operator*(const Complex& b) const
{
  if (NaN || b.NaN)  return Complex(true); // NaN
  return Complex(real*b.real - imag*b.imag,
                 real*b.imag + imag*b.real);
}

Complex Complex::operator/(const Complex& b) const
{
  if (NaN || b.NaN)  return Complex(true); // NaN
  Complex tmp = (*this) * b.Conj();
  Complex magSquared = b.Mag() * b.Mag();
  if (magSquared.Mag().real == 0.0)
    {
      return Complex(true); // Nan
    }
  return Complex(tmp.real/magSquared.real, tmp.imag/magSquared.real);
}

// Member functions
Complex Complex::Mag() const
{
  if (NaN) return Complex(true); // Not a number
  return Complex(sqrt(real*real + imag*imag));
}

// Member functions
double Complex::Mag2() const
{ // Mag squared
  if (NaN) return 0.0; // Not a number
  return real*real + imag*imag;
}

Complex Complex::Angle() const
{
  if (NaN) return Complex(true); // Not a number
  if (Mag().real == 0) return Complex(true);
  return Complex(atan2(imag, real) * 360 / (2 * M_PI));
}

Complex Complex::Conj() const
{ // Return to complex conjugate
  if (NaN) return Complex(true); // Not a number
  return Complex(real, -imag);
}

void Complex::Print() const
{
  if (NaN)
    {
      cout << "NaN";
    }
  else
    {
      if (imag == 0)
        { // just real part with no parens
          cout << real;
        }
      else
        {
          cout << '(' << real << "," << imag << ')';
        }
    }
}

// Global function to output a Complex value
std::ostream& operator << (std::ostream &os, const Complex& c)
{ // Provided by the Prof. Riley
  if (c.NaN)
    {
      os << "NaN";
    }
  else
    {
      if (c.imag == 0)
        { // just real part with no parens
          os << c.real;
        }
      else
        {
          os << '(' << c.real << "," << c.imag << ')';
        }
    }
  return os;
}
