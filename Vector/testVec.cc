// Test program for the ECE4893/8893 Assignment 3
// Fall 2010
// George F. Riley

#include <iostream>
#include <sstream>
#include <stdlib.h>

// Include our Vector class definition and implementation
#include "Vector.h"
#include "Vector.cc"
#include "String.h"

using namespace std;

void Test1()
{ // Just create a vector with default constructor and add
  // elements with Push_Back and Push_Front.  Then retrieve
  // with only the indexing operator.  Allows destructor to clean up
  cout << "Starting Test1" << endl;
  Vector<String> v;
  
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      v.Push_Back(String(oss.str().c_str()));
    }
    
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string front " << i;
      v.Push_Front(String(oss.str().c_str()));
    }
    
  // Print out results
  cout << "Test1 results" << endl;
  
  for (size_t i = 0; i < v.Size(); ++i)
    {
      cout << v[i] << endl;
    }
}

void Test2()
{ // Just create a vector with default constructor and add
  // elements with Push_Back and Push_Front.  Then retrieve
  // with only the Back() function and Pop_Back.  Also
  // tests "Empty"
  cout << "Starting Test2" << endl;
  Vector<String> v;
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      String tmp(oss.str().c_str());
      v.Push_Back(String(oss.str().c_str()));
    }
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string front " << i;
      v.Push_Front(String(oss.str().c_str()));
    }
  // Print out results
  cout << "Test2 results" << endl;
  
  while (!v.Empty())
    {
      String st = v.Back();
      v.Pop_Back();
      cout << st << endl;
    }
}

void Test3()
{ // Just create a vector with default constructor and add
  // elements with Push_Back and Push_Front.  Then retrieve
  // with only the front() function and Pop_Front.  Also
  // tests "Empty"
  cout << "Starting Test3" << endl;
  Vector<String> v;
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      v.Push_Back(String(oss.str().c_str()));
    }
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string front " << i;
      v.Push_Front(String(oss.str().c_str()));
    }
  // Print out results
  cout << "Test3 results" << endl;
  
  while (!v.Empty())
    {
      String st = v.Front();
      v.Pop_Front();
      cout << st << endl;
    }
}

void Test4()
{ // Same as test1 except tests the copy constructor and clear
  cout << "Starting Test4" << endl;
  Vector<String> v;
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      v.Push_Back(String(oss.str().c_str()));
    }
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string front " << i;
      v.Push_Front(String(oss.str().c_str()));
    }
  
  // Now make a copy of the vector
  Vector<String> v1(v);
  
  // Print out results
  cout << "Test4 results" << endl;
  for (size_t i = 0; i < v1.Size(); ++i)
    {
      cout << v1[i] << endl;
    }
  // Clear the vector and print again
  
  v.Clear();
  cout << "Test4 results again, should be Empty" << endl;
  for (size_t i = 0; i < v.Size(); ++i)
    {
      cout << v[i] << endl;
    }
}

void Test5()
{// Create a vector the print out using iterators
  cout << "Starting Test5" << endl;
  Vector<String> v;
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      v.Push_Back(String(oss.str().c_str()));
    }
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string front " << i;
      v.Push_Front(String(oss.str().c_str()));
    }
  // Print out results
  cout << "Test5 results" << endl;
  VectorIterator<String> it = v.Begin();
  while(it != v.End())
    {
      cout << *it++ << endl;
    }
}

void Test6()
{ // test indexing operator on left-hand-size of assignment
  cout << "Starting Test6" << endl;
  Vector<String> v;
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      v.Push_Back(String(oss.str().c_str()));
    }
  // Now use indexing operator to access (and chnage) elements
  for (size_t i = 0; i < v.Size(); ++i)
    {
      ostringstream oss;
      oss << "Hello from string replaced " << i;
      v[i] = String(oss.str().c_str());
    }
  // And print results
  cout << "Test6 results" << endl;
  for (size_t i = 0; i < v.Size(); ++i)
    {
      cout << v[i] << endl;
    }
}

  
#ifdef GRAD_STUDENT
void Test7()
{ // Test insert and erase (grad students only)
  cout << "Starting Test7" << endl;
  Vector<String> v;
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      v.Push_Back(String(oss.str().c_str()));
    }
  for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string front " << i;
      v.Push_Front(String(oss.str().c_str()));
    }
  // Now find the 25th element and insert a new string before
  VectorIterator<String> it;
  VectorIterator<String> it1 = v.End();;
  size_t i = 0;
  for (it = v.Begin(); it != v.End(); ++it)
    {
      if (++i == 25) it1 = it; // it1 is an iterator to 25th element
    }
  v.Insert(String("Inserted element before 25th"), it1);

  // Now print out using iterators
  cout << "Test7 results" << endl;
  VectorIterator<String> it2 = v.End();
  size_t i2 = 0;
  for (VectorIterator<String> it = v.Begin(); it != v.End(); ++it)
    {
      cout << *it << endl;
      if (++i2 == 10) it2 = it; // Iterator for 10th element
    }
  // Erase 10th and print again
  v.Erase(it2);
  cout << "Test7 results again, should be missing 10th element" << endl;
  for (VectorIterator<String> it = v.Begin(); it != v.End(); ++it)
    {
      cout << *it << endl;
    }
}

void Test8()
{ // Grad students only, test special constructor
  cout << "Starting Test8" << endl;
  Vector<String> v(50, String("Replicated Constructor"));
  // And print out
  cout << "Test8 results" << endl;
  for (size_t i = 0; i < v.Size(); ++i)
    {
      cout << v[i] << endl;
    }
  // Also test reserve
  Vector<String> v1;
  v1.Reserve(10);
    for (int i = 0; i < 50; ++i)
    {
      ostringstream oss;
      oss << "Hello from string back " << i;
      v1.Push_Back(String(oss.str().c_str()));
    }
    // No need t print out; the constructor counts will verify correctness
}
#endif
  

int main(int argc, char** argv)
{ // If second arg specified, it is the test number to run
  // if not specified, run all
  int testNum = -1;
  if (argc > 1) testNum = atol(argv[1]);
  String::ClearCounts();
  if (testNum < 0 || testNum == 1) { Test1(); String::PrintCounts();}
  if (testNum < 0 || testNum == 2) { Test2(); String::PrintCounts();}
  if (testNum < 0 || testNum == 3) { Test3(); String::PrintCounts();}
  if (testNum < 0 || testNum == 4) { Test4(); String::PrintCounts();}
  if (testNum < 0 || testNum == 5) { Test5(); String::PrintCounts();}
  if (testNum < 0 || testNum == 6) { Test6(); String::PrintCounts();}
#ifdef GRAD_STUDENT
  if (testNum < 0 || testNum == 7) { Test7(); String::PrintCounts();}
  if (testNum < 0 || testNum == 8) { Test8(); String::PrintCounts();}
#endif
}
