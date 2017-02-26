// Distributed two-dimensional Discrete FFT transform
// Wenqi Xian
// ECE8893 Project 1


#include <iostream>
#include <fstream>
#include <sstream>
#include <string>
#include <vector>
#include <signal.h>
#include <math.h>
#include <mpi.h>
#include <stdio.h>
#include <stdlib.h>

#include "Complex.h"
#include "InputImage.h"

using namespace std;

void Transform1D(Complex* h, int w, Complex* H)
{
  // Implement a simple 1-d DFT using the double summation equation
  // given in the assignment handout.  h is the time-domain input
  // data, w is the width (N), and H is the output array.
  for (int n = 0; n < w; n++) {
    for (int k = 0; k < w; k++) {
      H[n] = H[n] + Complex(cos(2 * M_PI * n * k / w), - sin(2 * M_PI * n * k / w)) * h[k]; 
    }
    if (fabs(H[n].real) < 1e-10) H[n].real = 0;
    if (fabs(H[n].imag) < 1e-10) H[n].imag = 0;
  }
}

void Transpose(Complex* result, Complex* input, int row, int col)
{
    for (int i = 0; i < row; i++){
       for (int j = 0; j < col; j++) {
           result[j * row + i] = input[i * col + j];
       }
    }
}

void Transform2D(const char* inputFN) 
{ // Do the 2D transform here.
  // 1) Use the InputImage object to read in the Tower.txt file and
  //    find the width/height of the input image.
  // 2) Use MPI to find how many CPUs in total, and which one
  //    this process is
  // 3) Allocate an array of Complex object of sufficient size to
  //    hold the 2d DFT results (size is width * height)
  // 4) Obtain a pointer to the Complex 1d array of input data
  // 5) Do the individual 1D transforms on the rows assigned to your CPU
  // 6) Send the resultant transformed values to the appropriate
  //    other processors for the next phase.
  // 6a) To send and receive columns, you might need a separate
  //     Complex array of the correct size.
  // 7) Receive messages from other processes to collect your columns
  // 8) When all columns received, do the 1D transforms on the columns
  // 9) Send final answers to CPU 0 (unless you are CPU 0)
  //   9a) If you are CPU 0, collect all values from other processors
  //       and print out with SaveImageData().
  InputImage image(inputFN);  // Create the helper object for reading the image
  // Step (1) in the comments is the line above. 
  // Your code here, steps 2-9
  int input_width = image.GetWidth();
  int input_height = image.GetHeight();
  Complex* result = new Complex[input_width * input_height];
  Complex* input = image.GetImageData();

  // Transform1D
  for (int i = 0; i < input_height; i++) {
    Transform1D(input + i * input_width, input_width, result + i * input_width);
  }

  image.SaveImageData("MyAfter1D.txt", result, input_width, input_height);

  // Transpose 
  Complex* transposed = new Complex[input_width * input_height];
  Transpose(transposed, result, input_height, input_width);

  Transform1D again
  Complex* transposed_result = new Complex[input_width * input_height];
  for (int i = 0; i < input_height; i++) {
    Transform1D(transposed + i * input_width, input_width, transposed_result + i * input_width);
  }
  
  // Transpose again 
  Complex* final = new Complex[input_width * input_height];
  Transpose(final, transposed_result, input_height, input_width);

  image.SaveImageData("MyAfter2D.txt", final, input_width, input_height);

}

int main(int argc, char** argv)
{
  string fn("Tower.txt"); // default file name
  if (argc > 1) fn = string(argv[1]);  // if name specified on cmd line
  Transform2D(fn.c_str()); // Perform the transform.
}  
  

  
