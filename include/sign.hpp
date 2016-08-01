#ifndef SIGN
#define SIGN

/*==============================================================================
  Includes
==============================================================================*/
#include "buffer.hpp"
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*==============================================================================
  Class      : template (T context, D datas)
  Extensions :
  Aliases    : Block, Levels
  Friends   -> Component, CompressedStack
            <-
==============================================================================*/
template <class T, class D> class Component; // Required for the friendship
template <class T, class D>
class CompressedStack;                         // Required for the friendship
template <class T, class D> class NormalStack; // Required for the friendship
template <class T, class D> class Signature {
  friend class Component<T, D>;
  friend class CompressedStack<T, D>;
  friend class NormalStack<T, D>;

public:
  // IO
  std::string toString();

private:
  // Constructors
  Signature<T, D>(int index, std::streampos position,
                  std::shared_ptr<T> context, Buffer<T, D> buffer);
  Signature<T, D>(std::vector<Signature<T, D>> block);
  Signature<T, D>();

  // Getters
  bool single();

  int mFirst;
  int mLast;
  std::streampos mPosition;
  std::shared_ptr<T> mContext;
  Buffer<T, D> mBuffer;
};

/* Derived types: Block and Levels */
// A Partially Compressed Block is composed of the signatures of its SubBlocks
template <class T, class D> using Block = std::vector<Signature<T, D>>;
template <class T, class D> Block<T, D> initBlock(int space);

// Each level of compressed Blocks (first and second) are stored in Levels
template <class T, class D> using Levels = std::vector<Block<T, D>>;
template <class T, class D> Levels<T, D> initLevels(int space, int depth);

/*==============================================================================
  Constructors : Signature, initBlock, initLevels
==============================================================================*/
template <class T, class D>
Signature<T, D>::Signature(int index, std::streampos position,
                           std::shared_ptr<T> context, Buffer<T, D> buffer) {
  mFirst = index;
  mLast = index;
  mPosition = position;
  mContext = context;
  mBuffer = buffer;
}

template <class T, class D> Signature<T, D>::Signature(Block<T, D> block) {
  Signature<T, D> frontSign = block.front();
  Signature<T, D> backSign = block.back();
  mFirst = frontSign.mFirst;
  mLast = backSign.mLast;
  mPosition = frontSign.mPosition;
  mContext = frontSign.mContext;
  mBuffer = frontSign.mBuffer;
}

template <class T, class D> Signature<T, D>::Signature() {
  mFirst = 0;
  mLast = 0;
  mPosition = std::streampos(0);
  mContext = std::shared_ptr<T>(nullptr);
  mBuffer = Buffer<T, D>(0);
}

template <class T, class D> Block<T, D> initBlock(int space) {
  Block<T, D> block;
  block.reserve(space);
  return block;
}

template <class T, class D> Levels<T, D> initLevels(int space, int depth) {
  Levels<T, D> levels;
  for (int lvl = 1; lvl < depth; lvl++) {
    Block<T, D> block = initBlock<T, D>(space);
    levels.push_back(block);
  }
  levels.reserve(depth - 1);

  return levels;
}

/*==============================================================================
  Getters : single
==============================================================================*/
template <class T, class D> bool Signature<T, D>::single() {
  return (mLast == mFirst);
}

/*==============================================================================
  IO : toString, blockToString
==============================================================================*/
template <class T, class D> std::string Signature<T, D>::toString() {
  std::string str;
  str = "(" + std::to_string(mFirst) + ",";
  str += std::to_string(mLast) + ")";
  return str;
}

template <class T, class D> std::string blockToString(Block<T, D> block) {
  std::string str = "";
  for (typename Block<T, D>::iterator it = block.begin(); it != block.end();
       ++it) {
    if (str == "") {
      str += "[" + (*it).toString();
    } else {
      str += "," + (*it).toString();
    }
  }
  if (str != "") {
    str += ']';
  }
  return str;
}

#endif /* SIGN */
