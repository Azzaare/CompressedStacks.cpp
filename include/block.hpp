#ifndef BLOCK
#define BLOCK

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
  Aliases    : Level, LevelVector
  Friends   -> Component, CompressedStack
            <-
==============================================================================*/
template <class T, class D, class I> class Block {
  friend class Component<T, D, I>;
  friend class CompressedStack<T, D, I>;
  friend class ClassicStack<T, D, I>;

public:
  // IO
  std::string toString();

private:
  // Constructors
  Block<T, D, I>(I index, std::streampos position, std::shared_ptr<T> context,
                 Buffer<T, D, I> buffer);
  Block<T, D, I>(std::vector<Block<T, D, I>> level);
  Block<T, D, I>();

  // Getters
  bool single();

  I mFirst;
  I mLast;
  std::streampos mPosition;
  std::shared_ptr<T> mContext;
  Buffer<T, D, I> mBuffer;
};

/* Derived types: Level and LevelVector */
// A Partially Compressed Level is composed of its blocks
template <class T, class D, class I> using Level = std::vector<Block<T, D, I>>;
template <class T, class D, class I> Level<T, D, I> initLevel(I space);

// Each level of compressed Levels (first and second) are stored in LevelVector
template <class T, class D, class I>
using LevelVector = std::vector<Level<T, D, I>>;
template <class T, class D, class I>
LevelVector<T, D, I> initLevelVector(I space, I depth);

/*==============================================================================
  Constructors : Block, initLevel, initLevelVector
==============================================================================*/
template <class T, class D, class I>
Block<T, D, I>::Block(I index, std::streampos position,
                      std::shared_ptr<T> context, Buffer<T, D, I> buffer) {
  mFirst = index;
  mLast = index;
  mPosition = position;
  mContext = context;
  mBuffer = buffer;
}

template <class T, class D, class I>
Block<T, D, I>::Block(Level<T, D, I> level) {
  Block<T, D, I> frontBlock = level.front();
  Block<T, D, I> backBlock = level.back();
  mFirst = frontBlock.mFirst;
  mLast = backBlock.mLast;
  mPosition = frontBlock.mPosition;
  mContext = frontBlock.mContext;
  mBuffer = frontBlock.mBuffer;
}

template <class T, class D, class I> Block<T, D, I>::Block() {
  mFirst = 0;
  mLast = 0;
  mPosition = std::streampos(0);
  mContext = std::shared_ptr<T>(nullptr);
  mBuffer = Buffer<T, D, I>(0);
}

template <class T, class D, class I> Level<T, D, I> initLevel(I space) {
  Level<T, D, I> level;
  level.reserve(space);
  return level;
}

template <class T, class D, class I>
LevelVector<T, D, I> initLevelVector(I space, I depth) {
  LevelVector<T, D, I> levels;
  for (I lvl = 1; lvl < depth; lvl++) {
    Level<T, D, I> level = initLevel<T, D, I>(space);
    levels.push_back(level);
  }
  levels.reserve(depth - 1);

  return levels;
}

/*==============================================================================
  Getters : single
==============================================================================*/
template <class T, class D, class I> bool Block<T, D, I>::single() {
  return (mLast == mFirst);
}

/*==============================================================================
  IO : toString, levelToString
==============================================================================*/
template <class T, class D, class I> std::string Block<T, D, I>::toString() {
  std::string str;
  str = "(" + std::to_string(mFirst) + ",";
  str += std::to_string(mLast) + ")";
  return str;
}

template <class T, class D, class I>
std::string levelToString(Level<T, D, I> level) {
  std::string str = "";
  for (typename Level<T, D, I>::iterator it = level.begin(); it != level.end();
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

#endif /* BLOCK */
