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
  // Internals
  Data<T,D> top(int k);
  void push(const Data<T,D> &data, std::streampos position);
  Data<T,D> pop(Problem<T,D> &Problem);
  bool isempty();

  bool isSecondEmpty();

  // Setters
  void setContext(std::shared_ptr<T> context);

  // Getters
  Block<T,D> getFirstPartial(int lvl);
  Block<T,D> getCompressed();
  ExplicitPointer<T,D> getFirstExplicit();
  SPData<T,D> getExplicitData(int k);

  // IO
  std::string toString();

  // Push Internals
  void pushExplicit(SPData<T,D> elt);
  void pushCompressed(SPData<T,D> elt, int lvl, std::streampos position);
  Data<T,D> top();
  int topIndex();
  void compress();
  void resetBlock(Signature<T,D> sign, int lvl);

  // Pop Internals
  void popBuffer();
  void reconstruct(Problem<T,D> &problem);
  void reconstruct(Problem<T,D> &problem,const Signature<T,D> &sign, int lvl);
  void popFirst();
  void popSecond();
  void propagateFirst(int index, int lvl);
  void propagateSecond(int index, int lvl);
  void emptyFirst(int index, int lvl);
  void emptySecond(int index, int lvl);

  // Structure constraints
  int mSize;  // (Expected) size of the input in #elements
  int mSpace; // Maximum space order of the compressed stack
  int mDepth; // Depth (#levels of compression) based on size and space

  // Position of previous input (before reading)
  std::streampos mPosition;

  // First, Second, and Compressed components
  Component<T,D> mFirst;
  Component<T,D> mSecond;
  Block<T,D> mCompressed;

  // Buffer to read the top k elements
  Buffer<T,D> mBuffer;

  // Pointer to the context in Problem
  std::shared_ptr<T> mContext;
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D>
CompressedStack<T,D>::CompressedStack(int size, int space, int buffer, std::shared_ptr<T> context, std::streampos position)
: mFirst(space, int( ceil(log(size)/log(space)-.1)))
, mSecond(space, int( ceil(log(size)/log(space)-.1)))
, mBuffer(buffer){
  mSize = size;
  mSpace = space;
  mDepth = int( ceil(log(size)/log(space)-.1));
  mPosition = position;
  mCompressed = initBlock<T,D>(mSpace);
  mContext = context;
}

template <class T, class D>
CompressedStack<T,D>::CompressedStack(int size, int space, int buffer, const Signature<T,D> &sign)
: mFirst(space, int( ceil(log(size)/log(space)-.1)))
, mSecond(space, int( ceil(log(size)/log(space)-.1)))
, mBuffer(buffer){
  mSize = size;
  mSpace = space;
  mDepth = int( ceil(log(size)/log(space)-.1));
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

/*==============================================================================
  Getters : getFirstPartial
==============================================================================*/
template <class T, class D>
Block<T,D> CompressedStack<T,D>::getFirstPartial(int lvl){
  return mFirst.mPartial[lvl];
}

template <class T, class D>
Block<T,D> CompressedStack<T,D>::getCompressed(){
  return mCompressed;
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
  str += "\t\tFully Compressed\n";
  str += blockToString(mCompressed);
  str += mBuffer.toString();
  return str;
}

/*==============================================================================

==============================================================================*/

/*==============================================================================
  Stack Functions: push, pop, isempty, compress
==============================================================================*/
template <class T, class D>
bool CompressedStack<T,D>::isSecondEmpty(){
  return mSecond.mPartial.empty();
}

// Function that compress the top block of mSecond to a sign in mCompressed
template <class T, class D>
void CompressedStack<T,D>::compress(){
  if (!mSecond.isempty()) {
    mCompressed.back().mLast = mSecond.topIndex();
  }
}

template <class T, class D>
bool CompressedStack<T,D>::isempty(){
  return (mFirst.isempty() && mSecond.isempty());
}

template <class T, class D>
void CompressedStack<T,D>::resetBlock(Signature<T,D> sign, int lvl){
  mFirst.mPartial[lvl].clear();
  mFirst.mPartial[lvl].reserve(std::pow(mSpace, mDepth + 1 - lvl));
  mFirst.mPartial[lvl].push_back(sign);
}

// Function push that push the data in explicit and index in partial/compressed
template <class T, class D>
void CompressedStack<T,D>::push(const Data<T,D> &elt, std::streampos position){
  // update the buffer (if buffer size is bigger than 0)
  SPData<T,D> ptr_elt = std::make_shared<Data<T,D>>(elt);
  mBuffer.push(ptr_elt);
  // update the explicit Blocks, with possibly shifting first to second
  pushExplicit(ptr_elt);
  // update the compressed Blocks at each levels (including fully compressed)
  for (int lvl = 0; lvl < mDepth - 1; lvl++) {
    std::cout << "Pushing on level " << lvl << std::endl;
    pushCompressed(ptr_elt, lvl, position);
  }
}

// Function push for the Explicit members of the stack
template <class T, class D>
void CompressedStack<T,D>::pushExplicit(SPData<T,D> elt){
  int index = elt->mIndex;
  SPData<T,D> eltPtr = elt;
  Signature<T,D> sign (index, mPosition, mContext, mBuffer);

  // If the explicit datas of component 1 are empty we push
  if (mFirst.isExplicitEmpty()) {
    mFirst.pushExplicit(eltPtr);
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
        if (mCompressed.empty()) {
          mCompressed.push_back(mSecond.mSign);
        } else {
          // Compress mSecond into mCompressed
          (mCompressed.back()).mLast = mSecond.mSign.mLast;
        }
      }
      mSecond.mSign = sign;
      mFirst.mSign = sign;
      mSecond.mExplicit = mFirst.mExplicit;
      mFirst.clearExplicit(mSpace);
      mFirst.pushExplicit(eltPtr);
    }
  }
}

// Function push for the part. and fully compressed members of the stack
template <class T, class D>
void CompressedStack<T,D>::pushCompressed(SPData<T,D> elt, int lvl, std::streampos position){
  int distSubBlock = std::pow(mSpace,(mDepth - lvl));
  int distBlock = distSubBlock * mSpace;
  int index = elt->mIndex;
  Signature<T,D> sign (index, position, mContext, mBuffer);

  if (mFirst.isempty(lvl)) {
    mFirst.push(sign, lvl);
  } else {
    int headIndex = mFirst.topIndex(lvl);
    int startBlock = headIndex - (headIndex - 1) % distBlock;
    // distance of the new index and current block
    int delta = index - startBlock + 1;
    if (delta <= distBlock) {
      // Distance with the current subblock
      int startSubBlock = headIndex - (headIndex - 1) % distSubBlock;
      int eta = index - startSubBlock + 1;
      // compress new element in the top of the current Block
      if (eta <= distSubBlock) {
        mFirst.mPartial[lvl].back().mLast = index;
      } else {
        mFirst.push(sign, lvl);
      }
    } else {
      if (lvl == 0) {
        int distComponent = int(ceil(mSize/mSpace));
        int startComponent = headIndex - (headIndex - 1) % distComponent;
        int gamma = index - startComponent + 1;
        if (gamma <= distComponent) {
          compress();
        } else {
          mCompressed.push_back(Signature<T,D>(mSecond.mPartial[1]));
        }
      }
      mSecond.mPartial[lvl] = mFirst.mPartial[lvl];
      resetBlock(sign, lvl);
    }
  }
}

template <class T, class D>
void CompressedStack<T,D>::popFirst(){
  // Pop the explicit data from the 1st component
  mFirst.mExplicit.pop_back();

  int index = topIndex();
  if (mDepth > 1) {
    if (mFirst.mExplicit.empty()) {
      emptyFirst(index, mDepth - 1);
    } else {
      int newIndex = (*(mFirst.mExplicit.back())).mIndex;
      propagateFirst(newIndex, mDepth - 1);
    }
  }
}

template <class T, class D>
void CompressedStack<T,D>::popSecond(){
  // Pop the explicit data from the 2nd component
  mSecond.mExplicit.pop_back();

  int index = topIndex();
  if (mDepth > 1) {
    if (mSecond.mExplicit.empty()) {
      emptySecond(index, mDepth - 1);
    } else {
      int newIndex = (*(mSecond.mExplicit.back())).mIndex;
      propagateSecond(newIndex, mDepth - 1);
    }
  }
}

template <class T, class D>
void CompressedStack<T,D>::propagateFirst(int index, int lvl){
  for (int i = 0; i < lvl; i++) {
    mFirst.mPartial[i].back().mLast = index;
  }
}

template <class T, class D>
void CompressedStack<T,D>::propagateSecond(int index, int lvl){
  if (mFirst.mPartial.empty()) {
    mSecond.mPartial[lvl].back().mLast = index;
    if (lvl > 0) {
      propagateSecond(index, lvl - 1);
    }
  } else {
    propagateFirst(index, lvl);
  }
}

template <class T, class D>
void CompressedStack<T,D>::emptyFirst(int index, int lvl){
  mFirst.mPartial[lvl].pop_back();
  if (lvl > 0) {
    if (mFirst.mPartial[lvl].size() == 0) {
      emptyFirst(index, lvl - 1);
    } else {
      int newIndex = mFirst.mPartial[lvl].back().mLast;
      propagateFirst(newIndex, lvl - 1);
    }
  }
}

template <class T, class D>
void CompressedStack<T,D>::emptySecond(int index, int lvl){
  if (mFirst.mPartial[lvl].empty()) {
    mSecond.mPartial[lvl].pop_back();
    if (lvl > 0) {
      if (mSecond.mPartial[lvl].size() == 0) {
        emptySecond(index, lvl - 1);
      } else {
        int newIndex = mSecond.mPartial[lvl].back().mLast;
        propagateSecond(newIndex, lvl - 1);
      }
    }
  } else {
    emptyFirst(index, lvl);
  }
}

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
      if (mFirst.mPartial[lvl].empty()) {
        Signature<T,D> sign = Signature<T,D>(mFirst.mPartial[lvl].back());
        reconstruct(problem, sign, lvl + 1);
        break;
      } else if (mSecond.mPartial[lvl].empty()) {
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
  std::streampos posReminder = mPosition;
  int indexReminder = problem.mIndex;

  problem.mIndex = sign.mFirst;
  int auxSize = std::pow(mSpace, mDepth - lvl);
  std::shared_ptr<Stack<T,D>> auxStack = std::make_shared<CompressedStack<T,D>>(auxSize, mSpace, mBuffer.mSize, sign);

  // Swap stack pointers + run  + swap back
  swap(problem.mStack, auxStack);
  problem.run(sign.mLast - sign.mFirst);
  swap(problem.mStack, auxStack);
  problem.mIndex = indexReminder;
  mPosition = posReminder;

  // Copy the First component of the reconstructed stack into the main stack
  int auxDepth = int( ceil(log(auxSize)/log(mSpace)-.1));
  int delta = mDepth - auxDepth;
  for (int i = 0; i < auxDepth; i++) {
    mSecond.mPartial[delta + i] = (*auxStack).getFirstPartial(i);
  }
  if (lvl == 0) {
    if (!(*auxStack).isSecondEmpty()) {
      (*auxStack).compress();
    }
    mCompressed = (*auxStack).getCompressed();
  }
  mSecond.mExplicit = (*auxStack).getFirstExplicit();
}

template <class T, class D>
void CompressedStack<T,D>::popBuffer(){
  SPData<T,D> elt = getExplicitData(mBuffer.mSize);
  mBuffer.pop(elt);
}

template <class T, class D>
Data<T,D> CompressedStack<T,D>::pop(Problem<T,D> &problem){
  popBuffer();
  SPData<T,D> elt;
  if (mFirst.mExplicit.empty()) {
    if (mSecond.mExplicit.empty()) {
      // Reconstruct the compressed stack with the first available signature
      reconstruct(problem);
    }
    elt = mSecond.mExplicit.back();
    popSecond();
  } else {
    elt = mFirst.mExplicit.back();
    popFirst();
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
      return mCompressed.back().mLast;
    }
    return (*(mSecond.mExplicit.back())).mIndex;
  }
  return (*(mFirst.mExplicit.back())).mIndex;
}

#endif /* COMPRESSEDSTACK */
