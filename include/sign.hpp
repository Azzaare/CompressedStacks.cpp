#ifndef SIGN
#define SIGN

/*** Signature : Header ***/
#include <vector>
#include <string>
#include <iostream>

template <class T>
class Signature
{
public:
  // Constructors
  Signature<T>(int index, int position, T context);

  // Setters
  void setLast(int index);

  // Getters
  int getFirst();
  int getLast();
  int getPosition();
  T getContext();

  // IO
  std::string toString();
  void print();
  void println();

private:
  int mFirst;
  int mLast;
  int mPosition;
  T mContext;
};

/* Derived types: Block and Levels */
// A Partially Compressed Block is composed of the signatures of its SubBlocks
template<class T> using Block = std::vector<T,Signature<T>>;
template<class T> Block<T> initBlock(int space);

// Each level of compressed Blocks (first and second) are stored in Levels
template<class T> using Levels = std::vector<T,Block<T>>;
template<class T> Levels<T> initLevels(int space, int depth);

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

/** IO **/
template <class T>
std::string Signature<T>::toString()
{
  std::string str;
  str = "(" + std::to_string(mFirst) + ",";
  str += std::to_string(mLast) + ")";
  return str;
}
template <class T>
void Signature<T>::print()
{
  std::string str;
  str = this->toString();
  std::cout << str;
}
template <class T>
void Signature<T>::println()
{
  this->print();
  std::cout << "\n";
}

template<class T>
std::string toString(Block<T> block)
{
  std::string str;
  str = "[";
  for (typename Block<T>::iterator it = block.begin() ; it != block.end(); ++it)
  {
    str += "\t\t\t\t" + (*it).toString() + "\n";
  }
  str.back() = ']';
  return str;
}

#endif /* SIGN */
