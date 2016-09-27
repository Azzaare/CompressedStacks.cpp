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
template <class T, class D> class Component; // Required for the friendship
template <class T, class D>
class CompressedStack;                         // Required for the friendship
template <class T, class D> class ClassicStack; // Required for the friendship
template <class T, class D> class Block {
  friend class Component<T, D>;
  friend class CompressedStack<T, D>;
  friend class ClassicStack<T, D>;

public:
  // IO
  std::string toString();

private:
  // Constructors
  Block<T, D>(int index, std::streampos position,
                  std::shared_ptr<T> context, Buffer<T, D> buffer);
  Block<T, D>(std::vector<Block<T, D>> level);
  Block<T, D>();

  // Getters
  bool single();

  int mFirst;
  int mLast;
  std::streampos mPosition;
  std::shared_ptr<T> mContext;
  Buffer<T, D> mBuffer;
};

/* Derived types: Level and LevelVector */
// A Partially Compressed Level is composed of its blocks
template <class T, class D> using Level = std::vector<Block<T, D>>;
template <class T, class D> Level<T, D> initLevel(int space);

// Each level of compressed Levels (first and second) are stored in LevelVector
template <class T, class D> using LevelVector = std::vector<Level<T, D>>;
template <class T, class D> LevelVector<T, D> initLevelVector(int space, int depth);

/*==============================================================================
  Constructors : Block, initLevel, initLevelVector
==============================================================================*/
template <class T, class D>
Block<T, D>::Block(int index, std::streampos position,
                           std::shared_ptr<T> context, Buffer<T, D> buffer) {
  mFirst = index;
  mLast = index;
  mPosition = position;
  mContext = context;
  mBuffer = buffer;
}

template <class T, class D> Block<T, D>::Block(Level<T, D> level) {
  Block<T, D> frontBlock = level.front();
  Block<T, D> backBlock = level.back();
  mFirst = frontBlock.mFirst;
  mLast = backBlock.mLast;
  mPosition = frontBlock.mPosition;
  mContext = frontBlock.mContext;
  mBuffer = frontBlock.mBuffer;
}

template <class T, class D> Block<T, D>::Block() {
  mFirst = 0;
  mLast = 0;
  mPosition = std::streampos(0);
  mContext = std::shared_ptr<T>(nullptr);
  mBuffer = Buffer<T, D>(0);
}

template <class T, class D> Level<T, D> initLevel(int space) {
  Level<T, D> level;
  level.reserve(space);
  return level;
}

template <class T, class D> LevelVector<T, D> initLevelVector(int space, int depth) {
  LevelVector<T, D> levels;
  for (int lvl = 1; lvl < depth; lvl++) {
    Level<T, D> level = initLevel<T, D>(space);
    levels.push_back(level);
  }
  levels.reserve(depth - 1);

  return levels;
}

/*==============================================================================
  Getters : single
==============================================================================*/
template <class T, class D> bool Block<T, D>::single() {
  return (mLast == mFirst);
}

/*==============================================================================
  IO : toString, levelToString
==============================================================================*/
template <class T, class D> std::string Block<T, D>::toString() {
  std::string str;
  str = "(" + std::to_string(mFirst) + ",";
  str += std::to_string(mLast) + ")";
  return str;
}

template <class T, class D> std::string levelToString(Level<T, D> level) {
  std::string str = "";
  for (typename Level<T, D>::iterator it = level.begin(); it != level.end();
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
