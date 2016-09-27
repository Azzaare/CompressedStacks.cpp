#ifndef COMPRESSEDSTACK
#define COMPRESSEDSTACK

/*==============================================================================
   Includes
==============================================================================*/
#include "buffer.hpp"
#include "component.hpp"
#include "data.hpp"
#include "block.hpp"
#include "stack.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <string>

/*==============================================================================
   Class      : template (T context, D datas)
   Extensions :
   Aliases    :
   Friends   -> StackAlgo
             <- Block, Component, Buffer, Data
==============================================================================*/
template <class T, class D> class StackAlgo; // Required for the friendship
template <class T, class D> class CompressedStack : public Stack<T, D> {
  friend class StackAlgo<T, D>;

public:
  CompressedStack<T, D>(int size, int space, int buffer,
                        std::shared_ptr<T> context,
                        std::streampos position = std::streampos(0));
  CompressedStack<T, D>(int size, int space, int buffer,
                        const Block<T, D> &block);

private:
  /* Required from virtual functions in Stack.hpp */
  // Empty functions (test emptyness)
  bool empty(int lvl = -1, int component = 0);
  // lvl = -1 test all levels
  // lvl = 0 test mCompressed
  // lvl = mDepth test mExplicit
  // 0 < lvl < mDepth test mPartial[lvl - 1]
  // component = 0 tests both component
  // component = i > 0 tests component i

  Data<T, D> top(int k = 1);
  Data<T, D> pop(StackAlgo<T, D> &StackAlgo);
  void push(const Data<T, D> &data);
  void copyContent(CompressedStack<T, D> &stack);
  void compress();

  // Setters required as from virtual functions in Stack.hpp
  void setContext(std::shared_ptr<T> context);
  void setPosition(std::streampos position);
  void setCompressed(Level<T, D> block);

  // Getters required as from virtual functions in Stack.hpp
  Level<T, D> getFirstPartial(int lvl);
  Level<T, D> getCompressed();
  ExplicitPointer<T, D> getFirstExplicit();
  Block<T, D> getFirstBlock();
  int getBufferSize ();

  // IO
  std::string toString();

  /* Back function (give the back element of a vector)*/
  Block<T, D> back(int lvl = 0, int component = 0);
  SPData<T, D> topPointer(int k = 1);
  void pop_back(int lvl, int component);

  /* New Internals */
  bool single(int lvl, int component);
  void setBlock(int component, Block<T, D> block = Block<T, D>());
  void setLast(int component, int index, int lvl = -1);
  int getLast(int lvl, int component = 0);
  std::shared_ptr<Block<T, D>> getBlock(int component, int lvl = 0);

  /* Push related */
  void pushExplicit(SPData<T, D> elt);
  void pushCompressed(SPData<T, D> elt, int lvl, int headIndex);
  void compress(Level<T, D> block);
  void resetLevel(Block<T, D> block, int lvl);

  /* Pop related */
  void popBuffer();
  SPData<T, D> popExplicit(StackAlgo<T, D> &StackAlgo, int component);
  void popComponent(int index, int component);
  void propagate(int index, int lvl, int component = 1);
  void clear(int index, int lvl, int component);
  void reconstruct(StackAlgo<T, D> &problem);

  // Other getters
  SPData<T, D> getExplicitData(int k);
  Level<T, D> getSmartCompress(int lvl);
  Block<T, D> getBottomBlock();

  // Structure constraints
  int mSize;  // (Expected) size of the input in #elements
  int mSpace; // Maximum space order of the compressed stack
  int mDepth; // Depth (#levels of compression) based on size and space

  // Position of previous input (before reading)
  std::streampos mPosition;

  // Pointer to the context in StackAlgo
  std::shared_ptr<T> mContext;

protected:
  // First, Second, and Compressed components
  Component<T, D> mFirst;
  Component<T, D> mSecond;
  Level<T, D> mCompressed;

  // Buffer to read the top k elements
  Buffer<T, D> mBuffer;
};

/*==============================================================================
   Constructors
==============================================================================*/
template <class T, class D>
CompressedStack<T, D>::CompressedStack(int size, int space, int buffer,
                                       std::shared_ptr<T> context,
                                       std::streampos position)
    : mFirst(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mSecond(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mBuffer(buffer) {
  mSize = size;
  mSpace = space;
  mDepth = int(ceil(log(size) / log(space) - .001) - 1);
  mPosition = position;
  mCompressed = initLevel<T, D>(mSpace);
  mContext = context;
}

template <class T, class D>
CompressedStack<T, D>::CompressedStack(int size, int space, int buffer,
                                       const Block<T, D> &block)
    : mFirst(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mSecond(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mBuffer(buffer) {
  mSize = size;
  mSpace = space;
  mDepth = int(ceil(log(size) / log(space) - .001) - 1);
  mCompressed = initLevel<T, D>(mSpace);

  mPosition = block.mPosition;
  mContext = block.mContext;
}

/*==============================================================================
   Internals
    1) emptyExplicit (int lvl, int component) : test if the explicit part of
   component is empty. If component = 0, test on all component.
    2) back(int lvl, int component) : if lvl = 0 give the back of mCompressed,
   else give the back of mPartial[lvl - 1] of component
    3-4) top/topPointer(int k) : give the k-st (/pointer to) elt in the
buffer/explicit. Index starts implicitly at 1 (for buffer index reasons)
    5) single (int lvl, int component) : return true if the block on
lvl/component has the same first and last index
    6) pop_back(int lvl, int component) : pop_back on mPartial[lvl - 1] in
component
    7) setBlock(int component, Block<T,D> block) : assign a block to component
    8) setLast(int component, int index) : set mBlock.mLast to index in component
    9) getBlock(int component, int lvl) : get the last block in component at lvl.
If lvl = 0, look in the fully compressed part
==============================================================================*/
template <class T, class D>
bool CompressedStack<T, D>::empty(int lvl, int component) {
  bool b = true;

  if (lvl == -1) {
    while (lvl < mDepth && b) {
      lvl++;
      if (component == 0 && lvl == 0) {
        b &= empty(0);
      } else if (lvl > 0) {
        b &= empty(lvl, component);
      }
    }
  } else if (lvl == mDepth) {
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

template <class T, class D>
Block<T, D> CompressedStack<T, D>::back(int lvl, int component) {
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

template <class T, class D>
SPData<T, D> CompressedStack<T, D>::topPointer(int k) {
  SPData<T, D> elt;
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

template <class T, class D> Data<T, D> CompressedStack<T, D>::top(int k) {
  return *topPointer(k);
}

template <class T, class D>
bool CompressedStack<T, D>::single(int lvl, int component) {
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

template <class T, class D>
void CompressedStack<T, D>::pop_back(int lvl, int component) {
  if (lvl == mDepth) {
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

template <class T, class D>
void CompressedStack<T, D>::setBlock(int component, Block<T, D> block) {
  if (component == 1) {
    mFirst.mBlock = block;
  } else {
    mSecond.mBlock = block;
  }
}

template <class T, class D>
void CompressedStack<T, D>::setLast(int component, int index, int lvl) {
  if (component == 0) {
    mCompressed.back().mLast = index;
  } else if (lvl == -1 || lvl == mDepth) {
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

template <class T, class D>
int CompressedStack<T, D>::getLast(int lvl, int component) {
  if (lvl == mDepth) {
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

template <class T, class D>
std::shared_ptr<Block<T, D>> CompressedStack<T, D>::getBlock(int component, int lvl) {
  if (lvl == 0) {
    return std::make_shared<Block<T, D>>(mCompressed.back());
  } else if (component == 1) {
    return std::make_shared<Block<T, D>>(mFirst.mPartial[lvl - 1].back());
  } else {
    return std::make_shared<Block<T, D>>(mSecond.mPartial[lvl - 1].back());
  }
}

/*==============================================================================
   Setters : setContext
==============================================================================*/
template <class T, class D>
void CompressedStack<T, D>::setContext(std::shared_ptr<T> context) {
  mContext = context;
}

template <class T, class D>
void CompressedStack<T, D>::setPosition(std::streampos position) {
  mPosition = position;
}

template <class T, class D>
void CompressedStack<T, D>::setCompressed(Level<T, D> block) {
  mCompressed = block;
}

/*==============================================================================
   Getters : getFirstPartial, getCompressed, getFirstExplicit, getExplicitData
==============================================================================*/
template <class T, class D>
Level<T, D> CompressedStack<T, D>::getFirstPartial(int lvl) {
  return mFirst.mPartial[lvl - 1];
}

template <class T, class D> Level<T, D> CompressedStack<T, D>::getCompressed() {
  return mCompressed;
}

template <class T, class D>
Level<T, D> CompressedStack<T, D>::getSmartCompress(int lvl) {
  Level<T, D> compressed(mCompressed);
  for (int i = lvl; i >= 0; i--) {
    if (empty(i + 1, 1)) {
      int s = mSecond.mPartial[i].size();
      if (s > 1) {
        Block<T, D> block = getBottomBlock();
        block.mLast = mSecond.mPartial[i][s - 2].mLast;
        compressed.push_back(block);
        break;
      }
    } else if (mFirst.mPartial[i].size() == 1) {
      if (!empty(i + 1, 2)) {
        Block<T, D> block = getBottomBlock();
        block.mLast = mSecond.mPartial[i].back().mLast;
        compressed.push_back(block);
        break;
      }
    } else {
      int s = mFirst.mPartial[i].size();
      Block<T, D> block = getBottomBlock();
      block.mLast = mFirst.mPartial[i][s - 2].mLast;
      compressed.push_back(block);
      break;
    }
  }
  return compressed;
}

template <class T, class D>
ExplicitPointer<T, D> CompressedStack<T, D>::getFirstExplicit() {
  return mFirst.mExplicit;
}

template <class T, class D>
SPData<T, D> CompressedStack<T, D>::getExplicitData(int k) {
  if (k <= (int)mFirst.mExplicit.size()) {
    return mFirst.mExplicit[k - 1];
  } else {
    return mFirst.mBlock.mBuffer.topPointer(k - mFirst.mExplicit.size());
  }
}

template <class T, class D>
Block<T, D> CompressedStack<T, D>::getFirstBlock() {
  return mFirst.mBlock;
}

template <class T, class D>
Block<T, D> CompressedStack<T, D>::getBottomBlock() {
  if (empty(1, 2)) {
    return mFirst.mPartial[0].front();
  } else {
    return mSecond.mPartial[0].front();
  }
}

template <class T, class D>
int CompressedStack<T, D>::getBufferSize() {
  return mBuffer.mSize;
}

/*==============================================================================
   IO : toString
==============================================================================*/
template <class T, class D> std::string CompressedStack<T, D>::toString() {
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
template <class T, class D>
void CompressedStack<T, D>::push(const Data<T, D> &elt) {
  // update the buffer (if buffer size is bigger than 0)
  SPData<T, D> ptr_elt = std::make_shared<Data<T, D>>(elt);
  mBuffer.push(ptr_elt);
  // update the explicit Level, with possibly shifting first to second
  pushExplicit(ptr_elt);
  // update the compressed Levels at each level (including fully compressed)
  for (int lvl = mDepth - 1; lvl > 0; lvl--) {
    int headIndex = getLast(lvl);
    pushCompressed(ptr_elt, lvl, headIndex);
  }
}

// Function push for the Explicit members of the stack
template <class T, class D>
void CompressedStack<T, D>::pushExplicit(SPData<T, D> elt) {
  int index = elt->mIndex;
  SPData<T, D> eltPtr = elt;

  // If the explicit datas of component 1 are empty we push
  if (empty(mDepth, 1)) {
    mFirst.pushExplicit(eltPtr);
    std::shared_ptr<T> context;
    if (mContext != nullptr) {
      context = std::make_shared<T>(*mContext);
    }
    Block<T, D> block(index, mPosition, context, mBuffer);
    mFirst.mBlock = block;
  }
  // We check if thoses explicit datas are full
  else {
    int headIndex = mFirst.topIndex();
    int startLevel = headIndex - (headIndex - 1) % mSpace;
    if (index - startLevel < mSpace) {
      mFirst.pushExplicit(eltPtr);
      mFirst.mBlock.mLast = index;
    } else {
      if ((mDepth == 1) && !empty(mDepth, 2)) {
        int distCompressed = mSpace;
        int startCompressed = headIndex - (headIndex - 1) % distCompressed;
        if (index - startCompressed < distCompressed) {
          mCompressed.back().mLast = headIndex;
        } else {
          mCompressed.push_back(mSecond.mBlock);
        }
      }
      mSecond.mBlock = mFirst.mBlock;
      std::shared_ptr<T> context = std::make_shared<T>(*mContext);
      Block<T, D> block(index, mPosition, context, mBuffer);
      mFirst.mBlock = block;
      mSecond.mExplicit = mFirst.mExplicit;
      mFirst.clearExplicit(mSpace);
      mFirst.pushExplicit(eltPtr);
    }
  }
}

// Function push for the partial and fully compressed members of the stack
template <class T, class D>
void CompressedStack<T, D>::pushCompressed(SPData<T, D> elt, int lvl,
                                           int headIndex) {
  int distBlock = std::pow(mSpace, (mDepth - lvl));
  int distLevel = distBlock * mSpace;
  int index = elt->mIndex;
  Block<T, D> block(index, mPosition, mFirst.mBlock.mContext, mBuffer);

  if (empty(lvl, 1)) {
    mFirst.push(block, lvl);
  } else {
    int startLevel = headIndex - (headIndex - 1) % distLevel;
    // distance of the new index and current block
    int delta = index - startLevel;
    if (delta < distLevel) {
      // Distance with the current Block
      int startBlock = headIndex - (headIndex - 1) % distBlock;
      int eta = index - startBlock;
      // compress new element in the top Block of the current Level
      if (eta < distBlock) {
        setLast(1, index, lvl);
      } else {
        mFirst.push(block, lvl);
      }
    } else {
      if (lvl == 1 && (!empty(1, 2))) {
        int distCompressed = std::pow(mSpace, mDepth);
        int startCompressed = headIndex - (headIndex - 1) % distCompressed;
        int gamma = index - startCompressed;
        if (gamma < distCompressed && !empty(0)) {
          setLast(0, getLast(lvl, 2));
        } else {
          mCompressed.push_back(Block<T, D>(mSecond.mPartial[0]));
        }
      }
      mSecond.mPartial[lvl - 1] = mFirst.mPartial[lvl - 1];
      resetLevel(block, lvl);
    }
  }
}

template <class T, class D>
void CompressedStack<T, D>::resetLevel(Block<T, D> block, int lvl) {
  mFirst.mPartial[lvl - 1].clear();
  mFirst.mPartial[lvl - 1].reserve(mSpace);
  mFirst.mPartial[lvl - 1].push_back(block);
}

// Function that compress the top block of mSecond to a block in mCompressed
template <class T, class D> void CompressedStack<T, D>::compress() {
  if (empty(0)) {
    Block<T, D> block(mSecond.mPartial[0]);
    mCompressed.push_back(block);
  } else {
    int topSecond;
    if (mDepth > 1) {
      topSecond = mSecond.mPartial[0].back().mLast;
      mSecond.mBlock = Block<T, D>(mSecond.mPartial[0]);
    } else {
      topSecond = mSecond.mExplicit.back()->mIndex;
    }
    int topCompressed = mCompressed.back().mLast;
    int sizeLevel = std::pow(mSpace, mDepth);
    int startSecond = topSecond + 1 - (topSecond % sizeLevel);
    int startCompressed = topCompressed + 1 - (topCompressed % sizeLevel);
    if (startSecond > startCompressed) {
      mCompressed.push_back(mSecond.mBlock);
    } else {
      mCompressed.back().mLast = topSecond;
    }
  }
}

// Function that compress the top block of mSecond to a block in mCompressed
template <class T, class D>
void CompressedStack<T, D>::compress(Level<T, D> block) {
  if (empty(0)) {
    mCompressed = block;
  } else {
    for (typename Level<T, D>::iterator it = block.begin(); it != block.end();
         ++it) {
      int topBlock = (*it).mLast;
      int topCompressed = mCompressed.back().mLast;
      int sizeLevel = std::pow(mSpace, mDepth);
      int startBlock = topBlock + 1 - (topBlock % sizeLevel);
      int startCompressed = topCompressed + 1 - (topCompressed % sizeLevel);
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
template <class T, class D>
void CompressedStack<T, D>::reconstruct(StackAlgo<T, D> &problem) {
  std::shared_ptr<Block<T, D>> block;
  int lvl;
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

  std::streampos posReminder = problem.mInput.tellg();
  int indexReminder = problem.mIndex;
  std::ios_base::iostate eofbitReminder = problem.mInput.rdstate();

  problem.mContext = std::make_shared<T>(*block->mContext);
  problem.mIndex = block->mFirst - 1;
  problem.mInput.clear();
  problem.mInput.seekg(block->mPosition);

  int auxSize = mSize;
  if (lvl > 0) {
    auxSize = std::pow(mSpace, mDepth - lvl + 1);
  }
  std::shared_ptr<Stack<T, D>> auxStack =
      std::make_shared<CompressedStack<T, D>>(
          auxSize, mSpace, mBuffer.mSize, problem.mContext, block->mPosition);

  // Swap stack pointers + run  + swap back
  swap(problem.mStack, auxStack);
  problem.run(block->mLast);
  swap(problem.mStack, auxStack);

  if (lvl == 0) {
    auxStack->copyContent(*this);
  } else {
    // Copy the First component of the reconstructed stack into the main stack
    int auxDepth = int(ceil(log(auxSize) / log(mSpace) - .001) - 1);
    int delta = mDepth - auxDepth;
    for (int i = 1; i < auxDepth; i++) {
      mSecond.mPartial[delta + i - 1] = auxStack->getFirstPartial(i);
    }
    mSecond.mExplicit = auxStack->getFirstExplicit();
    mSecond.mBlock = auxStack->getFirstBlock();
  }

  problem.mIndex = indexReminder;
  problem.mInput.seekg(posReminder);
  problem.mContext = mContext;
  problem.mInput.setstate(eofbitReminder);
}

template <class T, class D>
void CompressedStack<T, D>::copyContent(CompressedStack<T, D> &stack) {
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
template <class T, class D>
Data<T, D> CompressedStack<T, D>::pop(StackAlgo<T, D> &problem) {
  popBuffer();
  if (empty(mDepth)) {
    reconstruct(problem);
  }
  int component = 1;
  if (empty(mDepth, 1)) {
    component = 2;
  }
  Data<T, D> elt = *popExplicit(problem, component);
  popComponent(elt.mIndex, component);
  return elt;
}

template <class T, class D> void CompressedStack<T, D>::popBuffer() {
  if (mBuffer.mSize > 0) {
    SPData<T, D> elt = getExplicitData(mBuffer.mSize);
    mBuffer.pop(elt);
  }
}

template <class T, class D>
SPData<T, D> CompressedStack<T, D>::popExplicit(StackAlgo<T, D> &problem,
                                                int component) {
  SPData<T, D> elt = topPointer(1);
  pop_back(mDepth, component);
  return elt;
}

template <class T, class D>
void CompressedStack<T, D>::popComponent(int index, int component) {
  if (empty(mDepth, component)) {
    setBlock(component);
  } else {
    int newLast = getLast(mDepth);
    setLast(component, newLast);
  }
  if (mDepth > 1) {
    clear(index, mDepth - 1, component);
  }
}

template <class T, class D>
void CompressedStack<T, D>::clear(int index, int lvl, int component) {
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
      int newLast = getLast(lvl + 1);
      propagate(newLast, lvl, component);
    }
  }
}

template <class T, class D>
void CompressedStack<T, D>::propagate(int index, int lvl, int component) {
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
