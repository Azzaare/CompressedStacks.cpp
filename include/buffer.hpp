#ifndef BUFFER
#define BUFFER

/** Buffer class : declaration **/
#include "data.hpp"
#include <vector>
#include <string>
#include <iostream>
#include <memory>

template <class D>
class Buffer
{
public:
  // Constructor
  Buffer<D>(int size);

  // Getters
  Data<D> top(int k);
  int getSize();
  int getStart();

  // Setters
  void setStart();
  void setStart(int start);
  void setData(std::shared_ptr<Data<D>> elt, int index);

  // Push and Pop
  void push(std::shared_ptr<Data<D>> elt);
  void pop();

  // IO
  std::string toString();
  void print();
  void println();

private:
  int mSize;
  int mStart;
  ExplicitPointer<D> mExplicit;
};



/** Constructors **/
template <class D>
Buffer<D>::Buffer(int size)
{
  mSize = size;
  mStart = 0;

  ExplicitPointer<D> xplicit = initExplicitPointer<D>();
  xplicit.reserve(size);
  mExplicit = xplicit;
}

/** Getters **/
template <class D>
int Buffer<D>::getSize()
{
  return mSize;
}
template <class D>
int Buffer<D>::getStart()
{
  return mStart;
}

template <class D>
Data<D> Buffer<D>::top(int k)
{
  if (k < getSize()) {
    int index = (k + mStart - 1) % mSize; // -1 match the start of vectors at 0
    return *(mExplicit[index]);
  }
  throw "Access to a top element bigger than the size of the buffer";
}

/** Setters **/
template <class D>
void Buffer<D>::setStart(int start)
{
  mStart = start;
}
template <class D>
void Buffer<D>::setStart()
{
  mStart = (mStart + 1) % mSize;
}
template <class D>
void Buffer<D>::setData(std::shared_ptr<Data<D>> elt, int id)
{
  int index = (id + mStart - 1) % mSize; // -1 match the start of vectors at 0
  mExplicit[index] = elt;
}

/** Push and Pop**/
template <class D>
void Buffer<D>::push(std::shared_ptr<Data<D>> elt)
{
  if (mSize > 0) {
    setData(elt, mStart+1);
  }
}

template <class D>
void Buffer<D>::pop()
{
  setStart();
}

/** IO **/
template <class D>
std::string Buffer<D>::toString()
{
  std::string str;
  str = "\t\tBuffer size is " + std::to_string(mSize);
  str += " and start at index " + std::to_string(mStart) + "\n";
  str += explicitPointerToString(mExplicit);
  return str;
}
template <class D>
void Buffer<D>::print()
{
  std::string str;
  str = this->toString();
  std::cout << str;
}
template <class D>
void Buffer<D>::println()
{
  this->print();
  std::cout << "\n";
}

#endif /* BUFFER */
