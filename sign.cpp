/*** Signature : implementation ***/
#include "sign.hpp"

/** Constructors **/
// Signature
template <class T>
Signature<T>::Signature(int index, int position, T context)
{
  mFirst = index;
  mLast = index;
  mPosition = position;
  mContext = context;
}

/** Setters **/
template <class T>
void Signature<T>::setLast(int index)
{
  mLast = index;
}

/** Getters **/
template <class T>
int Signature<T>::getFirst()
{
  return mFirst;
}
template <class T>
int Signature<T>::getLast()
{
  return mLast;
}
template <class T>
int Signature<T>::getPosition()
{
  return mPosition;
}
template <class T>
T Signature<T>::getContext()
{
  return mContext;
}

/** New Block and Levels **/
template<class T>
Block<T> initBlock(int space)
{
  Block<T> block;
  block.resize(space);
  return block;
}

template<class T>
Levels<T> initLevels(int space, int depth)
{
  Levels<T> levels;
  for (int lvl = 1; lvl < depth; lvl++)
   {
    Block<T> block = initBlock<T>(space);
    levels.push(block);
   }
   levels.resize(depth-1);

   return levels;
}
