// Define a variable length string class
// ECE4893/8893 Fall 2012
// George F. Riley

// First the "include guard
#ifndef __STRING_H__
#define __STRING_H__

#include <iostream>

class String 
{
public:
  String();                // Default constructor
  String(const char* st);  // Construct with initial string pointer
  String(const String&);   // Copy constructor
  ~String();               // Destructor
  // Assignment operator
  String& operator=(const String&);
  // Access to char* variable
  char* c_str() const;
  static void ClearCounts();      // Clear the constructor/destructor counts
  static void PrintCounts();      // Print the counts then clear
private:
  char* st;                // The actual string
public:
  // Static counts
  static int DefaultCount;
  static int CreateCount;
  static int CopyCount;
  static int AssignCount;
  static int DestructCount;

};

// Define a non-member printing function
std::ostream& operator<<(std::ostream&, const String& st);
#endif
