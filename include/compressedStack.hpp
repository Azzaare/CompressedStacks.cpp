#ifndef COMPRESSEDSTACK
#define COMPRESSEDSTACK

/*==============================================================================
  Includes
==============================================================================*/
#include "sign.hpp"
#include "stack.hpp"
#include "component.hpp"
#include "data.hpp"
#include "buffer.hpp"
#include <string>
#include <cmath>
#include <memory>
#include <algorithm>

/*==============================================================================
  Class      : template (T context, D datas)
  Extensions :
  Aliases    :
  Friends   -> Problem
            <- Signature, Component, Buffer, Data
==============================================================================*/
template <class T, class D> class Problem; // Required for the friendship
template <class T, class D>
class CompressedStack: public Stack<T,D>{
  friend class Problem<T,D>;

public:
  CompressedStack<T,D>(int size, int space, int buffer, std::shared_ptr<T> context, std::streampos position = std::streampos(0));
  CompressedStack<T,D>(int size, int space, int buffer, const Signature<T,D> &sign);

private:
/* Internals */
  Data<T,D> top(int k);
  bool isempty();
  bool isSecondEmpty();
  Data<T,D> top();
  int topIndex();
  int smartIndex(int index);


// Push related
  void push(const Data<T,D> &data);
  void pushExplicit(SPData<T,D> elt);
  void pushCompressed(SPData<T,D> elt, int lvl, int headIndex);
  void compress();
  void compress(Block<T,D> block);
  void resetBlock(Signature<T,D> sign, int lvl);

// Pop related
  Data<T,D> pop(Problem<T,D> &Problem);
  void popBuffer();
  void popFirst(int index);
  void popSecond(int index);
  void propagateFirst(int index, int lvl);
  void propagateSecond(int index, int lvl);
  void emptyFirst(int index, int lvl);
  void emptySecond(int index, int lvl);

// Reconstruct related
  void reconstruct(Problem<T,D> &problem);
  void reconstruct(Problem<T,D> &problem,const Signature<T,D> &sign, int lvl);
  void copyContent(CompressedStack<T,D> &stack);

  // Setters
  void setContext(std::shared_ptr<T> context);
  void setPosition(std::streampos position);
  void setCompressed(Block<T,D> block);

  // Getters
  Block<T,D> getFirstPartial(int lvl);
  Block<T,D> getCompressed();
  ExplicitPointer<T,D> getFirstExplicit();
  SPData<T,D> getExplicitData(int k);
  Signature<T,D> getFirstSign();
  Block<T,D> getSmartCompress(int lvl);
  Signature<T,D> getBottomSign();
  Signature<T,D> getTop2Partial(int lvl);

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
  Component<T,D> mFirst;
  Component<T,D> mSecond;
  Block<T,D> mCompressed;

  // Buffer to read the top k elements
  Buffer<T,D> mBuffer;


};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D>
CompressedStack<T,D>::CompressedStack(int size, int space, int buffer, std::shared_ptr<T> context, std::streampos position)
: mFirst(space, int( ceil(log(size)/log(space)-.001)) - 1)
, mSecond(space, int( ceil(log(size)/log(space)-.001)) - 1)
, mBuffer(buffer){
  mSize = size;
  mSpace = space;
  mDepth = int( ceil(log(size)/log(space)-.001) - 1);
  mPosition = position;
  mCompressed = initBlock<T,D>(mSpace);
  mContext = context;
}

template <class T, class D>
CompressedStack<T,D>::CompressedStack(int size, int space, int buffer, const Signature<T,D> &sign)
: mFirst(space, int( ceil(log(size)/log(space)-.001)) - 1)
, mSecond(space, int( ceil(log(size)/log(space)-.001)) - 1)
, mBuffer(buffer){
  mSize = size;
  mSpace = space;
  mDepth = int( ceil(log(size)/log(space)-.001) - 1);
  mCompressed = initBlock<T,D>(mSpace);

  mPosition = sign.mPosition;
  mContext = sign.mContext;
}

/*==============================================================================
  Setters : setContext
==============================================================================*/
template <class T, class D>
void CompressedStack<T,D>::setContext(std::shared_ptr<T> context){
  mContext = context;
}

template <class T, class D>
void CompressedStack<T,D>::setPosition(std::streampos position){
  mPosition = position;
}

template <class T, class D>
void CompressedStack<T,D>::setCompressed(Block<T,D> block){
  mCompressed = block;
}

/*==============================================================================
  Getters : getFirstPartial, getCompressed, getFirstExplicit, getExplicitData
==============================================================================*/
template <class T, class D>
Block<T,D> CompressedStack<T,D>::getFirstPartial(int lvl){
  return mFirst.mPartial[lvl];
}

template <class T, class D>
Signature<T,D> CompressedStack<T,D>::getTop2Partial(int lvl){
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

template <class T, class D>
Block<T,D> CompressedStack<T,D>::getCompressed(){
  return mCompressed;
}

template <class T, class D>
Block<T,D> CompressedStack<T,D>::getSmartCompress(int lvl){
  Block<T,D> compressed(mCompressed);
  for (int i = lvl; i >= 0; i--) {
    if (mFirst.mPartial[i].empty()) {
      int s = mSecond.mPartial[i].size();
      if (s > 1) {
        Signature<T,D> sign = getBottomSign();
        sign.mLast = mSecond.mPartial[i][s-2].mLast;
        compressed.push_back(sign);
        break;
      }
    } else if (mFirst.mPartial[i].size() == 1) {
      if (!mSecond.mPartial[i].empty()) {
        Signature<T,D> sign = getBottomSign();
        sign.mLast = mSecond.mPartial[i].back().mLast;
        compressed.push_back(sign);
        break;
      }
    } else {
      int s = mFirst.mPartial[i].size();
      Signature<T,D> sign = getBottomSign();
      sign.mLast = mFirst.mPartial[i][s-2].mLast;
      compressed.push_back(sign);
      break;
    }
  }
  return compressed;
}

template <class T, class D>
ExplicitPointer<T,D> CompressedStack<T,D>::getFirstExplicit(){
  return mFirst.mExplicit;
}

template <class T, class D>
SPData<T,D> CompressedStack<T,D>::getExplicitData(int k){
  if (k <= (int) mFirst.mExplicit.size()) {
    return mFirst.mExplicit[k-1];
  } else {
    return mFirst.mSign.mBuffer.getPointer(k - 1 - mFirst.mExplicit.size());
  }
}

template <class T, class D>
Signature<T,D> CompressedStack<T,D>::getFirstSign(){
  return mFirst.mSign;
}

template <class T, class D>
Signature<T,D> CompressedStack<T,D>::getBottomSign(){
  if (mSecond.mPartial[0].empty()) {
    return mFirst.mPartial[0].front();
  } else {
    return mSecond.mPartial[0].front();
  }
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D>
std::string CompressedStack<T,D>::toString(){
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
  Stack Functions: isempty, compress, isSecondEmpty
==============================================================================*/
template <class T, class D>
bool CompressedStack<T,D>::isSecondEmpty(){
  bool b = mSecond.mExplicit.empty();
  for (int lvl = 0; lvl < mDepth - 2; lvl++) {
    b = b && mSecond.mPartial[lvl].empty();
  }
  return b;
}

template <class T, class D>
bool CompressedStack<T,D>::isempty(){
  return (mFirst.isempty() && mSecond.isempty() && mCompressed.empty());
}

/*==============================================================================
  Push Functions: resetBlock, push, pushExplicit, pushCompressed
==============================================================================*/
// Function that compress the top block of mSecond to a sign in mCompressed
template <class T, class D>
void CompressedStack<T,D>::compress(){
  if (mCompressed.empty()) {
    Signature<T,D> sign(mSecond.mPartial[0]);
    mCompressed.push_back(sign);
  } else {
    int topSecond;
    if (mDepth > 1) {
      topSecond = mSecond.mPartial[0].back().mLast;
      mSecond.mSign = Signature<T,D>(mSecond.mPartial[0]);
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
void CompressedStack<T,D>::compress(Block<T,D> block){
  if (mCompressed.empty()) {
    mCompressed = block;
  } else {
    for (typename Block<T,D>::iterator it = block.begin() ; it != block.end(); ++it) {
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
void CompressedStack<T,D>::resetBlock(Signature<T,D> sign, int lvl){
  mFirst.mPartial[lvl].clear();
  mFirst.mPartial[lvl].reserve(std::pow(mSpace, mDepth + 1 - lvl));
  mFirst.mPartial[lvl].push_back(sign);
}

// Function push that push the data in explicit and index in partial/compressed
template <class T, class D>
void CompressedStack<T,D>::push(const Data<T,D> &elt){
  int headIndex = topIndex();
  // update the buffer (if buffer size is bigger than 0)
  SPData<T,D> ptr_elt = std::make_shared<Data<T,D>>(elt);
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
void CompressedStack<T,D>::pushExplicit(SPData<T,D> elt){
  int index = elt->mIndex;
  SPData<T,D> eltPtr = elt;


  // If the explicit datas of component 1 are empty we push
  if (mFirst.isExplicitEmpty()) {
    mFirst.pushExplicit(eltPtr);
    std::shared_ptr<T> context = std::make_shared<T>(*mContext);
    Signature<T,D> sign (index, mPosition, context, mBuffer);
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
        int distCompressed = mSpace*mSpace;
        int startCompressed = headIndex - (headIndex - 1) % distCompressed;
        if (index - startCompressed < distCompressed) {
          mCompressed.back().mLast = headIndex;
        } else {
          mCompressed.push_back(mSecond.mSign);
        }
      }
      mSecond.mSign = mFirst.mSign;
      std::shared_ptr<T> context = std::make_shared<T>(*mContext);
      Signature<T,D> sign (index, mPosition, context, mBuffer);
      mFirst.mSign = sign;
      mSecond.mExplicit = mFirst.mExplicit;
      mFirst.clearExplicit(mSpace);
      mFirst.pushExplicit(eltPtr);
    }
  }
}

// Function push for the part. and fully compressed members of the stack
template <class T, class D>
void CompressedStack<T,D>::pushCompressed(SPData<T,D> elt, int lvl, int headIndex){
  int distSubBlock = std::pow(mSpace,(mDepth - lvl - 1));
  int distBlock = distSubBlock * mSpace;
  int index = elt->mIndex;
  Signature<T,D> sign (index, mPosition, mFirst.mSign.mContext, mBuffer);

  if (mFirst.mPartial[lvl].empty()) {
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
      if (lvl == 0 && (!mSecond.mPartial[0].empty())) {
        int distCompressed = std::pow(mSpace, mDepth + 1);
        int startCompressed = headIndex - (headIndex - 1) % distCompressed;
        int gamma = index - startCompressed;
        if (gamma < distCompressed && !mCompressed.empty()) {
          mCompressed.back().mLast = mSecond.mPartial[0].back().mLast;
        } else {
          mCompressed.push_back(Signature<T,D>(mSecond.mPartial[0]));
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
void CompressedStack<T,D>::reconstruct(Problem<T,D> &problem){
  for (int i = 1; i <= mDepth; i++) {
    int lvl = mDepth - 1 - i; // lvl == -1 is for the fully compressed part
    if (lvl < 0) {
      Signature<T,D> sign = Signature<T,D>(mCompressed.back());
      reconstruct(problem, sign, lvl + 1);
      break;
    } else {
      if (!mFirst.mPartial[lvl].empty()) {
        Signature<T,D> sign = Signature<T,D>(mFirst.mPartial[lvl].back());
        reconstruct(problem, sign, lvl + 1);
        break;
      } else if (!mSecond.mPartial[lvl].empty()) {
        Signature<T,D> sign = Signature<T,D>(mSecond.mPartial[lvl].back());
        reconstruct(problem, sign, lvl + 1);
        break;
      }
    }
  }

}

// Reconstruct an auxiliary stack based on the signature found above
template <class T, class D>
void CompressedStack<T,D>::reconstruct(Problem<T,D> &problem, const Signature<T,D> &sign, int lvl){
  std::streampos posReminder = problem.mInput.tellg();
  int indexReminder = problem.mIndex;

  problem.mContext = std::make_shared<T>(*sign.mContext);
  problem.mIndex = sign.mFirst - 1;
  problem.mInput.seekg(sign.mPosition);

  int auxSize = std::pow(mSpace, mDepth - lvl + 1);
  std::shared_ptr<Stack<T,D>> auxStack = std::make_shared<CompressedStack<T,D>>(auxSize, mSpace, mBuffer.mSize, problem.mContext, sign.mPosition);

  // Swap stack pointers + run  + swap back
  swap(problem.mStack, auxStack);
  problem.run(sign.mLast);
  swap(problem.mStack, auxStack);

  if (lvl == 0) {
    (*auxStack).copyContent(*this);
  } else {
    // Copy the First component of the reconstructed stack into the main stack
    int auxDepth = int( ceil(log(auxSize)/log(mSpace)-.001)) - 1;
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
}

template <class T, class D>
void CompressedStack<T,D>::copyContent(CompressedStack<T,D> &stack) {
  stack.mFirst = mFirst;
  stack.mSecond = mSecond;
  stack.mCompressed = mCompressed;
  stack.mBuffer = mBuffer;
}

/*==============================================================================
  Pop Functions: pop, popBuffer, popFirst, popSecond, emptyFirst, emptySecond, propagateFirst, propagateSecond, smartIndex
==============================================================================*/
template <class T, class D>
int CompressedStack<T,D>::smartIndex(int index){
  if (mBuffer.mSize > 0) {
    return mBuffer.top(1).mIndex;
  }
  if (!mFirst.mExplicit.empty()) {
    return (*mFirst.mExplicit.back()).mIndex;
  }
  if (!mSecond.mExplicit.empty()) {
    return (*mSecond.mExplicit.back()).mIndex;
  }

  for (int lvl = mDepth - 2; lvl >= 0; lvl--) {
    if (!mFirst.single(lvl)) {

    }
    if (mFirst.mPartial[lvl].size() + mSecond.mPartial[lvl].size() > 1) {
      return getTop2Partial(lvl).mLast;
    }
  }

  if (!mCompressed.empty()) {
    return mCompressed.back().mLast;
  }
  return 0;
}

template <class T, class D>
void CompressedStack<T,D>::popFirst(int oldIndex){
  // Pop the explicit data from the 1st component
  mFirst.mExplicit.pop_back();
  int index = smartIndex(oldIndex);
  if (mFirst.mExplicit.empty()) {
    Signature<T,D> sign(0, std::streampos (0), std::shared_ptr<T>(nullptr), Buffer<T,D>(0));
    mFirst.mSign = sign;
  } else {
    int temp = std::min(mFirst.mSign.mLast, index);
    mFirst.mSign.mLast = temp;
  }
  if (mDepth > 1) {
    emptyFirst(index, mDepth - 2);
  }
}

template <class T, class D>
void CompressedStack<T,D>::popSecond(int oldIndex){
  // Pop the explicit data from the 2nd component
  mSecond.mExplicit.pop_back();

  int index = smartIndex(oldIndex);
  if (mSecond.mExplicit.empty()) {
    Signature<T,D> sign(0, std::streampos (0), std::shared_ptr<T>(nullptr), Buffer<T,D>(0));
    mSecond.mSign = sign;
  } else {
    int temp = std::min(mSecond.mSign.mLast, index);
    mSecond.mSign.mLast = temp;
  }

  if (mDepth > 1) {
    emptySecond(index, mDepth - 2);
  }
}

template <class T, class D>
void CompressedStack<T,D>::propagateFirst(int index, int lvl){
  for (int i = 0; i <= lvl; i++) {
    int temp = std::min(mFirst.mPartial[i].back().mLast, index);
    mFirst.mPartial[i].back().mLast = temp;
  }
}

template <class T, class D>
void CompressedStack<T,D>::propagateSecond(int index, int lvl){
  if (mFirst.mPartial[lvl].empty()) {
    int temp = std::min(mSecond.mPartial[lvl].back().mLast, index);
    mSecond.mPartial[lvl].back().mLast = temp;
    if (lvl > 0) {
      propagateSecond(index, lvl - 1);
    }
  } else {
    propagateFirst(index, lvl);
  }
}

template <class T, class D>
void CompressedStack<T,D>::emptyFirst(int index, int lvl){
  if (mFirst.single(lvl)) {
    mFirst.mPartial[lvl].pop_back();
    if (lvl > 0) {
        emptyFirst(index, lvl - 1);
    } else if (mFirst.isempty(0)) {
      Signature<T,D> sign(0, std::streampos (0), std::shared_ptr<T>(nullptr), Buffer<T,D>(0));
      mFirst.mSign = sign;
    }
  } else {
    propagateFirst(index, lvl);
  }
}

template <class T, class D>
void CompressedStack<T,D>::emptySecond(int index, int lvl){
  if (mFirst.single(lvl) || !mFirst.mPartial[lvl].empty()) {
    emptyFirst(index, lvl);
  } else {
    if (mSecond.single(lvl)) {
      mSecond.mPartial[lvl].pop_back();
      if (lvl > 0) {
        emptySecond(index, lvl - 1);
      } else if (mSecond.isempty(0)) {
        Signature<T,D> sign(0, std::streampos (0), std::shared_ptr<T>(nullptr), Buffer<T,D>(0));
        mSecond.mSign = sign;
      }
    } else {
      propagateSecond(index, lvl);
    }
  }
}

template <class T, class D>
void CompressedStack<T,D>::popBuffer(){
  if (mBuffer.mSize > 0) {
    SPData<T,D> elt = getExplicitData(mBuffer.mSize);
    mBuffer.pop(elt);
  }
}

template <class T, class D>
Data<T,D> CompressedStack<T,D>::pop(Problem<T,D> &problem){
  popBuffer();
  SPData<T,D> elt;
  if (mFirst.mExplicit.empty() && mSecond.mExplicit.empty()) {
    // Reconstruct the compressed stack with the first available signature
    reconstruct(problem);
  }
  if (mFirst.mExplicit.empty()) {
    if (mSecond.mExplicit.empty()) {
      problem.println();
//      exit(0);
    }
    elt = mSecond.mExplicit.back();
    popSecond((*elt).mIndex);
  } else {
    elt = mFirst.mExplicit.back();
    popFirst((*elt).mIndex);
  }
  return *elt;
}

template <class T, class D>
Data<T,D> CompressedStack<T,D>::top(int k){
  if (k == 0) {
    return top();
  }
  return mBuffer.top(k);
}

template <class T, class D>
Data<T,D> CompressedStack<T,D>::top(){
  if (mFirst.isExplicitEmpty()) {
    return mSecond.top();
  }
  return mFirst.top();
}
// Top index of a Compressed Stack
template <class T, class D>
int CompressedStack<T,D>::topIndex(){
  if (mFirst.mExplicit.empty()) {
    if (mSecond.mExplicit.empty()) {
      if (!mCompressed.empty()) {
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
