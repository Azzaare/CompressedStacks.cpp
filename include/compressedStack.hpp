#ifndef COMPRESSEDSTACK
#define COMPRESSEDSTACK

/*==============================================================================
   Includes
==============================================================================*/
#include "block.hpp"
#include "buffer.hpp"
#include "component.hpp"
#include "data.hpp"
#include "stack.hpp"
#include <algorithm>
#include <cmath>
#include <cstdint>
#include <memory>
#include <string>

/*==============================================================================
   Class      : template (T context, D datas)
   Extensions :
   Aliases    :
   Friends   -> StackAlgo
             <- Block, Component, Buffer, Data
==============================================================================*/
template <class T, class D, class I>
class CompressedStack : public Stack<T, D, I> {
  friend class StackAlgo<T, D, I>;

public:
  CompressedStack<T, D, I>(I size, I space, I buffer,
                           std::shared_ptr<T> context,
                           std::streampos position = std::streampos(0));
  CompressedStack<T, D, I>(I size, I space, I buffer,
                           const Block<T, D, I> &block);

  // private:
  /* Required from virtual functions in Stack.hpp */
  // Empty functions (test emptyness)
  bool empty(std::intmax_t lvl = -1, I component = 0);
  // lvl = -1 test all levels
  // lvl = 0 test mCompressed
  // lvl = mDepth test mExplicit
  // 0 < lvl < mDepth test mPartial[lvl - 1]
  // component = 0 tests both component
  // component = i > 0 tests component i

  Data<T, D, I> top(I k = 1);
  Data<T, D, I> pop(StackAlgo<T, D, I> &StackAlgo);
  void push(const Data<T, D, I> &data);
  void copyContent(CompressedStack<T, D, I> &stack);
  void compress();

  // Setters required as from virtual functions in Stack.hpp
  void setContext(std::shared_ptr<T> context);
  void setPosition(std::streampos position);
  void setCompressed(Level<T, D, I> block);

  // Getters required as from virtual functions in Stack.hpp
  Level<T, D, I> getFirstPartial(std::intmax_t lvl);
  Level<T, D, I> getCompressed();
  ExplicitPointer<T, D, I> getFirstExplicit();
  Block<T, D, I> getFirstBlock();
  I getBufferSize();

  // IO
  std::string toString();

  /* Back function (give the back element of a vector)*/
  Block<T, D, I> back(std::intmax_t lvl = 0, I component = 0);
  SPData<T, D, I> topPointer(I k = 1);
  void pop_back(std::intmax_t lvl, I component);

  /* New Internals */
  bool single(std::intmax_t lvl, I component);
  void setBlock(I component, Block<T, D, I> block = Block<T, D, I>());
  void setLast(I component, I index, std::intmax_t lvl = -1);
  I getLast(std::intmax_t lvl, I component = 0);
  std::shared_ptr<Block<T, D, I>> getBlock(I component, std::intmax_t lvl = 0);

  /* Push related */
  void pushExplicit(SPData<T, D, I> elt);
  void pushCompressed(SPData<T, D, I> elt, std::intmax_t lvl, I headIndex);
  void compress(Level<T, D, I> block);
  void resetLevel(Block<T, D, I> block, std::intmax_t lvl);

  /* Pop related */
  void popBuffer();
  SPData<T, D, I> popExplicit(StackAlgo<T, D, I> &StackAlgo, I component);
  void popComponent(I index, I component);
  void propagate(I index, std::intmax_t lvl, I component = 1);
  void clear(I index, std::intmax_t lvl, I component);
  int reconstruct(StackAlgo<T, D, I> &problem);

  // Other getters
  SPData<T, D, I> getExplicitData(I k);
  Level<T, D, I> getSmartCompress(std::intmax_t lvl);
  Block<T, D, I> getBottomBlock();

  // Structure constraints
  I mSize;  // (Expected) size of the input in #elements
  I mSpace; // Maximum space order of the compressed stack
  I mDepth; // Depth (#levels of compression) based on size and space

  // Position of previous input (before reading)
  std::streampos mPosition;

  // Pointer to the context in StackAlgo
  std::shared_ptr<T> mContext;

protected:
  // First, Second, and Compressed components
  Component<T, D, I> mFirst;
  Component<T, D, I> mSecond;
  Level<T, D, I> mCompressed;

  // Buffer to read the top k elements
  Buffer<T, D, I> mBuffer;

  // Vector to count the number of reconstructions at the top level
  std::vector<int> mReconstructions;
};

/*==============================================================================
   Constructors
==============================================================================*/
template <class T, class D, class I>
CompressedStack<T, D, I>::CompressedStack(I size, I space, I buffer,
                                          std::shared_ptr<T> context,
                                          std::streampos position)
    : mFirst(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mSecond(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mBuffer(buffer) {
  mSize = size;
  mSpace = space;
  mDepth = int(ceil(log(size) / log(space) - .001) - 1);
  mPosition = position;
  mCompressed = initLevel<T, D, I>(mSpace);
  mContext = context;
  mReconstructions = std::vector<int>(mDepth + 1, 0);
}

template <class T, class D, class I>
CompressedStack<T, D, I>::CompressedStack(I size, I space, I buffer,
                                          const Block<T, D, I> &block)
    : mFirst(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mSecond(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mBuffer(buffer) {
  mSize = size;
  mSpace = space;
  mDepth = int(ceil(log(size) / log(space) - .001) - 1);
  mCompressed = initLevel<T, D, I>(mSpace);

  mPosition = block.mPosition;
  mContext = block.mContext;
  mReconstructions = std::vector<int>(mDepth + 1, 0);
}

/*==============================================================================
   Internals
    1) emptyExplicit (std::intmax_t lvl, I component) : test if the explicit
part of
   component is empty. If component = 0, test on all component.
    2) back(std::intmax_t lvl, I component) : if lvl = 0 give the back of
mCompressed,
   else give the back of mPartial[lvl - 1] of component
    3-4) top/topPointer(I k) : give the k-st (/pointer to) elt in the
buffer/explicit. Index starts implicitly at 1 (for buffer index reasons)
    5) single (std::intmax_t lvl, I component) : return true if the block on
lvl/component has the same first and last index
    6) pop_back(std::intmax_t lvl, I component) : pop_back on mPartial[lvl - 1]
in
component
    7) setBlock(I component, Block<T,D> block) : assign a block to component
    8) setLast(I component, I index) : set mBlock.mLast to index in
component
    9) getBlock(I component, std::intmax_t lvl) : get the last block in
component at
lvl.
If lvl = 0, look in the fully compressed part
==============================================================================*/
template <class T, class D, class I>
bool CompressedStack<T, D, I>::empty(std::intmax_t lvl, I component) {
  bool b = true;

  if (lvl == -1) {
    while (lvl < (int)mDepth && b) {
      lvl++;
      if (component == 0 && lvl == 0) {
        b &= empty(0);
      } else if (lvl > 0) {
        b &= empty(lvl, component);
      }
    }
  } else if (lvl == (int)mDepth) {
    if (component == 0 || component == 1) {
      b &= mFirst.mExplicit.empty();
    }
    if (component == 0 || component == 2) {
      b &= mSecond.mExplicit.empty();
    }
  } else if (lvl > 0) {
    if (component == 0 || component == 1) {
      b &= mFirst.mPartial[lvl - 1].empty();
    }
    if (component == 0 || component == 2) {
      b &= mSecond.mPartial[lvl - 1].empty();
    }
  } else if (lvl == 0 && component == 0) {
    b &= mCompressed.empty();
  }
  return b;
}

template <class T, class D, class I>
Block<T, D, I> CompressedStack<T, D, I>::back(std::intmax_t lvl, I component) {
  if (lvl == mDepth) {
    if (component == 1) {
      return mFirst.mBlock;
    } else {
      return mSecond.mBlock;
    }
  } else if (lvl > 0) {
    if (component == 1) {
      return mFirst.mPartial[lvl - 1].back();
    }
    if (component == 2) {
      return mSecond.mPartial[lvl - 1].back();
    }
  } else if (lvl == 0) {
    return mCompressed.back();
  }
}

template <class T, class D, class I>
SPData<T, D, I> CompressedStack<T, D, I>::topPointer(I k) {
  SPData<T, D, I> elt;
  if (mBuffer.mSize < k) {
    if (empty(mDepth, 1)) {
      elt = mSecond.mExplicit.back();
    } else {
      elt = mFirst.mExplicit.back();
    }
  } else {
    elt = mBuffer.topPointer(k);
  }
  return elt;
}

template <class T, class D, class I>
Data<T, D, I> CompressedStack<T, D, I>::top(I k) {
  return *topPointer(k);
}

template <class T, class D, class I>
bool CompressedStack<T, D, I>::single(std::intmax_t lvl, I component) {
  bool b;
  if (component == 1) {
    b = mFirst.single(lvl);
  } else if (component == 2) {
    b = mSecond.single(lvl);
  } else { // Check if the first block in the lvl is single
    if (empty(lvl, 1)) {
      b = single(lvl, 2);
    } else {
      b = single(lvl, 1);
    }
  }
  return b;
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::pop_back(std::intmax_t lvl, I component) {
  if (lvl == (int)mDepth) {
    if (component == 1) {
      mFirst.mExplicit.pop_back();
    } else {
      mSecond.mExplicit.pop_back();
    }
  } else {
    if (component == 1) {
      mFirst.mPartial[lvl - 1].pop_back();
    } else {
      mSecond.mPartial[lvl - 1].pop_back();
    }
  }
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::setBlock(I component, Block<T, D, I> block) {
  if (component == 1) {
    mFirst.mBlock = block;
  } else {
    mSecond.mBlock = block;
  }
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::setLast(I component, I index,
                                       std::intmax_t lvl) {
  if (component == 0) {
    mCompressed.back().mLast = index;
  } else if (lvl == -1 || lvl == (int)mDepth) {
    if (component == 1) {
      mFirst.mBlock.mLast = index;
    } else {
      mSecond.mBlock.mLast = index;
    }
  } else {
    if (component == 1) {
      mFirst.mPartial[lvl - 1].back().mLast = index;
    } else {
      mSecond.mPartial[lvl - 1].back().mLast = index;
    }
  }
}

template <class T, class D, class I>
I CompressedStack<T, D, I>::getLast(std::intmax_t lvl, I component) {
  if (lvl == (int)mDepth) {
    if (empty(mDepth)) {
      return 0;
    } else if (component == 2) {
      return mSecond.mExplicit.back()->mIndex;
    } else if (component == 1) {
      return mFirst.mExplicit.back()->mIndex;
    } else {
      return top(1).mIndex;
    }
  } else {
    if (component < 2 && !empty(lvl, 1)) {
      return mFirst.mPartial[lvl - 1].back().mLast;
    } else if (component != 1 && !empty(lvl, 2)) {
      return mSecond.mPartial[lvl - 1].back().mLast;
    } else {
      return 0;
    }
  }
}

template <class T, class D, class I>
std::shared_ptr<Block<T, D, I>>
CompressedStack<T, D, I>::getBlock(I component, std::intmax_t lvl) {
  if (lvl == 0) {
    return std::make_shared<Block<T, D, I>>(mCompressed.back());
  } else if (component == 1) {
    return std::make_shared<Block<T, D, I>>(mFirst.mPartial[lvl - 1].back());
  } else {
    return std::make_shared<Block<T, D, I>>(mSecond.mPartial[lvl - 1].back());
  }
}

/*==============================================================================
   Setters : setContext
==============================================================================*/
template <class T, class D, class I>
void CompressedStack<T, D, I>::setContext(std::shared_ptr<T> context) {
  mContext = context;
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::setPosition(std::streampos position) {
  mPosition = position;
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::setCompressed(Level<T, D, I> block) {
  mCompressed = block;
}

/*==============================================================================
   Getters : getFirstPartial, getCompressed, getFirstExplicit, getExplicitData
==============================================================================*/
template <class T, class D, class I>
Level<T, D, I> CompressedStack<T, D, I>::getFirstPartial(std::intmax_t lvl) {
  return mFirst.mPartial[lvl - 1];
}

template <class T, class D, class I>
Level<T, D, I> CompressedStack<T, D, I>::getCompressed() {
  return mCompressed;
}

template <class T, class D, class I>
Level<T, D, I> CompressedStack<T, D, I>::getSmartCompress(std::intmax_t lvl) {
  Level<T, D, I> compressed(mCompressed);
  for (I i = lvl; i >= 0; i--) {
    if (empty(i + 1, 1)) {
      I s = mSecond.mPartial[i].size();
      if (s > 1) {
        Block<T, D, I> block = getBottomBlock();
        block.mLast = mSecond.mPartial[i][s - 2].mLast;
        compressed.push_back(block);
        break;
      }
    } else if (mFirst.mPartial[i].size() == 1) {
      if (!empty(i + 1, 2)) {
        Block<T, D, I> block = getBottomBlock();
        block.mLast = mSecond.mPartial[i].back().mLast;
        compressed.push_back(block);
        break;
      }
    } else {
      I s = mFirst.mPartial[i].size();
      Block<T, D, I> block = getBottomBlock();
      block.mLast = mFirst.mPartial[i][s - 2].mLast;
      compressed.push_back(block);
      break;
    }
  }
  return compressed;
}

template <class T, class D, class I>
ExplicitPointer<T, D, I> CompressedStack<T, D, I>::getFirstExplicit() {
  return mFirst.mExplicit;
}

template <class T, class D, class I>
SPData<T, D, I> CompressedStack<T, D, I>::getExplicitData(I k) {
  if (k <= mFirst.mExplicit.size()) {
    return mFirst.mExplicit[k - 1];
  } else {
    return mFirst.mBlock.mBuffer.topPointer(k - mFirst.mExplicit.size());
  }
}

template <class T, class D, class I>
Block<T, D, I> CompressedStack<T, D, I>::getFirstBlock() {
  return mFirst.mBlock;
}

template <class T, class D, class I>
Block<T, D, I> CompressedStack<T, D, I>::getBottomBlock() {
  if (empty(1, 2)) {
    return mFirst.mPartial[0].front();
  } else {
    return mSecond.mPartial[0].front();
  }
}

template <class T, class D, class I>
I CompressedStack<T, D, I>::getBufferSize() {
  return mBuffer.mSize;
}

/*==============================================================================
   IO : toString
==============================================================================*/
template <class T, class D, class I>
std::string CompressedStack<T, D, I>::toString() {
  std::string str;
  str = "\tCompressed Stack with " + std::to_string(mSize) + " elements, ";
  str += std::to_string(mSpace) + " space order, ";
  str += std::to_string(mDepth) + " depth.\n";
  str += "\t\tFirst Component\n";
  str += mFirst.toString();
  str += "\t\tSecond Component\n";
  str += mSecond.toString();
  str += "\t\tFully Compressed  -> ";
  str += levelToString(mCompressed);
  str += mBuffer.toString();
  return str;
}

/*==============================================================================
  Push Functions:
    1) push
    2) pushExplicit
    3) pushCompressed
    4) resetLevel
==============================================================================*/
// Function push that push the data in explicit and index in partial/compressed
template <class T, class D, class I>
void CompressedStack<T, D, I>::push(const Data<T, D, I> &elt) {
  // update the buffer (if buffer size is bigger than 0)
  SPData<T, D, I> ptr_elt = std::make_shared<Data<T, D, I>>(elt);
  mBuffer.push(ptr_elt);
  // update the explicit Level, with possibly shifting first to second
  pushExplicit(ptr_elt);
  // update the compressed Levels at each level (including fully compressed)
  for (std::intmax_t lvl = mDepth - 1; lvl > 0; lvl--) {
    I headIndex = getLast(lvl);
    pushCompressed(ptr_elt, lvl, headIndex);
  }
}

// Function push for the Explicit members of the stack
template <class T, class D, class I>
void CompressedStack<T, D, I>::pushExplicit(SPData<T, D, I> elt) {
  I index = elt->mIndex;
  SPData<T, D, I> eltPtr = elt;

  // If the explicit datas of component 1 are empty we push
  if (empty(mDepth, 1)) {
    mFirst.pushExplicit(eltPtr);
    std::shared_ptr<T> context;
    if (mContext != nullptr) {
      context = std::make_shared<T>(*mContext);
    }
    Block<T, D, I> block(index, mPosition, context, mBuffer);
    mFirst.mBlock = block;
  }
  // We check if thoses explicit datas are full
  else {
    I headIndex = mFirst.topIndex();
    I startLevel = headIndex - (headIndex - 1) % mSpace;
    if (index - startLevel < mSpace) {
      mFirst.pushExplicit(eltPtr);
      mFirst.mBlock.mLast = index;
    } else {
      if ((mDepth == 1) && !empty(mDepth, 2)) {
        I distCompressed = mSpace;
        I startCompressed = headIndex - (headIndex - 1) % distCompressed;
        if (index - startCompressed < distCompressed) {
          mCompressed.back().mLast = headIndex;
        } else {
          mCompressed.push_back(mSecond.mBlock);
        }
      }
      mSecond.mBlock = mFirst.mBlock;
      std::shared_ptr<T> context = std::make_shared<T>(*mContext);
      Block<T, D, I> block(index, mPosition, context, mBuffer);
      mFirst.mBlock = block;
      mSecond.mExplicit = mFirst.mExplicit;
      mFirst.clearExplicit(mSpace);
      mFirst.pushExplicit(eltPtr);
    }
  }
}

// Function push for the partial and fully compressed members of the stack
template <class T, class D, class I>
void CompressedStack<T, D, I>::pushCompressed(SPData<T, D, I> elt,
                                              std::intmax_t lvl, I headIndex) {
  I distBlock = std::pow(mSpace, (mDepth - lvl));
  I distLevel = distBlock * mSpace;
  I index = elt->mIndex;
  Block<T, D, I> block(index, mPosition, mFirst.mBlock.mContext, mBuffer);

  if (empty(lvl, 1)) {
    mFirst.push(block, lvl);
  } else {
    I startLevel = headIndex - (headIndex - 1) % distLevel;
    // distance of the new index and current block
    I delta = index - startLevel;
    if (delta < distLevel) {
      // Distance with the current Block
      I startBlock = headIndex - (headIndex - 1) % distBlock;
      I eta = index - startBlock;
      // compress new element in the top Block of the current Level
      if (eta < distBlock) {
        setLast(1, index, lvl);
      } else {
        mFirst.push(block, lvl);
      }
    } else {
      if (lvl == 1 && (!empty(1, 2))) {
        I distCompressed = std::pow(mSpace, mDepth);
        I startCompressed = headIndex - (headIndex - 1) % distCompressed;
        I gamma = index - startCompressed;
        if (gamma < distCompressed && !empty(0)) {
          setLast(0, getLast(lvl, 2));
        } else {
          mCompressed.push_back(Block<T, D, I>(mSecond.mPartial[0]));
        }
      }
      mSecond.mPartial[lvl - 1] = mFirst.mPartial[lvl - 1];
      resetLevel(block, lvl);
    }
  }
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::resetLevel(Block<T, D, I> block,
                                          std::intmax_t lvl) {
  mFirst.mPartial[lvl - 1].clear();
  mFirst.mPartial[lvl - 1].reserve(mSpace);
  mFirst.mPartial[lvl - 1].push_back(block);
}

// Function that compress the top block of mSecond to a block in mCompressed
template <class T, class D, class I> void CompressedStack<T, D, I>::compress() {
  if (empty(0)) {
    Block<T, D, I> block(mSecond.mPartial[0]);
    mCompressed.push_back(block);
  } else {
    I topSecond;
    if (mDepth > 1) {
      topSecond = mSecond.mPartial[0].back().mLast;
      mSecond.mBlock = Block<T, D, I>(mSecond.mPartial[0]);
    } else {
      topSecond = mSecond.mExplicit.back()->mIndex;
    }
    I topCompressed = mCompressed.back().mLast;
    I sizeLevel = std::pow(mSpace, mDepth);
    I startSecond = topSecond + 1 - (topSecond % sizeLevel);
    I startCompressed = topCompressed + 1 - (topCompressed % sizeLevel);
    if (startSecond > startCompressed) {
      mCompressed.push_back(mSecond.mBlock);
    } else {
      mCompressed.back().mLast = topSecond;
    }
  }
}

// Function that compress the top block of mSecond to a block in mCompressed
template <class T, class D, class I>
void CompressedStack<T, D, I>::compress(Level<T, D, I> block) {
  if (empty(0)) {
    mCompressed = block;
  } else {
    for (typename Level<T, D, I>::iterator it = block.begin();
         it != block.end(); ++it) {
      I topBlock = (*it).mLast;
      I topCompressed = mCompressed.back().mLast;
      I sizeLevel = std::pow(mSpace, mDepth);
      I startBlock = topBlock + 1 - (topBlock % sizeLevel);
      I startCompressed = topCompressed + 1 - (topCompressed % sizeLevel);
      if (startBlock > startCompressed) {
        mCompressed.push_back((*it));
      } else {
        mCompressed.back().mLast = topBlock;
      }
    }
  }
}

/*==============================================================================
  Reconstruct Functions:
    1) reconstruct
    2) copyContent
==============================================================================*/
template <class T, class D, class I>
int CompressedStack<T, D, I>::reconstruct(StackAlgo<T, D, I> &problem) {
  std::shared_ptr<Block<T, D, I>> block;
  std::intmax_t lvl;
  for (lvl = mDepth; lvl >= 0; lvl--) {
    if (lvl == 0) {
      block = getBlock(0);
      break;
    } else {
      if (!empty(lvl, 1)) {
        block = getBlock(1, lvl);
        break;
      } else if (!empty(lvl, 2)) {
        block = getBlock(2, lvl);
        break;
      }
    }
  }
  // Add the reconstruction for level lvl in mReconstructions
  mReconstructions[lvl]++;

  std::streampos posReminder = problem.mInput.tellg();
  I indexReminder = problem.mIndex;
  std::ios_base::iostate eofbitReminder = problem.mInput.rdstate();

  problem.mContext = std::make_shared<T>(*block->mContext);
  problem.mIndex = block->mFirst - 1;
  problem.mInput.clear();
  problem.mInput.seekg(block->mPosition);

  I auxSize = mSize;
  if (lvl > 0) {
    auxSize = std::pow(mSpace, mDepth - lvl + 1);
  }
  std::shared_ptr<Stack<T, D, I>> auxStack =
      std::make_shared<CompressedStack<T, D, I>>(
          auxSize, mSpace, mBuffer.mSize, problem.mContext, block->mPosition);

  // Swap stack pointers + run  + swap back
  swap(problem.mStack, auxStack);
  problem.run(block->mLast);
  swap(problem.mStack, auxStack);

  if (lvl == 0) {
    auxStack->copyContent(*this);
  } else {
    // Copy the First component of the reconstructed stack into the main stack
    I auxDepth = int(ceil(log(auxSize) / log(mSpace) - .001) - 1);
    I delta = mDepth - auxDepth;
    for (I i = 1; i < auxDepth; i++) {
      mSecond.mPartial[delta + i - 1] = auxStack->getFirstPartial(i);
    }
    mSecond.mExplicit = auxStack->getFirstExplicit();
    mSecond.mBlock = auxStack->getFirstBlock();
  }

  problem.mIndex = indexReminder;
  problem.mInput.seekg(posReminder);
  problem.mContext = mContext;
  problem.mInput.setstate(eofbitReminder);

  return lvl;
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::copyContent(CompressedStack<T, D, I> &stack) {
  stack.mFirst = mFirst;
  stack.mSecond = mSecond;
  stack.mCompressed.pop_back();
  stack.mBuffer = mBuffer;
}

/*==============================================================================
   Pop Functions:
    1) pop
    2) popBuffer
    3) popExplicit
    4) popComponent
    5) clear
    6) propagate
==============================================================================*/
template <class T, class D, class I>
Data<T, D, I> CompressedStack<T, D, I>::pop(StackAlgo<T, D, I> &problem) {
  popBuffer();
  if (empty(mDepth)) {
    reconstruct(problem);
  }
  I component = 1;
  if (empty(mDepth, 1)) {
    component = 2;
  }
  Data<T, D, I> elt = *popExplicit(problem, component);
  popComponent(elt.mIndex, component);
  return elt;
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::popBuffer() {
  if (mBuffer.mSize > 0) {
    SPData<T, D, I> elt = getExplicitData(mBuffer.mSize);
    mBuffer.pop(elt);
  }
}

template <class T, class D, class I>
SPData<T, D, I>
CompressedStack<T, D, I>::popExplicit(StackAlgo<T, D, I> &problem,
                                      I component) {
  SPData<T, D, I> elt = topPointer(1);
  pop_back(mDepth, component);
  return elt;
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::popComponent(I index, I component) {
  if (empty(mDepth, component)) {
    setBlock(component);
  } else {
    I newLast = getLast(mDepth);
    setLast(component, newLast);
  }
  if (mDepth > 1) {
    clear(index, mDepth - 1, component);
  }
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::clear(I index, std::intmax_t lvl, I component) {
  if (component == 2 && !empty(lvl, 1)) {
    clear(index, lvl, 1);
  } else {
    if (single(lvl, component)) {
      pop_back(lvl, component);
      if (lvl > 1) {
        clear(index, lvl - 1, component);
      } else if (empty(1, component)) {
        setBlock(component);
      }
    } else {
      I newLast = getLast(lvl + 1);
      propagate(newLast, lvl, component);
    }
  }
}

template <class T, class D, class I>
void CompressedStack<T, D, I>::propagate(I index, std::intmax_t lvl,
                                         I component) {
  while (lvl > 0) {
    if (component > 1 && !empty(lvl, 1)) {
      component = 1;
    } else {
      setLast(component, index, lvl);
      lvl--;
    }
  }
}

#endif /* COMPRESSEDSTACK */
