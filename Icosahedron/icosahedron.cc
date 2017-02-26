// Draw an Icosahedron
// ECE4893/8893 Project 4
// Wenqi Xian

#include <iostream>
#include <math.h>
#include <stdlib.h>
#include <GL/glut.h>
#include <GL/glext.h>
#include <GL/gl.h>
#include <GL/glu.h>

using namespace std;

#define NFACE 20
#define NVERTEX 12

#define X .525731112119133606
#define Z .850650808352039932

// These are the 12 vertices for the icosahedron
static GLfloat vdata[NVERTEX][3] = {
   {-X, 0.0, Z}, {X, 0.0, Z}, {-X, 0.0, -Z}, {X, 0.0, -Z},
   {0.0, Z, X}, {0.0, Z, -X}, {0.0, -Z, X}, {0.0, -Z, -X},
   {Z, X, 0.0}, {-Z, X, 0.0}, {Z, -X, 0.0}, {-Z, -X, 0.0}
};

// These are the 20 faces.  Each of the three entries for each
// vertex gives the 3 vertices that make the face.
static GLint tindices[NFACE][3] = {
   {0,4,1}, {0,9,4}, {9,5,4}, {4,5,8}, {4,8,1},
   {8,10,1}, {8,3,10}, {5,3,8}, {5,2,3}, {2,7,3},
   {7,10,3}, {7,6,10}, {7,11,6}, {11,0,6}, {0,1,6},
   {6,1,10}, {9,0,11}, {9,11,2}, {9,2,5}, {7,2,11} };

// Initializing gloabal variables
int testNumber; // Global variable indicating which test number is desired
int depth; // Depth value
static float rate = 10.0;
float* valR;
float* valB;
float* valG;
GLfloat rotLim = 360.0;


// Default functions
void init()
{

glClearColor(0.0, 0.0, 0.0, 0.0); // Clear background color
glShadeModel(GL_FLAT);
glEnable(GL_LINE_SMOOTH);
glLoadIdentity(); // For rotation, translation matrix calculations
// Initialize viewing transformation
gluLookAt(0.0, 0.0, 1.0, 0.0, 0.0, 0.0, 0.0, 1.0, 0.0);
}

void reshape(int w, int h)
{

  glMatrixMode(GL_PROJECTION);
  glLoadIdentity();
  glMatrixMode(GL_MODELVIEW);
  glViewport(0,0, (GLsizei)w, (GLsizei)h);
}

void timer(int)
{
  glutPostRedisplay();
  glutTimerFunc(1000.0/rate, timer, 0);
}

// Defining callback functions
//1] Draw triangle method

void DrawTriangle(GLfloat* v1, GLfloat* v2, GLfloat* v3, int face)
{
  // Drawing triangles with each face having a different color
  glBegin(GL_TRIANGLES);
    glColor3f(valR[face], valB[face], valG[face]); // Diffrent color for each face
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
  glEnd();

  glBegin(GL_LINE_LOOP);
    glColor3f(1.0, 1.0, 1.0); // White lines
    glVertex3fv(v1);
    glVertex3fv(v2);
    glVertex3fv(v3);
  glEnd();

}

void normalize(GLfloat* v)
{
  GLfloat d = sqrt(pow(v[0],2) + pow(v[1],2) + pow(v[2],2));

  v[0] = (GLfloat) v[0]/d;
  v[1] = (GLfloat) v[1]/d;
  v[2] = (GLfloat) v[2]/d;
}

void subDivide(GLfloat* v1, GLfloat* v2, GLfloat* v3, int dth, int face)
{
  int delta = depth-dth;
  int i = pow(4,delta);

  // Calculate midpoint of trianles
  GLfloat v12[3];
  GLfloat v23[3];
  GLfloat v31[3];

  for(int i = 0; i < 3; i++)
  {
    v12[i] = (GLfloat) (v1[i] + v2[i])/2.0;
    v23[i] = (GLfloat) (v2[i] + v3[i])/2.0;
    v31[i] = (GLfloat) (v3[i] + v1[i])/2.0;
  }

  normalize(v12);
  normalize(v23);
  normalize(v31);

  if(dth == 1)
  {
    DrawTriangle(v1, v12, v31, 4*face*i);
    DrawTriangle(v12, v31, v23,4*face*i+1);
    DrawTriangle(v12, v23, v2, 4*face*i+2);
    DrawTriangle(v31, v23, v3, 4*face*i+3);
  }
  else
  {
    // Recursive calls
    subDivide(v1, v12, v31, dth-1, face);
    subDivide(v12, v31, v23,dth-1, face);
    subDivide(v12, v23, v2, dth-1, face);
    subDivide(v31, v23, v3, dth-1, face);
  }
}

// Coloring algorithm
void setColor(int n)
{
  srand(time(NULL)); // Set random seed to null;

  for(int i = 0; i < n; i++)
  {
    valR[i] = (float) rand()/RAND_MAX;  // Assign random 'R' value
    valB[i] = (float) rand()/RAND_MAX;  // Assign random 'B' value
    valG[i] = (float) rand()/RAND_MAX;  // Assign random 'G' value
  }
}

// Test cases.  Fill in your code for each test case
void Test1()
{
  // Test 1: Draw simple Icosahedron
  glPushMatrix();
  for (int i = 0; i < NFACE; i++)
  {
    DrawTriangle(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], i);
  }
  glPopMatrix();
}

void Test2()
{
  // Test 2: Draw simple Icosahedron and rotate about X and Y axis
  static GLfloat rotX = 0.0;
  static GLfloat rotY = 0.0;
  glRotatef(rotX, 1.0, 0.0, 0.0); // Rotate about X-axis
  glRotatef(rotY, 0.0, 1.0, 0.0); // Rotate about Y-axis
  rotX += 1.0;
  rotY += 1.0;

  if(rotX == rotLim)
    rotX = (GLfloat)0.0;

  if(rotY == rotLim)
    rotY = (GLfloat)0.0;
// Rotate and call Test1;
  Test1();
}

void Test3()
{
  //Test 3: subdivide once
  // depth = 1;
  glPushMatrix();
  for (int i = 0; i < NFACE; i++) {
    subDivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], 1, i);
  }
  glPopMatrix();
}

void Test4()
{
  // Test 4: Subdivide once and rotate about X and Y axis
  static GLfloat rotX = 0.0;
  static GLfloat rotY = 0.0;
  glRotatef(rotX, 1.0, 0.0, 0.0); // Rotate about X-axis
  glRotatef(rotY, 0.0, 1.0, 0.0); // Rotate about Y-axis
  rotX += 1.0;
  rotY += 1.0;

  if(rotX == rotLim)
    rotX = (GLfloat)0.0;

  if(rotY == rotLim)
    rotY = (GLfloat)0.0;

// Rotate and call Test3;
  Test3();
}

void Test5(int depth)
{
  int d = depth;
  glPushMatrix();
  //Test 3: subdivide once
  // depth = variable;
  glPushMatrix();
  for (int i = 0; i < NFACE; i++) {
    subDivide(&vdata[tindices[i][0]][0], &vdata[tindices[i][1]][0], &vdata[tindices[i][2]][0], d, i);
  }
  glPopMatrix();
  glPopMatrix();
}

void Test6(int depth)
{
  int d = depth;
  // Test 5: Subdivide depending on depth and rotate about X and Y axis
  static GLfloat rotX = 0.0;
  static GLfloat rotY = 0.0;
  glRotatef(rotX, 1.0, 0.0, 0.0); // Rotate about X-axis
  glRotatef(rotY, 0.0, 1.0, 0.0); // Rotate about Y-axis
  rotX += 1.0;
  rotY += 1.0;

  if(rotX == rotLim)
    rotX = (GLfloat)0.0;

  if(rotY == rotLim)
    rotY = (GLfloat)0.0;

// Rotate and call Test5;
  Test5(d);
}

void display(void)
{
    glClear(GL_COLOR_BUFFER_BIT);
    glEnable(GL_LINE_SMOOTH);

    if(depth >= 3)
      glLineWidth(0.5); //
    else                // ---> check
      glLineWidth(1.0); //

    switch(testNumber)
    {
      case 1:
        Test1();
        break;

      case 2:
        Test2();
        break;

      case 3:
        Test3();
        break;

      case 4:
        Test4();
        break;

      case 5:
        Test5(depth);
        break;

      case 6:
        Test6(depth);
        break;

      default:
        cout<<"ERROR: testNumber Invalid."<<endl;
        exit(3);
    }

    glutSwapBuffers();  // For double buffering
}

int main(int argc, char** argv)
{
  if (argc < 2)
    {
      std::cout << "Usage: icosahedron testnumber" << endl;
      exit(1);
    }
  // Set the global test number
  testNumber = atol(argv[1]);
  if(testNumber >= 3)
    depth = 1;  // SubDivide for testNumber >= 3
  else
    depth = 0;

  // Input depth for tests 5 and 6
  if(testNumber > 4)
  {
    if(argc < 3)
    {
      cout << "ERROR: Input depth value" << endl;
      exit(2);
    }

    depth = atoi(argv[2]);

    // Check condition to limit depth value
    if(depth > 5)
    {
      cout<<"ERROR: Limit depth value to 5."<<endl;
      exit(3);
    }
  }

  glutInit(&argc, argv);
  int n = (int) NFACE * pow(4,depth);
  valR = new float[n];
  valB = new float[n];
  valG = new float[n];
  setColor(n);  // Assigning random colors for triangle faces;

  // Initialize glut  and create your window here
  glutInitDisplayMode(GLUT_DOUBLE | GLUT_RGB);
  glutInitWindowSize (500,500);
  glutInitWindowPosition (100, 100);
  glutCreateWindow("Icosahedron");
  init();

  // Set your glut callbacks here
  glutDisplayFunc(display);
  glutReshapeFunc(reshape);
  glutTimerFunc(1000.0 /rate , timer, 0);

  // Enter the glut main loop here
  glutMainLoop();

  // Freeing heap memory incase of memory leaks
  delete(valR);
  delete(valB);
  delete(valG);

  return 0;
}