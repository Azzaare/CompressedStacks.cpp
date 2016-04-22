/**** Compressed Stack: declarations ****/
#include "sign.hpp"
#include "stack.hpp"

/* Component of a Compressed Stack */
template <class T, class D>
class Component
{
public:
  Component<T,D>(int space, int depth);

  // Setters
  void setSignature(Signature<T> sign);

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
