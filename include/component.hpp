#ifndef COMPONENT
#define COMPONENT

/*==============================================================================
  Includes
==============================================================================*/
#include "block.hpp"
#include "stack.hpp"
#include <cstdint>
#include <memory>
#include <string>

/*==============================================================================
  Class      : template (T context, D datas)
  Extensions :
  Aliases    :
  Friends   -> CompressedStack
            <- Data, Block
==============================================================================*/
template <class T, class D, class I> class Component {
  friend class CompressedStack<T, D, I>;

private:
  Component<T, D, I>(I space, I depth);

  // Setters
  void clearExplicit(I space);

  // Push and pop
  void pushExplicit(SPData<T, D, I> elt);
  void push(Block<T, D, I> block, std::intmax_t lvl);
  Data<T, D, I> top();
  Block<T, D, I> top(std::intmax_t lvl);
  I topIndex(std::intmax_t lvl = 0);

  // IO
  std::string toString();

  // State
  bool single(std::intmax_t lvl);

  LevelVector<T, D, I> mPartial;
  ExplicitPointer<T, D, I> mExplicit;
  Block<T, D, I> mBlock;
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D, class I>
Component<T, D, I>::Component(I space, I depth)
    : mBlock(0, std::streampos(0), std::shared_ptr<T>(nullptr),
             Buffer<T, D, I>(0)) {

  LevelVector<T, D, I> partial = initLevelVector<T, D, I>(space, depth);
  mPartial = partial;

  ExplicitPointer<T, D, I> xplicit = initExplicitPointer<T, D, I>();
  xplicit.reserve(space);
  mExplicit = xplicit;
}

/*==============================================================================
  IO : levelsToStringInComponent, toString
==============================================================================*/
template <class T, class D, class I>
std::string levelsToString(LevelVector<T, D, I> levels) {
  std::string str;
  str = "";
  I index = 0;
  for (typename LevelVector<T, D, I>::iterator it = levels.begin();
       it != levels.end(); ++it) {
    index++;
    str += "\t\t\tLevel " + std::to_string(index) + "   ->";
    str += " " + levelToString(*it) + "\n";
  }
  return str;
}

template <class T, class D, class I>
std::string Component<T, D, I>::toString() {
  std::string str;
  str = levelsToString(mPartial);
  str += "\t\t\tExplicit  -> ";
  str += explicitPointerToString(mExplicit);
  str += "\n\t\t\tBlock -> ";
  str += mBlock.toString() + "\n";
  return str;
}

/*==============================================================================
  Stack Functions: push, pop, top, topIndex, isempty, isExplicitEmpty
==============================================================================*/
template <class T, class D, class I>
void Component<T, D, I>::pushExplicit(SPData<T, D, I> elt) {
  mExplicit.push_back(elt);
}
template <class T, class D, class I>
void Component<T, D, I>::push(Block<T, D, I> block, std::intmax_t lvl) {
  mPartial[lvl - 1].push_back(block);
}

template <class T, class D, class I> Data<T, D, I> Component<T, D, I>::top() {
  return *(mExplicit.back());
}
template <class T, class D, class I>
Block<T, D, I> Component<T, D, I>::top(std::intmax_t lvl) {
  return mPartial[lvl - 1].back();
}

template <class T, class D, class I>
I Component<T, D, I>::topIndex(std::intmax_t lvl) {
  if (lvl > 0 && lvl <= (int)mPartial.size()) {
    return top(lvl).mLast;
  } else {
    return top().mIndex;
  }
}

template <class T, class D, class I>
bool Component<T, D, I>::single(std::intmax_t lvl) {
  if (mPartial[lvl - 1].empty()) {
    return false;
  }
  return (mPartial[lvl - 1].back().single());
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D, class I>
void Component<T, D, I>::clearExplicit(I space) {
  mExplicit.clear();
  mExplicit.reserve(space);
}

#endif /* COMPONENT */
