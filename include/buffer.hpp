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
template <class T, class D>
class CompressedStack;                         // Required for the friendship
template <class T, class D> class Block;   // Required for the friendship
template <class T, class D> class Component;   // Required for the friendship
template <class T, class D> class ClassicStack; // Required for the friendship
template <class T, class D> class Buffer {
  friend class CompressedStack<T, D>;
  friend class ClassicStack<T, D>;
  friend class Block<T, D>;
  friend class Component<T, D>;

private:
  // Constructor
  Buffer<T, D>(int size = 0);

  // Getters
  Data<T, D> top(int k);
  SPData<T, D> topPointer(int k);

  // Setters
  void setStart(int k = 1);
  void setData(SPData<T, D> elt, int id = 1);

  // Push and Pop
  void push(SPData<T, D> elt);
  void pop(SPData<T, D> elt);

  // IO
  std::string toString();

  // Members
  int mSize;
  int mStart;
  ExplicitPointer<T, D> mExplicit;
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D> Buffer<T, D>::Buffer(int size) {
  mSize = size;
  mStart = 0;

  ExplicitPointer<T, D> xplicit = initExplicitPointer<T, D>();
  xplicit.reserve(size);
  mExplicit = xplicit;
}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D> Data<T, D> Buffer<T, D>::top(int k) {
  if (k <= mSize) {
    int index = (k + mStart - 1) % mSize; // -1 match the start of vectors at 0
    return *(mExplicit[index]);
  }
  throw "Access to a top element bigger than the size of the buffer";
}

template <class T, class D> SPData<T, D> Buffer<T, D>::topPointer(int k) {
  if (k <= mSize) {
    int index = (k + mStart - 1) % mSize; // -1 match the start of vectors at 0
    return mExplicit[index];
  }
  throw "Access to a top element bigger than the size of the buffer";
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D> void Buffer<T, D>::setStart(int k) {
  mStart = (mStart + k) % mSize;
}
template <class T, class D>
void Buffer<T, D>::setData(SPData<T, D> elt, int id) {
  int index = (id + mStart - 1) % mSize; // -1 match the start of vectors at 0
  mExplicit[index] = elt;
}

/*==============================================================================
  Stack Functions: push, pop
==============================================================================*/
template <class T, class D> void Buffer<T, D>::push(SPData<T, D> elt) {
  if (mSize > 0) {
    if ((int) mExplicit.size() < mSize) {
      mExplicit.push_back(elt);
    } else {
      setData(elt);
      setStart();
    }
  }
}

template <class T, class D> void Buffer<T, D>::pop(SPData<T, D> elt) {
  setStart(mSize - 1);
  setData(elt);
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D> std::string Buffer<T, D>::toString() {
  std::string str = "";
  if (mSize > 0) {
    str = "\n\t\tBuffer size is " + std::to_string(mSize);
    str += " and start at index " + std::to_string(mStart) + "\n";
    str += "\t\t\t";
    std::string aux = "";
    for (int i = mStart; i < mSize; i++) {
      if (aux == "") {
        aux += "{" + mExplicit[i]->toString();
      } else {
      aux += "," + mExplicit[i]->toString();
      }
    }
    for (int i = 0; i < mStart; i++) {
      aux += "," + mExplicit[i]->toString();
    }
    str += aux + "}";
  }
  return str;
}

#endif /* BUFFER */
