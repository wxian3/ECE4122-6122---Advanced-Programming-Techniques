//
// ECE8893 - Complex Number Class decaration
//


class Complex {
public:
  //Constructors;
  Complex();
  Complex(bool);   // NaN constructor
  Complex(double r, double i); // Real and imaginary parts
  Complex(double r);           // Real part only, imag = 0.
  // Overloaded operators
  Complex operator+ (const Complex& b) const;
  Complex operator- (const Complex& b) const;
  Complex operator* (const Complex& b) const;
  Complex operator/ (const Complex& b) const;
  // Required member functions
  Complex Mag()   const; //Returns the magnitude of the complex number
  double  Mag2()  const; //Returns the square of the magnitude
  Complex Angle() const; //returns the angle of the complex number
  Complex Conj()  const; //returns the complex conjugate of the Complex number
  void    Print() const; // Print the complex value
  // Data Members
public:
  double real;
  double imag;
  bool   NaN;   // True if "Not a number".  This is set on divide by zero
                // or ill-formatted text string on string constructor,
                // or any arithmetic operation with a NaN operand.
};








