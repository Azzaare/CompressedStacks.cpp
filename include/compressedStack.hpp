#ifndef COMPRESSEDSTACK
#define COMPRESSEDSTACK

/*==============================================================================
   Includes
   ==============================================================================*/
#include "buffer.hpp"
#include "component.hpp"
#include "data.hpp"
#include "sign.hpp"
#include "stack.hpp"
#include <algorithm>
#include <cmath>
#include <memory>
#include <string>

/*==============================================================================
   Class      : template (T context, D datas)
   Extensions :
   Aliases    :
   Friends   -> Problem
             <- Signature, Component, Buffer, Data
   ==============================================================================*/
template <class T, class D> class Problem; // Required for the friendship
template <class T, class D> class CompressedStack : public Stack<T, D> {
  friend class Problem<T, D>;

public:
  CompressedStack<T, D>(int size, int space, int buffer,
                        std::shared_ptr<T> context,
                        std::streampos position = std::streampos(0));
  CompressedStack<T, D>(int size, int space, int buffer,
                        const Signature<T, D> &sign);

private:
  /* Empty functions (test emptyness) */
  bool empty(int lvl = -1, int component = 0);
  // lvl = -1 test all levels
  // lvl = 0 test mCompressed
  // lvl = mDepth test mExplicit
  // 0 < lvl < mDepth test mPartial[lvl - 1]
  // component = 0 tests both component
  // component = i > 0 tests component i

  /* Back function (give the back element of a vector)*/
  Signature<T, D> back(int lvl = 0, int component = 0);
  SPData<T, D> topPointer(int k = 1);
  Data<T, D> top(int k = 1);

  /* New Setters */
  //  void setLast();

  /* Internals */
  int topIndex();
  int smartIndex(int index);

  // Push related
  void push(const Data<T, D> &data);
  void pushExplicit(SPData<T, D> elt);
  void pushCompressed(SPData<T, D> elt, int lvl, int headIndex);
  void compress();
  void compress(Block<T, D> block);
  void resetBlock(Signature<T, D> sign, int lvl);

  // Pop related
  Data<T, D> pop(Problem<T, D> &Problem);
  void popBuffer();
  SPData<T, D> popExplicit(Problem<T, D> &Problem, int component);
  void popComponent(int index, int component);
  void propagate(int index, int lvl, int component = 1);
  void clear(int index, int lvl, int component);

  // Reconstruct related
  void reconstruct(Problem<T, D> &problem);
  void reconstruct(Problem<T, D> &problem, const Signature<T, D> &sign,
                   int lvl);
  void copyContent(CompressedStack<T, D> &stack);

  // Setters
  void setContext(std::shared_ptr<T> context);
  void setPosition(std::streampos position);
  void setCompressed(Block<T, D> block);

  // Getters
  Block<T, D> getFirstPartial(int lvl);
  Block<T, D> getCompressed();
  ExplicitPointer<T, D> getFirstExplicit();
  SPData<T, D> getExplicitData(int k);
  Signature<T, D> getFirstSign();
  Block<T, D> getSmartCompress(int lvl);
  Signature<T, D> getBottomSign();
  Signature<T, D> getTop2Partial(int lvl);

  // IO
  std::string toString();

  // Structure constraints
  int mSize;  // (Expected) size of the input in #elements
  int mSpace; // Maximum space order of the compressed stack
  int mDepth; // Depth (#levels of compression) based on size and space

  // Position of previous input (before reading)
  std::streampos mPosition;

  // Pointer to the context in Problem
  std::shared_ptr<T> mContext;

protected:
  // First, Second, and Compressed components
  Component<T, D> mFirst;
  Component<T, D> mSecond;
  Block<T, D> mCompressed;

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
  mCompressed = initBlock<T, D>(mSpace);
  mContext = context;
}

template <class T, class D>
CompressedStack<T, D>::CompressedStack(int size, int space, int buffer,
                                       const Signature<T, D> &sign)
    : mFirst(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mSecond(space, int(ceil(log(size) / log(space) - .001)) - 1),
      mBuffer(buffer) {
  mSize = size;
  mSpace = space;
  mDepth = int(ceil(log(size) / log(space) - .001) - 1);
  mCompressed = initBlock<T, D>(mSpace);

  mPosition = sign.mPosition;
  mContext = sign.mContext;
}

/*==============================================================================
   New Internals
    1) emptyExplicit (int lvl, int component) : test if the explicit part of
   component is empty. If component = 0, test on all component.
    2) back(int lvl, int component) : if lvl = 0 give the back of mCompressed,
   else give the back of mPartial[lvl - 1] of component
    3) top(int k) : give the k-st elt in the buffer/explicit. Index starts
   implicitly at 1 (for buffer index reasons)
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
Signature<T, D> CompressedStack<T, D>::back(int lvl, int component) {
  if (lvl == mDepth) {
    if (component == 1) {
      return mFirst.mSign;
    } else {
      return mSecond.mSign;
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
void CompressedStack<T, D>::setCompressed(Block<T, D> block) {
  mCompressed = block;
}

/*==============================================================================
   Getters : getFirstPartial, getCompressed, getFirstExplicit, getExplicitData
   ==============================================================================*/
template <class T, class D>
Block<T, D> CompressedStack<T, D>::getFirstPartial(int lvl) {
  return mFirst.mPartial[lvl];
}

template <class T, class D>
Signature<T, D> CompressedStack<T, D>::getTop2Partial(int lvl) {
  int sizeFirst = mFirst.mPartial[lvl].size();
  int sizeSecond = mSecond.mPartial[lvl].size();
  if (sizeFirst + sizeSecond < 2) {
    throw "Incorrect access in smartIndex/getTop2Partial";
  }
  if (sizeFirst >= 2) {
    return mFirst.mPartial[lvl][sizeFirst - 2];
  } else {
    return mSecond.mPartial[lvl][sizeSecond + sizeFirst - 2];
  }
}

template <class T, class D> Block<T, D> CompressedStack<T, D>::getCompressed() {
  return mCompressed;
}

template <class T, class D>
Block<T, D> CompressedStack<T, D>::getSmartCompress(int lvl) {
  Block<T, D> compressed(mCompressed);
  for (int i = lvl; i >= 0; i--) {
    if (empty(i + 1, 1)) {
      int s = mSecond.mPartial[i].size();
      if (s > 1) {
        Signature<T, D> sign = getBottomSign();
        sign.mLast = mSecond.mPartial[i][s - 2].mLast;
        compressed.push_back(sign);
        break;
      }
    } else if (mFirst.mPartial[i].size() == 1) {
      if (!empty(i + 1, 2)) {
        Signature<T, D> sign = getBottomSign();
        sign.mLast = mSecond.mPartial[i].back().mLast;
        compressed.push_back(sign);
        break;
      }
    } else {
      int s = mFirst.mPartial[i].size();
      Signature<T, D> sign = getBottomSign();
      sign.mLast = mFirst.mPartial[i][s - 2].mLast;
      compressed.push_back(sign);
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
    return mFirst.mSign.mBuffer.topPointer(k - 1 - mFirst.mExplicit.size());
  }
}

template <class T, class D>
Signature<T, D> CompressedStack<T, D>::getFirstSign() {
  return mFirst.mSign;
}

template <class T, class D>
Signature<T, D> CompressedStack<T, D>::getBottomSign() {
  if (empty(1, 2)) {
    return mFirst.mPartial[0].front();
  } else {
    return mSecond.mPartial[0].front();
  }
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
  str += blockToString(mCompressed);
  str += mBuffer.toString();
  return str;
}

/*==============================================================================
   Push Functions: resetBlock, push, pushExplicit, pushCompressed
   ==============================================================================*/
// Function that compress the top block of mSecond to a sign in mCompressed
template <class T, class D> void CompressedStack<T, D>::compress() {
  if (empty(0)) {
    Signature<T, D> sign(mSecond.mPartial[0]);
    mCompressed.push_back(sign);
  } else {
    int topSecond;
    if (mDepth > 1) {
      topSecond = mSecond.mPartial[0].back().mLast;
      mSecond.mSign = Signature<T, D>(mSecond.mPartial[0]);
    } else {
      topSecond = (*mSecond.mExplicit.back()).mIndex;
    }
    int topCompressed = mCompressed.back().mLast;
    int sizeBlock = std::pow(mSpace, mDepth);
    int startSecond = topSecond + 1 - (topSecond % sizeBlock);
    int startCompressed = topCompressed + 1 - (topCompressed % sizeBlock);
    if (startSecond > startCompressed) {
      mCompressed.push_back(mSecond.mSign);
    } else {
      mCompressed.back().mLast = topSecond;
    }
  }
}

// Function that compress the top block of mSecond to a sign in mCompressed
template <class T, class D>
void CompressedStack<T, D>::compress(Block<T, D> block) {
  if (empty(0)) {
    mCompressed = block;
  } else {
    for (typename Block<T, D>::iterator it = block.begin(); it != block.end();
         ++it) {
      int topSign = (*it).mLast;
      int topCompressed = mCompressed.back().mLast;
      int sizeBlock = std::pow(mSpace, mDepth);
      int startSign = topSign + 1 - (topSign % sizeBlock);
      int startCompressed = topCompressed + 1 - (topCompressed % sizeBlock);
      if (startSign > startCompressed) {
        mCompressed.push_back((*it));
      } else {
        mCompressed.back().mLast = topSign;
      }
    }
  }
}

template <class T, class D>
void CompressedStack<T, D>::resetBlock(Signature<T, D> sign, int lvl) {
  mFirst.mPartial[lvl].clear();
  mFirst.mPartial[lvl].reserve(std::pow(mSpace, mDepth + 1 - lvl));
  mFirst.mPartial[lvl].push_back(sign);
}

// Function push that push the data in explicit and index in partial/compressed
template <class T, class D>
void CompressedStack<T, D>::push(const Data<T, D> &elt) {
  int headIndex = topIndex();
  // update the buffer (if buffer size is bigger than 0)
  SPData<T, D> ptr_elt = std::make_shared<Data<T, D>>(elt);
  mBuffer.push(ptr_elt);
  // update the explicit Blocks, with possibly shifting first to second
  pushExplicit(ptr_elt);
  // update the compressed Blocks at each levels (including fully compressed)
  for (int lvl = 0; lvl < mDepth - 1; lvl++) {
    pushCompressed(ptr_elt, lvl, headIndex);
  }
}

// Function push for the Explicit members of the stack
template <class T, class D>
void CompressedStack<T, D>::pushExplicit(SPData<T, D> elt) {
  int index = elt->mIndex;
  SPData<T, D> eltPtr = elt;

  // If the explicit datas of component 1 are empty we push
  if (mFirst.isExplicitEmpty()) {
    mFirst.pushExplicit(eltPtr);
    std::shared_ptr<T> context = std::make_shared<T>(*mContext);
    Signature<T, D> sign(index, mPosition, context, mBuffer);
    mFirst.mSign = sign;
  }
  // We check if thoses explicit datas are full
  else {
    int headIndex = mFirst.topIndex();
    int startBlock = headIndex - (headIndex - 1) % mSpace;
    if (index - startBlock < mSpace) {
      mFirst.pushExplicit(eltPtr);
      mFirst.mSign.mLast = index;
    } else {
      if ((mDepth == 1) && (!(mSecond.isExplicitEmpty()))) {
        int distCompressed = mSpace;
        int startCompressed = headIndex - (headIndex - 1) % distCompressed;
        if (index - startCompressed < distCompressed) {
          mCompressed.back().mLast = headIndex;
        } else {
          mCompressed.push_back(mSecond.mSign);
        }
      }
      mSecond.mSign = mFirst.mSign;
      std::shared_ptr<T> context = std::make_shared<T>(*mContext);
      Signature<T, D> sign(index, mPosition, context, mBuffer);
      mFirst.mSign = sign;
      mSecond.mExplicit = mFirst.mExplicit;
      mFirst.clearExplicit(mSpace);
      mFirst.pushExplicit(eltPtr);
    }
  }
}

// Function push for the part. and fully compressed members of the stack
template <class T, class D>
void CompressedStack<T, D>::pushCompressed(SPData<T, D> elt, int lvl,
                                           int headIndex) {
  int distSubBlock = std::pow(mSpace, (mDepth - lvl - 1));
  int distBlock = distSubBlock * mSpace;
  int index = elt->mIndex;
  Signature<T, D> sign(index, mPosition, mFirst.mSign.mContext, mBuffer);

  if (empty(lvl + 1, 1)) {
    mFirst.push(sign, lvl);
  } else {
    int startBlock = headIndex - (headIndex - 1) % distBlock;
    // distance of the new index and current block
    int delta = index - startBlock;
    if (delta < distBlock) {
      // Distance with the current subblock
      int headSubBlock = mFirst.mPartial[lvl].back().mLast;
      int startSubBlock = headSubBlock - (headSubBlock - 1) % distSubBlock;
      int eta = index - startSubBlock;
      // compress new element in the top of the current Block
      if (eta < distSubBlock) {
        mFirst.mPartial[lvl].back().mLast = index;
      } else {
        mFirst.push(sign, lvl);
      }
    } else {
      if (lvl == 0 && (!empty(1, 2))) {
        int distCompressed = std::pow(mSpace, mDepth);
        int startCompressed = headIndex - (headIndex - 1) % distCompressed;
        int gamma = index - startCompressed;
        if (gamma < distCompressed && !empty(0)) {
          mCompressed.back().mLast = mSecond.mPartial[0].back().mLast;
        } else {
          mCompressed.push_back(Signature<T, D>(mSecond.mPartial[0]));
        }
      }
      mSecond.mPartial[lvl] = mFirst.mPartial[lvl];
      resetBlock(sign, lvl);
    }
  }
}
/*==============================================================================
   Reconstruct Functions:
   ==============================================================================*/
// Look for the first level of information available to reconstruct
template <class T, class D>
void CompressedStack<T, D>::reconstruct(Problem<T, D> &problem) {
  for (int i = 1; i <= mDepth; i++) {
    int lvl = mDepth - 1 - i; // lvl == -1 is for the fully compressed part
    if (lvl < 0) {
      Signature<T, D> sign = Signature<T, D>(mCompressed.back());
      reconstruct(problem, sign, lvl + 1);
      break;
    } else {
      if (!empty(lvl + 1, 1)) {
        Signature<T, D> sign = Signature<T, D>(mFirst.mPartial[lvl].back());
        reconstruct(problem, sign, lvl + 1);
        break;
      } else if (!empty(lvl + 1, 2)) {
        Signature<T, D> sign = Signature<T, D>(mSecond.mPartial[lvl].back());
        reconstruct(problem, sign, lvl + 1);
        break;
      }
    }
  }
}

// Reconstruct an auxiliary stack based on the signature found above
template <class T, class D>
void CompressedStack<T, D>::reconstruct(Problem<T, D> &problem,
                                        const Signature<T, D> &sign, int lvl) {
  Signature<T, D> sign2 = sign;
  std::cout << "Reconstruct on " << sign2.toString() << std::endl;

  std::streampos posReminder = problem.mInput.tellg();
  int indexReminder = problem.mIndex;
  std::ios_base::iostate eofbitReminder = problem.mInput.rdstate();

  problem.mContext = std::make_shared<T>(*sign.mContext);
  problem.mIndex = sign.mFirst - 1;
  problem.mInput.clear();
  problem.mInput.seekg(sign.mPosition);

  int auxSize = std::pow(mSpace, mDepth - lvl + 1);
  std::shared_ptr<Stack<T, D>> auxStack =
      std::make_shared<CompressedStack<T, D>>(auxSize, mSpace, mBuffer.mSize,
                                              problem.mContext, sign.mPosition);

  // Swap stack pointers + run  + swap back
  swap(problem.mStack, auxStack);
  problem.run(sign.mLast);
  swap(problem.mStack, auxStack);

  if (lvl == 0) {
    (*auxStack).copyContent(*this);
  } else {
    // Copy the First component of the reconstructed stack into the main stack
    int auxDepth = int(ceil(log(auxSize) / log(mSpace) - .001)) - 1;
    int delta = mDepth - auxDepth;
    for (int i = 0; i < auxDepth - 1; i++) {
      mSecond.mPartial[delta + i] = (*auxStack).getFirstPartial(i);
    }

    mSecond.mExplicit = (*auxStack).getFirstExplicit();
    mSecond.mSign = (*auxStack).getFirstSign();
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
    7) smartIndex
   ==============================================================================*/
// TODO: Reorganize pop functions

template <class T, class D>
Data<T, D> CompressedStack<T, D>::pop(Problem<T, D> &problem) {
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
SPData<T, D> CompressedStack<T, D>::popExplicit(Problem<T, D> &problem,
                                                int component) {
  SPData<T, D> elt;
  if (component == 1) {
    elt = mFirst.mExplicit.back();
    mFirst.mExplicit.pop_back();
  } else {
    elt = mSecond.mExplicit.back();
    mSecond.mExplicit.pop_back();
  }
  return elt;
}

template <class T, class D>
void CompressedStack<T, D>::popComponent(int oldIndex, int component) {
  int index = smartIndex(oldIndex);
  if (component == 1) {
    if (empty(mDepth, 1)) {
      mFirst.mSign = Signature<T, D>();
    } else {
      int temp = std::min(mFirst.mSign.mLast, index);
      mFirst.mSign.mLast = temp;
    }
    if (mDepth > 1) {
      clear(index, mDepth - 2, 1);
    }
  } else {
    if (empty(mDepth, 2)) {
      mSecond.mSign = Signature<T, D>();
    } else {
      int temp = std::min(mSecond.mSign.mLast, index);
      mSecond.mSign.mLast = temp;
    }

    if (mDepth > 1) {
      clear(index, mDepth - 2, 2);
    }
  }
}

// TODO: Simplify clear
template <class T, class D>
void CompressedStack<T, D>::clear(int index, int lvl, int component) {
  if (component == 1) {
    if (mFirst.single(lvl)) {
      mFirst.mPartial[lvl].pop_back();
      if (lvl > 0) {
        clear(index, lvl - 1, 1);
      } else if (empty(1, 1)) {
        mFirst.mSign = Signature<T, D>();
      }
    } else {
      propagate(index, lvl);
    }
  } else { // component == 2
    if (!empty(lvl + 1, 1)) {
      clear(index, lvl, 1);
    } else {
      if (mSecond.single(lvl)) {
        mSecond.mPartial[lvl].pop_back();
        if (lvl > 0) {
          clear(index, lvl - 1, 2);
        } else if (empty(1, 2)) {
          mSecond.mSign = Signature<T, D>();
        }
      } else {
        propagate(index, lvl, 2);
      }
    }
  }
}

template <class T, class D>
void CompressedStack<T, D>::propagate(int index, int lvl, int component) {
  while (component > 1 && lvl > 0) {
    if (empty(lvl + 1, 1)) {
      mSecond.mPartial[lvl].back().mLast = index;
      lvl--;
    } else {
      component = 1;
    }
  }
  while (lvl > 0) {
    mFirst.mPartial[lvl].back().mLast = index;
    lvl--;
  }
}

// TODO: check correctness of smartIndex
template <class T, class D> int CompressedStack<T, D>::smartIndex(int index) {
  if (mBuffer.mSize > 0) {
    return mBuffer.top(1).mIndex;
  }
  if (!empty(mDepth, 1)) {
    return (*mFirst.mExplicit.back()).mIndex;
  }
  if (!empty(mDepth, 2)) {
    return (*mSecond.mExplicit.back()).mIndex;
  }

  for (int lvl = mDepth - 2; lvl >= 0; lvl--) {
    if (!mFirst.single(lvl)) {
    }
    if (mFirst.mPartial[lvl].size() + mSecond.mPartial[lvl].size() > 1) {
      return getTop2Partial(lvl).mLast;
    }
  }

  if (!empty(0)) {
    return mCompressed.back().mLast;
  }
  return 0;
}

// TODO: Check and move topIndex
template <class T, class D> int CompressedStack<T, D>::topIndex() {
  if (empty(mDepth, 1)) {
    if (empty(mDepth, 2)) {
      if (empty(0)) {
        return mCompressed.back().mLast;
      } else {
        return 0;
      }
    }
    return (*(mSecond.mExplicit.back())).mIndex;
  }
  return (*(mFirst.mExplicit.back())).mIndex;
}

#endif /* COMPRESSEDSTACK */
