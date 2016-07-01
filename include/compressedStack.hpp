#ifndef COMPRESSEDSTACK
#define COMPRESSEDSTACK

/**** Compressed Stack: declarations ****/
#include "sign.hpp"
#include "stack.hpp"
#include "component.hpp"
#include <string>
#include <cmath>

/* Compressed Stack itself */
template <class T, class D>
class CompressedStack: public Stack<D>
{
public:
  CompressedStack<T,D>(int size, int space);

  // Internals
  Data<D> top(int k);
  void push(Data<D> data);
  Data<D> pop();
  bool isempty();

  // IO
  std::string toString();
  void print();
  void println();

private:
  // Structure constraints
  int mSize;  // (Expected) size of the input in #elements
  int mSpace; // Maximum space order of the compressed stack
  int mDepth; // Depth (#levels of compression) based on size and space

  // Position of previous input (before reading)
  int mPosition;

  // First, Second, and Compressed components
  Component<T,D> mFirst;
  Component<T,D> mSecond;
  Block<T> mCompressed;
};

/** Constructors **/
template <class T, class D>
CompressedStack<T,D>::CompressedStack(int size, int space)
: mFirst(size,space)
, mSecond(size,space)
{
  mSize = size;
  mSpace = space;
  mDepth = (int) ceil(log(size)/log(space)-.1); // - 1;

  mPosition = 0;

  mCompressed = initBlock<T>(mSpace);
}

/** IO **/
template <class T, class D>
std::string CompressedStack<T,D>::toString()
{
  std::string str;
  str = "\tCompressed Stack with " + std::to_string(mSize) + " elements, ";
  str += std::to_string(mSpace) + " space order, ";
  str += std::to_string(mDepth) + " depth.\n";
  str += "\t\tFirst Component\n";
  str += mFirst.toString();
  str += "\t\tSecond Component\n";
  str += mSecond.toString();
  str += "\t\tFully Compressed\n";
  str += blockToString(mCompressed);
  return str;
}

template <class T, class D>
void CompressedStack<T,D>::print()
{
  std::cout << this->toString();
}

template <class T, class D>
void CompressedStack<T,D>::println()
{
  this->print();
  std::cout << std::endl;
}

/** Pushes, pops and accesses **/
template <class T, class D>
bool CompressedStack<T,D>::isempty(){
  return true;
}

template <class T, class D>
void CompressedStack<T,D>::push(Data<D> data)
{

}

template <class T, class D>
Data<D> CompressedStack<T,D>::pop()
{
  Data<D> d (1,1);
  return d;
}

template <class T, class D>
Data<D> CompressedStack<T,D>::top(int k)
{
  Data<D> d (1,1);
  return d;
}

#endif /* COMPRESSEDSTACK */
