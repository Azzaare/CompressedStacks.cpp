#ifndef COMPRESSEDSTACK
#define COMPRESSEDSTACK

/**** Compressed Stack: declarations ****/
#include "sign.hpp"
#include "stack.hpp"
#include <string>

/* Component of a Compressed Stack */
template <class T, class D>
class Component
{
public:
  Component<T,D>(int space, int depth);

  // Setters
  void setSignature(Signature<T> sign);
  void setLastSign(int index);

  // IO
  std::string toString();
  void print();
  void println();

private:
  Levels<T> mPartial;
  Explicit<D> mExplicit;
  Signature<T>* mSign;
};


/* Compressed Stack itself */
template <class T, class D>
class CompressedStack:Stack<D>
{
public:
  CompressedStack<T,D>(int size, int space);

  // IO
  std::string toString();

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
Component<T,D>::Component(int space, int depth)
{
  mSign = nullptr;

  Levels<T> partial = initLevels<T>(space, depth);
  mPartial = partial;

  Explicit<D> xplicit;
  xplicit = initExplicit<D>();
  xplicit.resize(space);
  mExplicit = xplicit;
}

template <class T, class D>
CompressedStack<T,D>::CompressedStack(int size, int space)
{
  mSize = size;
  mSpace = space;
  mDepth = 0;
}

/** IO **/
template <class T>
std::string toString(Levels<T> levels)
{
  std::string str;
  str = "";
  int index = 0;
  for (typename Levels<T>::iterator it = levels.begin() ; it != levels.end(); ++it)
  {
    index++;
    str += "\t\t\tLevel" + std::to_string(index) + "->\n";
    str += "\t\t\t\t" + toString(*it) + "\n";
  }
  return str;
}

template <class T, class D>
std::string Component<T,D>::toString()
{
  std::string str;
  str = toString(mPartial);
  str += "\t\t\tExplicit->\n";
  str += toString(mExplicit);
  str += "\t\t\tSignature->\n";
  str += (&mSign).toString() + "\n";
  return str;
}

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
  str += toString();
  return str;
}

#endif /* COMPRESSEDSTACK */
