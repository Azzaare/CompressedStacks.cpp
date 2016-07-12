#ifndef BUFFER
#define BUFFER

/*==============================================================================
  Includes
==============================================================================*/
#include "data.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <memory>

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    :
  Friends   -> CompressedStack
            <-
==============================================================================*/
template <class T, class D> class CompressedStack; // Required for the friendship
template <class T, class D> class Signature; // Required for the friendship
template <class T, class D> class Component; // Required for the friendship
template <class T, class D>
class Buffer{
  friend class CompressedStack<T,D>;
  friend class Signature<T,D>;
  friend class Component<T,D>;

private:
  // Constructor
  Buffer<T,D>(int size = 0);

  // Getters
  Data<T,D> top(int k);
  SPData<T,D> getPointer(int k);

  // Setters
  void setStart();
  void setData(SPData<T,D> elt, int index);

  // Push and Pop
  void push(SPData<T,D> elt);
  void pop(SPData<T,D> elt);

  // IO
  std::string toString();

  // Members
  int mSize;
  int mStart;
  ExplicitPointer<T,D> mExplicit;
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D>
Buffer<T,D>::Buffer(int size){
  mSize = size;
  mStart = 0;

  ExplicitPointer<T,D> xplicit = initExplicitPointer<T,D>();
  xplicit.reserve(size);
  mExplicit = xplicit;
}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D>
Data<T,D> Buffer<T,D>::top(int k){
  if (k < mSize) {
    int index = (k + mStart - 1) % mSize; // -1 match the start of vectors at 0
    return *(mExplicit[index]);
  }
  throw "Access to a top element bigger than the size of the buffer";
}

template <class T, class D>
SPData<T,D> Buffer<T,D>::getPointer(int k){
  if (k < mSize) {
    int index = (k + mStart - 1) % mSize; // -1 match the start of vectors at 0
    return mExplicit[index];
  }
  throw "Access to a top element bigger than the size of the buffer";
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D>
void Buffer<T,D>::setStart(){
  mStart = (mStart + 1) % mSize;
}
template <class T, class D>
void Buffer<T,D>::setData(SPData<T,D> elt, int id){
  int index = (id + mStart - 1) % mSize; // -1 match the start of vectors at 0
  mExplicit[index] = elt;
}

/*==============================================================================
  Stack Functions: push, pop
==============================================================================*/
template <class T, class D>
void Buffer<T,D>::push(SPData<T,D> elt){
  if (mSize > 0) {
    setData(elt, mStart+1);
  }
}

template <class T, class D>
void Buffer<T,D>::pop(SPData<T,D> elt){
  setStart();
  setData(elt, mSize);
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D>
std::string Buffer<T,D>::toString(){
  std::string str = "";
  if (mSize > 0) {
    str = "\t\tBuffer size is " + std::to_string(mSize);
    str += " and start at index " + std::to_string(mStart) + "\n";
    str += "\t\t\t" + explicitPointerToString(mExplicit);
  }
  return str;
}

#endif /* BUFFER */
