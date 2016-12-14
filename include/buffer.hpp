#ifndef BUFFER
#define BUFFER

/*==============================================================================
  Includes
==============================================================================*/
#include "data.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    :
  Friends   -> CompressedStack
            <-
==============================================================================*/
template <class T, class D, class I> class Block; // Required for the friendship
template <class T, class D, class I>
class ClassicStack; // Required for the friendship
template <class T, class D, class I> class Buffer {
  friend class CompressedStack<T, D, I>;
  friend class ClassicStack<T, D, I>;
  friend class Block<T, D, I>;
  friend class Component<T, D, I>;

private:
  // Constructor
  Buffer<T, D, I>(I size = 0);

  // Getters
  Data<T, D, I> top(I k);
  SPData<T, D, I> topPointer(I k);

  // Setters
  void setStart(I k = 1);
  void setData(SPData<T, D, I> elt, I id = 1);

  // Push and Pop
  void push(SPData<T, D, I> elt);
  void pop(SPData<T, D, I> elt);
  void pop();

  // IO
  std::string toString();

  // Members
  I mSize;
  I mStart;
  ExplicitPointer<T, D, I> mExplicit;
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D, class I> Buffer<T, D, I>::Buffer(I size) {
  mSize = size;
  mStart = 0;

  ExplicitPointer<T, D, I> xplicit = initExplicitPointer<T, D, I>();
  xplicit.reserve(size);
  mExplicit = xplicit;
}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D, class I> Data<T, D, I> Buffer<T, D, I>::top(I k) {
  if (k <= mExplicit.size()) {
    I index =
        (mStart - k + mSize) % mSize; // -1 match the start of vectors at 0
    return *(mExplicit[index]);
  }
  throw "Access to a top element bigger than the size of the buffer";
}

template <class T, class D, class I>
SPData<T, D, I> Buffer<T, D, I>::topPointer(I k) {
  if (k <= mExplicit.size()) {
    I index =
        (mStart - k + mSize) % mSize; // -1 match the start of vectors at 0
    return mExplicit[index];
  }
  throw "Access to a top (pointer) element bigger than the size of the buffer";
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D, class I> void Buffer<T, D, I>::setStart(I k) {
  mStart = (mStart + k) % mSize;
}
template <class T, class D, class I>
void Buffer<T, D, I>::setData(SPData<T, D, I> elt, I id) {
  I index = (id + mStart - 1) % mSize; // -1 match the start of vectors at 0
  mExplicit[index] = elt;
}

/*==============================================================================
  Stack Functions: push, pop
==============================================================================*/
template <class T, class D, class I>
void Buffer<T, D, I>::push(SPData<T, D, I> elt) {
  if (mSize > 0) {
    if (mExplicit.size() < mSize) {
      mExplicit.push_back(elt);
    } else {
      setData(elt);
      setStart();
    }
  }
}

template <class T, class D, class I>
void Buffer<T, D, I>::pop(SPData<T, D, I> elt) {
  setStart(mSize - 1);
  setData(elt);
}

template <class T, class D, class I> void Buffer<T, D, I>::pop() {
  ExplicitPointer<T, D, I> newExplicit = initExplicitPointer<T, D, I>();
  newExplicit.reserve(mSize);

  for (I i = mStart; i < mExplicit.size(); i++) {
    newExplicit.push_back(mExplicit[i]);
  }
  for (I i = 0; i < mStart; i++) {
    newExplicit.push_back(mExplicit[i]);
  }
  newExplicit.pop_back();
  mExplicit = newExplicit;
  mStart = 0;
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D, class I> std::string Buffer<T, D, I>::toString() {
  std::string str = "";
  if (mSize > 0) {
    str = "\n\t\tBuffer size is " + std::to_string(mSize);
    str += " and start at index " + std::to_string(mStart) + "\n";
    str += "\t\t\t";
    std::string aux = "";
    for (I i = mStart; i < mExplicit.size(); i++) {
      if (aux == "") {
        aux += "{" + mExplicit[i]->toString();
      } else {
        aux += "," + mExplicit[i]->toString();
      }
    }
    for (I i = 0; i < mStart; i++) {
      aux += "," + mExplicit[i]->toString();
    }
    str += aux + "}";
  }
  return str;
}

#endif /* BUFFER */
