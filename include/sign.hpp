#ifndef SIGN
#define SIGN

/*==============================================================================
  Includes
==============================================================================*/
#include <vector>
#include <string>
#include <iostream>
#include <memory>

/*==============================================================================
  Class      : template (T context, D datas)
  Extensions :
  Aliases    : Block, Levels
  Friends   -> Component, CompressedStack
            <-
==============================================================================*/
template <class T, class D> class Component; // Required for the friendship
template <class T, class D> class CompressedStack; // Required for the friendship
template <class T, class D>
class Signature{
  friend class Component<T,D>;
  friend class CompressedStack<T,D>;

public:
  // IO
  std::string toString();

private:
  // Constructors
  Signature<T,D>(int index, std::streampos position, std::shared_ptr<T> context);
  Signature<T,D>(std::vector<Signature<T,D>> block);

  int mFirst;
  int mLast;
  std::streampos mPosition;
  std::shared_ptr<T> mContext;
};

/* Derived types: Block and Levels */
// A Partially Compressed Block is composed of the signatures of its SubBlocks
template<class T, class D> using Block = std::vector<Signature<T,D>>;
template<class T, class D> Block<T,D> initBlock(int space);

// Each level of compressed Blocks (first and second) are stored in Levels
template<class T, class D> using Levels = std::vector<Block<T,D>>;
template<class T, class D> Levels<T,D> initLevels(int space, int depth);

/*==============================================================================
  Constructors : Signature, initBlock, initLevels
==============================================================================*/
template <class T, class D>
Signature<T,D>::Signature(int index, std::streampos position, std::shared_ptr<T> context){
  mFirst = index;
  mLast = index;
  mPosition = position;
  mContext = context;
}

template <class T, class D>
Signature<T,D>::Signature(Block<T,D> block){
  Signature<T,D> frontSign = block.front();
  Signature<T,D> backSign = block.back();
  mFirst = frontSign.getFirst();
  mLast = backSign.getLast();
  mPosition = frontSign.getPosition();
  mContext = frontSign.getContext();
}

template <class T, class D>
Block<T,D> initBlock(int space){
  Block<T,D> block;
  block.reserve(space);
  return block;
}

template <class T, class D>
Levels<T,D> initLevels(int space, int depth){
  Levels<T,D> levels;
  for (int lvl = 1; lvl < depth; lvl++)
   {
    Block<T,D> block = initBlock<T,D>(space);
    levels.push_back(block);
   }
   levels.reserve(depth-1);

   return levels;
}

/*==============================================================================
  IO : toString, blockToString
==============================================================================*/
template <class T, class D>
std::string Signature<T,D>::toString(){
  std::string str;
  str = "(" + std::to_string(mFirst) + ",";
  str += std::to_string(mLast) + ")";
  return str;
}

template<class T, class D>
std::string blockToString(Block<T,D> block){
  std::string str;
  str = "[";
  for (typename Block<T,D>::iterator it = block.begin() ; it != block.end(); ++it)
  {
    str += "\t\t\t\t" + (*it).toString() + "\n";
  }
  str.back() = ']';
  return str;
}

#endif /* SIGN */
