// ECE4893/8893 Assignment 3
//  Georgia Tech, Fall 2011

// Students should NOT hae to change this, excepting
// commenting out the GRAD_STUDENT definition (for undergrads)
// This is the definition of the templated "Vector" class
// that stores a variable number of elements of arbitrary
// type T.

// First is the "include guard
#ifndef __VECTOR_H__
#define __VECTOR_H__

// unistd.h is needed for size_t
#include <unistd.h>

// Comment out below if not a grad student
// #define GRAD_STUDENT

// Forward declaration of VectorIterator (templated) class
template <typename T> class VectorIterator;
template <typename T>
class Vector
{
 public:
  // Define the iterator type based on VectorIterator below
  // Construcors
  Vector();                     // Default constructor
  Vector(const Vector&);        // Copy constructor
  Vector& operator=(const Vector& rhs);  // Assignment operator
#ifdef GRAD_STUDENT
  // Grad students only, implement next three
  Vector(size_t);               // Construct with initial "reserved" size
  Vector(size_t n, const T& t); // Construct with "n" copies of t.
  void Reserve(size_t n);       // Ensure enough memory for "n" elements
#endif
  // Destructor
  ~Vector();                    // Destructor
  // Add and remove elements
  // Note. The normal Standard Template Library vector does not
  // have push_front" or "pop_front".  We will implement them anyway
  void Push_Back(const T&);     // Add to end of vector
  void Push_Front(const T&);    // Add to front of vector
  void Pop_Back();              // Remove last element
  void Pop_Front();             // Remove first element
  // Element access
  T&   Front() const;           // Return reference to first element
  T&   Back() const;            // Return reference to last element
  const T& operator[](size_t) const;// Return reference to specified element
  T&   operator[](size_t);     // Return reference to specified (nonconst)
  // Metrics functions
  size_t Size() const;          // Return number of elements
  bool   Empty() const;         // True if empty
  // Clear the vecor
  void   Clear();               // Remove all elements
  // Define the iterator
  // Iterator initializers
  VectorIterator<T> Begin() const;       // Get an iterator to first element
  VectorIterator<T> End() const;         // Get an iterator to one beyond end
#ifdef GRAD_STUDENT
  // Grad students only, implement the next two
  void Erase(const VectorIterator<T>&);      // Remove specified element
  void Insert(const T&, const VectorIterator<T>&); // Insert element BEFORE itr
#endif
private:
  // Member variables here
  T*     elements;  // Points to the actual elements
  size_t count;     // Number of elements in the vector
  size_t reserved;  // Reserved memory count
};

// Define the VectorIterator class
template <typename T>
class VectorIterator 
{
 public:
  // Define your iterator constructor here
  VectorIterator();
  VectorIterator(T*);
  VectorIterator(const VectorIterator&);
  // De-referencing, and incrementing operators
  // Note we are not implementing the "arrow" (->) operator
  // Note we are not implementing the decrementing operators
  T& operator*() const;
  VectorIterator operator++();          // Prefix increment  
  VectorIterator operator++(int);        // Postfix increment
  // Comparison operators
  bool operator !=(const VectorIterator&) const;
  bool operator ==(const VectorIterator&) const;
 private:
  //Member variables
  T* current;
  // Allow the Vector class access to private members
  friend class Vector<T>;
};

#endif
