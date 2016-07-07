#ifndef SIGN
#define SIGN

/*** Signature : Header ***/
#include <vector>
#include <string>
#include <iostream>
#include <memory>

template <class T>
class Signature
{
public:
  // Constructors
  Signature<T>(int index, std::streampos position, std::shared_ptr<T> context);
  Signature<T>(std::vector<Signature<T>> block);

  // Setters
  void setLast(int index);

  // Getters
  int getFirst();
  int getLast();
  std::streampos getPosition();
  std::shared_ptr<T> getContext();

  // IO
  std::string toString();
  void print();
  void println();

private:
  int mFirst;
  int mLast;
  std::streampos mPosition;
  std::shared_ptr<T> mContext;
};

/* Derived types: Block and Levels */
// A Partially Compressed Block is composed of the signatures of its SubBlocks
template<class T> using Block = std::vector<Signature<T>>;
template<class T> Block<T> initBlock(int space);

// Each level of compressed Blocks (first and second) are stored in Levels
template<class T> using Levels = std::vector<Block<T>>;
template<class T> Levels<T> initLevels(int space, int depth);

/** Constructors **/
// Signature
template <class T>
Signature<T>::Signature(int index, std::streampos position, std::shared_ptr<T> context)
{
  mFirst = index;
  mLast = index;
  mPosition = position;
  mContext = context;
}

template <class T>
Signature<T>::Signature(Block<T> block)
{
  Signature<T> frontSign = block.front();
  Signature<T> backSign = block.back();
  mFirst = frontSign.getFirst();
  mLast = backSign.getLast();
  mPosition = frontSign.getPosition();
  mContext = frontSign.getContext();
}

template <class T>
Block<T> initBlock(int space)
{
  Block<T> block;
  block.reserve(space);
  return block;
}

template <class T>
Levels<T> initLevels(int space, int depth)
{
  Levels<T> levels;
  for (int lvl = 1; lvl < depth; lvl++)
   {
    Block<T> block = initBlock<T>(space);
    levels.push_back(block);
   }
   levels.reserve(depth-1);

   return levels;
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
std::streampos Signature<T>::getPosition()
{
  return mPosition;
}
template <class T>
std::shared_ptr<T> Signature<T>::getContext()
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
std::string blockToString(Block<T> block)
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

template<class T>
std::string levelsToString(Levels<T> levels)
{
  std::string str;
  return str;
}

#endif /* SIGN */
