// Calculate and display the Mandelbrot set
// ECE4893/8893 final project, Fall 2011

#include <iostream>
#include <string.h>
#include <stdio.h>
#include <math.h>
#include <stdlib.h>
#include <time.h>
#include <pthread.h>

#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>
#include "complex.h"

using namespace std;
#define WINDOW_DIM 512

// Each thread needs to know how many threads there are.
int nThreads = 16;
int startCount;

// The mutex and condition variables allow the main thread to
// know when all helper threads are completed.
pthread_mutex_t startCountMutex;
pthread_mutex_t exitMutex;
pthread_cond_t  exitCond;

// Min and max complex plane values
Complex  minC(-2.0, -1.2);
Complex  maxC(1.0, 1.8);
int      maxIt = 2000;     // Max iterations for the set computations

class RGB
{
public:
  RGB()
    : r(0), g(0), b(0) {}
  RGB(double r0, double g0, double b0)
    : r(r0), g(g0), b(b0) {}
public:
  double r;
  double g;
  double b;
};

RGB* colors = NULL;

void InitializeColors()
{
  srand(time(NULL)); 
  colors = new RGB[maxIt + 1];
  for (int i = 0; i < maxIt; ++i)
    {
      if (i < 6)
        { 
          colors[i] = RGB(1, 1, 1);
        }
      else
        {
          colors[i] = RGB((float) rand()/RAND_MAX, (float) rand()/RAND_MAX, (float) rand()/RAND_MAX);
        }
    }
  colors[maxIt] = RGB(0, 0, 0); 
}


//Total size of iterations array
int winSize = sizeof(int) * WINDOW_DIM * WINDOW_DIM;

//Total result array of iterations
int *result = (int*) malloc(winSize);

void* computeIteration(void* thread_id)
{
  double nX = (maxC.real - minC.real) / WINDOW_DIM;
  double nY = (maxC.imag - minC.imag) / WINDOW_DIM;

  int rowsPerThread = WINDOW_DIM / nThreads;
  int startingRow = (unsigned long)thread_id * rowsPerThread;

  for (int col = 0; col < WINDOW_DIM; col++) {
    for (int row = startingRow; row < startingRow + rowsPerThread; row++) {

      Complex zo(0.0, 0.0);
      Complex c((minC.real + nX * row), (minC.imag + nY * col));
      Complex z((minC.real + nX * row), (minC.imag + nY * col));
      int iteration = 0;

      for(int i = 0; i < maxIt ; i++)
      {
        if(z.Mag2() > 4.0) break;
          zo = z * z + c;
          z = zo;
          iteration++;
      }

      result[row + col * WINDOW_DIM] = iteration;
    }
  }

  // This thread is done; decrement the active count and see if all
  // have finished
  pthread_mutex_lock(&startCountMutex);
  startCount--;
  if (startCount == 0)
    { // Last to exit, notify main
      pthread_mutex_unlock(&startCountMutex);
      pthread_mutex_lock(&exitMutex);
      pthread_cond_signal(&exitCond);
      pthread_mutex_unlock(&exitMutex);
    }
  else
    {
      pthread_mutex_unlock(&startCountMutex);
    }
}

void computeMBSet()
{
  startCount = nThreads; // Total threads (to be) started
  // Now start the threads
  for (int i = 0; i < nThreads; ++i) {
    // Now create the thread
    pthread_t pt; // pThread variable (output param from create)
    pthread_create(&pt, 0, computeIteration, (void*)i);
  }
  // Main program now waits until all child threads completed
  pthread_cond_wait(&exitCond, &exitMutex);
}

void drawMBSet()
{
  glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

  glBegin(GL_POINTS);
  for (int row = 0; row < WINDOW_DIM; row++) {
    for (int col = 0; col < WINDOW_DIM; col++) {
      int val = result[row * WINDOW_DIM + col];
      glColor3f(colors[val].r, colors[val].g, colors[val].b);
      glVertex2f(col, row);
    }
  }
  glEnd();

}

void display(void)
{ // Your OpenGL display code here
  glLoadIdentity();
  gluOrtho2D(0, WINDOW_DIM, WINDOW_DIM, 0);
  drawMBSet();
  glutSwapBuffers();
}

void init()
{ // Your OpenGL initialization code here
  glClearColor(1.0,1.0,1.0,1.0); // Clear background color
  glEnable(GL_DEPTH_TEST);
  glShadeModel(GL_SMOOTH);

}

void reshape(int w, int h)
{ // Your OpenGL window reshape code here
  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glViewport(0,0, (GLsizei)w, (GLsizei)h);
}

void mouse(int button, int state, int x, int y)
{ // Your mouse click processing here
  // state == 0 means pressed, state != 0 means released
  // Note that the x and y coordinates passed in are in
  // PIXELS, with y = 0 at the top.
}

void motion(int x, int y)
{ // Your mouse motion here, x and y coordinates are as above
}

void keyboard(unsigned char c, int x, int y)
{ // Your keyboard processing here
}

int main(int argc, char** argv)
{
  // Initialize all mutex and condition variables 
  pthread_mutex_init(&exitMutex,0);
  pthread_mutex_init(&startCountMutex,0);
  pthread_cond_init(&exitCond, 0);
  // Main holds the exit mutex until waiting for exitCond condition
  pthread_mutex_lock(&exitMutex);

  // Initialize OpenGL, but only on the "master" thread or process.
  // See the assignment writeup to determine which is "master" 
  // and which is slave.
  glutInit(&argc, argv);
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGBA | GLUT_DEPTH);
  glutInitWindowSize(WINDOW_DIM, WINDOW_DIM);
  glutInitWindowPosition(100, 100);
  glutCreateWindow("MBSet");
  computeMBSet();

  init();
  glMatrixMode(GL_MODELVIEW);
  InitializeColors();
  // Set your glut callbacks here
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);

  glutMainLoop();
  return 0;
}

