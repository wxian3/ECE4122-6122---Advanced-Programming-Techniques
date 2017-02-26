// Implementation of the templated Vector class
// ECE4893/8893 lab 3
// Wenqi Xian

#include <iostream> // debugging
#include "Vector.h"

// Your implementation here
// Fill in all the necessary functions below
using namespace std;

// Default constructor
template <typename T>
Vector<T>::Vector()
{
  elements = 0;
  count = 0;
}

// Copy constructor
template <typename T>
Vector<T>::Vector(const Vector& rhs)
{
  count = rhs.count;
  elements = (T*)malloc((count) * sizeof(T));   
  for (size_t i = 0; i< count; ++i) {
    new (&elements[i]) T(rhs[i]); // copy the elements
  }
}

// Assignment operator
template <typename T>
Vector<T>& Vector<T>::operator=(const Vector& rhs)
{
  if (this == &rhs) return *this; // avoid self assignment
  free(elements);
  T* elements = (T*)malloc((count) * sizeof(T)); 
  count = rhs.count;  
  for (size_t i = 0; i< count; ++i) {
    new (&elements[i]) T(rhs[i]); // copy the elements
  }
  return *this;
}

#ifdef GRAD_STUDENT
// Other constructors
template <typename T>
Vector<T>::Vector(size_t nReserved)
{ // Initialize with reserved memory
}

template <typename T>
Vector<T>::Vector(size_t n, const T& t)
{ // Initialize with "n" copies of "t"
}

template <typename T>
void Vector<T>::Reserve(size_t n)
{ // Reserve extra memory
}

#endif

// Destructor
template <typename T>
Vector<T>::~Vector()
{
  Clear();
  free(elements);
}

// Add and access front and back
template <typename T>
void Vector<T>::Push_Back(const T& rhs)
{
  
  T* tmp = (T*)malloc((count + 1) * sizeof(T)); 
  for (size_t i = 0; i < count; ++i)
  { // Copy old elements
    new (&tmp[i]) T(elements[i]);
  }
  new (&tmp[count]) T(rhs);
  for (size_t i = 0; i < count; ++i) { 
    // Destroy old
    elements[i].~T();
  }
  free(elements);
  elements = tmp;
  count++;
  
}

template <typename T>
void Vector<T>::Push_Front(const T& rhs)
{
  T* tmp = (T*)malloc((count + 1) * sizeof(T)); 
  for (size_t i = 0; i < count; ++i)
  { // Copy old elements
    new (&tmp[i + 1]) T(elements[i]);
  }
  new (&tmp[0]) T(rhs);
  for (size_t i = 0; i < count; ++i) { // Destroy old
    elements[i].~T();
  }
  free(elements);
  elements = tmp;
  count++;
}

template <typename T>
void Vector<T>::Pop_Back()
{ // Remove last element
  count--;
  elements[count].~T(); // Call destructor on the element to be removed
}

template <typename T>
void Vector<T>::Pop_Front()
{ // Remove first element
  T* tmp = (T*)malloc((count - 1) * sizeof(T)); 
  for (size_t i = 0; i < count - 1; ++i) {
    // Copy old elements
    new (&tmp[i]) T(elements[i + 1]);
  }
  for (size_t i = 0; i < count; ++i) { 
    // Destroy old
    elements[i].~T();
  }
  free(elements);
  elements = tmp;
  count--;
}

// Element Access
template <typename T>
T& Vector<T>::Front() const
{
  return elements[0];
}

// Element Access
template <typename T>
T& Vector<T>::Back() const
{
  return elements[count - 1];
}

template <typename T>
const T& Vector<T>::operator[](size_t i) const
{ // const element access
  return elements[i];
}

template <typename T>
T& Vector<T>::operator[](size_t i)
{//nonconst element access
  return elements[i];
}

template <typename T>
size_t Vector<T>::Size() const
{
  return count;
}

template <typename T>
bool Vector<T>::Empty() const
{
  return count == 0;
}

// Implement clear
template <typename T>
void Vector<T>::Clear()
{
  while (count != 0) {
    Pop_Back();
  }
}

// Iterator access functions
template <typename T>
VectorIterator<T> Vector<T>::Begin() const
{
  return VectorIterator<T>(elements);
}

template <typename T>
VectorIterator<T> Vector<T>::End() const
{
  return VectorIterator<T>(elements + count);
}

#ifdef GRAD_STUDENT
// Erase and insert
template <typename T>
void Vector<T>::Erase(const VectorIterator<T>& it)
{
}

template <typename T>
void Vector<T>::Insert(const T& rhs, const VectorIterator<T>& it)
{
}
#endif

// Implement the iterators

// Constructors
template <typename T>
VectorIterator<T>::VectorIterator()
{
  current = 0;
}

template <typename T>
VectorIterator<T>::VectorIterator(T* c)
{
  current = c;
}

// Copy constructor
template <typename T>
VectorIterator<T>::VectorIterator(const VectorIterator<T>& rhs)
{
  current = rhs.current;
}

// Iterator defeferencing operator
template <typename T>
T& VectorIterator<T>::operator*() const
{
  return *current;
}

// Prefix increment
template <typename T>
VectorIterator<T>  VectorIterator<T>::operator++()
{
  current++;
  return *this;
}

// Postfix increment
template <typename T>
VectorIterator<T> VectorIterator<T>::operator++(int)
{ 
  T* prev = current;
  current++;
  return VectorIterator<T>(prev);
}

// Comparison operators
template <typename T>
bool VectorIterator<T>::operator !=(const VectorIterator<T>& rhs) const
{
  return !(current == rhs.current);
}

template <typename T>
bool VectorIterator<T>::operator ==(const VectorIterator<T>& rhs) const
{
  return current == rhs.current;
}




