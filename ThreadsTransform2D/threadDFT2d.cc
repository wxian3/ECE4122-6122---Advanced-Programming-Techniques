// Threaded two-dimensional Discrete FFT transform
// Wenqi Xian
// ECE8893 Project 2


#include <iostream>
#include <string>
#include <math.h>

#include "Complex.h"
#include "InputImage.h"

// You will likely need global variables indicating how
// many threads there are, and a Complex* that points to the
// 2d image being transformed.

using namespace std;

// Global variable visible to all threads
Complex* ImageData;
int ImageWidth;
int ImageHeight;
Complex* W;
int N;

// Each thread needs to know how many threads there are.
int nThreads = 16;

pthread_mutex_t startCountMutex;
pthread_mutex_t transposeMutex;
pthread_mutex_t exitMutex;
pthread_cond_t exitCond;

// pthread barrier
pthread_barrier_t barrier;

int startCount;
bool tranposed = true;

void Transpose(Complex* input, int row, int col)
{
    Complex* result = new Complex[row * col];
    for (int i = 0; i < row; i++){
       for (int j = 0; j < col; j++) {
           result[j * row + i] = input[i * col + j];
       }
    }
    
    for (int m = 0; m < row * col; m++){
      input[m] = result[m];
    } 
}

// Function to reverse bits in an unsigned integer
// This assumes there is a global variable N that is the
// number of points in the 1D transform.
unsigned ReverseBits(unsigned v)
{ //  Provided to students
  unsigned n = N; // Size of array (which is even 2 power k value)
  unsigned r = 0; // Return value
   
  for (--n; n > 0; n >>= 1)
    {
      r <<= 1;        // Shift return value
      r |= (v & 0x1); // Merge in next bit
      v >>= 1;        // Shift reversal value
    }
  return r;
}

// GRAD Students implement the following 2 functions.
// Undergrads can use the built-in barriers in pthreads.

// Call MyBarrier_Init once in main
void MyBarrier_Init()// you will likely need some parameters)
{
}

// Each thread calls MyBarrier after completing the row-wise DFT
void MyBarrier() // Again likely need parameters
{
}
                    
void Transform1D(Complex* h, int N)
{
  // Implement the efficient Danielson-Lanczos DFT here.
  // "h" is an input/output parameter
  // "N" is the size of the array (assume even power of 2)
  int status[N];
  int revBits;
  Complex temp;
  for (int i = 0; i < N; ++i)
  {
    if (status[i] != 1) {
      revBits = ReverseBits(i);
      temp = h[i];
      h[i] = h[revBits];
      h[revBits] = temp;
      status[revBits] = 1;
    }
  }

  for (int i = 2; i < N + 1; i = i * 2)
  {
    Complex* H = new Complex[i];
    for (int j = 0; j < N/i; ++j)
    {
      for (int k = 0; k < i; ++k)
      {
        int even;
        if (k < i/2) {
          even = i * j + k;
        } else {
          even = i * j + k - i/2;
        }
        H[k] = h[even] + h[even + i/2] * W[N / i * k];
      }
      for (int k = 0; k < i; ++k)
      {
        h[k + i * j] = H[k];
      }
    }
    
  }
}

void* Transform2DTHread(void* v)
{ // This is the thread startign point.  "v" is the thread number
  // Calculate 1d DFT for assigned rows
  // wait for all to complete
  // Calculate 1d DFT for assigned columns
  // Decrement active count and signal main if all complete


  // This thread is done; decrement the active count and see if all
  // have finished

  unsigned long myId = (unsigned long)v;
  int rowsPerThread = ImageHeight / nThreads;
  int startingRow = myId * rowsPerThread;
  for (int i = 0; i < rowsPerThread; i++) {
    Transform1D(ImageData + (startingRow + i) * ImageWidth, ImageWidth);
  }

  pthread_barrier_wait(&barrier);

  pthread_mutex_lock(&transposeMutex);
  if (tranposed) {
    Transpose(ImageData, ImageWidth, ImageHeight);
    tranposed = false;
  }
  pthread_mutex_unlock(&transposeMutex);
  
  for (int i = 0; i < rowsPerThread; i++) {
    Transform1D(ImageData + (startingRow + i) * ImageWidth, ImageWidth);
  }

  pthread_mutex_lock(&startCountMutex);

  startCount--;
  if (startCount == 0)
  {  // Last to exit, notify main
    // Transpose back
    Transpose(ImageData, ImageWidth, ImageHeight);
    pthread_mutex_lock(&exitMutex);
    pthread_cond_signal(&exitCond);
    pthread_mutex_unlock(&exitMutex);    
  } 
  pthread_mutex_unlock(&startCountMutex);
  
}

void Transform2D(const char* inputFN) 
{ // Do the 2D transform here.
  InputImage image(inputFN);  // Create the helper object for reading the image
  // Create the global pointer to the image array data
  // Create 16 threads
  // Wait for all threads complete
  // Write the transformed data
  startCount = nThreads; // Total threads (to be) started
  // Now start the threads
  for (int i = 0; i < nThreads; ++i) {
    // Now create the thread
    pthread_t pt; // pThread variable (output param from create)
    // Third param is the thread starting function
    // Fourth param is passed to the thread starting function
    pthread_create(&pt, 0, Transform2DTHread, (void*)i);
  }
  // Main program now waits until all child threads completed
  pthread_cond_wait(&exitCond, &exitMutex);
  image.SaveImageData("MyAfter2D.txt", ImageData, ImageWidth, ImageHeight);
}

int main(int argc, char** argv)
{
  string fn("Tower.txt"); // default file name
  if (argc > 1) fn = string(argv[1]);  // if name specified on cmd line
  InputImage image(fn.c_str());
  ImageData = image.GetImageData();
  ImageWidth = image.GetWidth();
  ImageHeight = image.GetHeight();

  // All mutex and condition variables must be "initialized"

  pthread_mutex_init(&startCountMutex,0);
  pthread_mutex_init(&exitMutex,0);
  pthread_cond_init(&exitCond, 0);
  // Main holds the exit mutex until waiting for exitCond condition
  pthread_mutex_lock(&exitMutex);

  // initialize pthread barrier
  pthread_barrier_init(&barrier, NULL, nThreads);
  
  N = ImageWidth;
  W = new Complex[N];
  for (int i = 0; i < N/2; ++i)
  {
    double real = cos(i * 2 * M_PI / N);
    double img = -sin(i * 2 * M_PI / N);
    W[i]= Complex(real, img);
    W[N/2 + i] = Complex(-real, -img);
  }
  Transform2D(fn.c_str()); // Perform the transform.
}  
  

  
