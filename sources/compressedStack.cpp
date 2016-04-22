/**** Compressed Stack: implementations ****/
#include "compressedStack.hpp"

/** Constructors **/
template <class T, class D>
Component<T,D>::Component(int space, int depth)
{
  mSign = nullptr;

  Levels<T> partial = initLevels<T>(space, depth);
  mPartial = partial;

  Explicit<D> xplicit;
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
