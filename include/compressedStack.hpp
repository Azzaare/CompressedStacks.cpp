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

  void push(const Data<T,D> &data);
  // Push Internals
  void pushExplicit(SPData<T,D> elt);
  void pushCompressed(SPData<T,D> elt, int lvl);
  Data<T,D> top();
  int topIndex();
  void compress();
  void resetBlock(Signature<T,D> sign, int lvl);

  Data<T,D> pop(Problem<T,D> &Problem);
  // Pop Internals
  void popBuffer();
  void reconstruct(Problem<T,D> &problem);
  void reconstruct(Problem<T,D> &problem,const Signature<T,D> &sign, int lvl);
  void popFirst(int index);
  void popSecond(int index);
  void propagateFirst(int index, int lvl);
  void propagateSecond(int index, int lvl);
  void emptyFirst(int index, int lvl);
  void emptySecond(int index, int lvl);
  int smartIndex(int index);

  // Setters
  void setContext(std::shared_ptr<T> context);
  void setPosition(std::streampos position);

  // Getters
  Block<T,D> getFirstPartial(int lvl);
  Block<T,D> getCompressed();
  ExplicitPointer<T,D> getFirstExplicit();
  SPData<T,D> getExplicitData(int k);

  // IO
  std::string toString();

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

template <class T, class D>
void CompressedStack<T,D>::setPosition(std::streampos position){
  mPosition = position;
}

/*==============================================================================
  Getters : getFirstPartial, getCompressed, getFirstExplicit, getExplicitData
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
  return (mFirst.isempty() && mSecond.isempty() && mCompressed.empty());
}

/*==============================================================================
  Push Functions: resetBlock, push, pushExplicit, pushCompressed
==============================================================================*/

template <class T, class D>
void CompressedStack<T,D>::resetBlock(Signature<T,D> sign, int lvl){
  mFirst.mPartial[lvl].clear();
  mFirst.mPartial[lvl].reserve(std::pow(mSpace, mDepth + 1 - lvl));
  mFirst.mPartial[lvl].push_back(sign);
}

// Function push that push the data in explicit and index in partial/compressed
template <class T, class D>
void CompressedStack<T,D>::push(const Data<T,D> &elt){
  // update the buffer (if buffer size is bigger than 0)
  SPData<T,D> ptr_elt = std::make_shared<Data<T,D>>(elt);
  mBuffer.push(ptr_elt);
  // update the explicit Blocks, with possibly shifting first to second
  pushExplicit(ptr_elt);
  // update the compressed Blocks at each levels (including fully compressed)
  for (int lvl = 0; lvl < mDepth - 1; lvl++) {
    pushCompressed(ptr_elt, lvl);
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
        if (mCompressed.empty()) {
          mCompressed.push_back(mSecond.mSign);
        } else {
          // Compress mSecond into mCompressed
          (mCompressed.back()).mLast = mSecond.mSign.mLast;
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
void CompressedStack<T,D>::pushCompressed(SPData<T,D> elt, int lvl){
  int distSubBlock = std::pow(mSpace,(mDepth - lvl - 1));
  int distBlock = distSubBlock * mSpace;
  int index = elt->mIndex;
  Signature<T,D> sign (index, mPosition, mFirst.mSign.mContext, mBuffer);

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
    if (mFirst.mPartial[lvl].empty()) {
      if (!mSecond.single(lvl)) {
        return (index - (index % (int) std::pow(mSpace, mDepth - lvl - 1)));
      } else {
        int size = mSecond.mPartial[lvl].size();
        if (size > 1) {
          return mSecond.mPartial[lvl][size-2].mLast;
        }
      }
    } else {
      if (!mFirst.single(lvl)) {
        return (index - (index % (int) std::pow(mSpace, mDepth - lvl - 1)));
      } else {
        int size = mFirst.mPartial[lvl].size();
        if (size > 1) {
          return mFirst.mPartial[lvl][size-2].mLast;
        }
      }
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
  if (mFirst.single(lvl)) {
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
  std::cout << "Context debug 1 -> " << *problem.mContext << std::endl;
  const std::streampos posReminder = problem.mInput.tellg();
  int indexReminder = problem.mIndex;

  T contextP = *problem.mContext;
  T contextCS = *mContext;
  T contextSign = *sign.mContext;

  std::cout << contextP << " " << contextCS << " " << contextSign << std::endl;

  problem.mContext = std::make_shared<T>(*sign.mContext);

  std::cout << "Context debug 2 -> " << *problem.mContext << std::endl;

  problem.mIndex = sign.mFirst - 1;
  problem.mInput.seekg(sign.mPosition);
  int auxSize = std::pow(mSpace, mDepth - lvl);
  std::shared_ptr<Stack<T,D>> auxStack = std::make_shared<CompressedStack<T,D>>(auxSize, mSpace, mBuffer.mSize, problem.mContext, sign.mPosition);
  std::cout << "Context debug 3 -> " << *problem.mContext << std::endl;


  // Swap stack pointers + run  + swap back
  swap(problem.mStack, auxStack);
  std::cout << "Reconstructing from " << sign.mFirst << " to " << sign.mLast << std::endl;
  problem.run(sign.mLast - problem.mIndex);
  swap(problem.mStack, auxStack);
  problem.mIndex = indexReminder;
  problem.mInput.seekg(posReminder);
  problem.mContext = mContext;
//  mContext = problem.mContext;
  std::cout << "Context debug 4 -> " << *problem.mContext << std::endl;

  // Copy the First component of the reconstructed stack into the main stack
  int auxDepth = int( ceil(log(auxSize)/log(mSpace)-.1));
  int delta = mDepth - auxDepth;
  for (int i = 0; i < auxDepth - 1; i++) {
    mSecond.mPartial[delta + i] = (*auxStack).getFirstPartial(i);
  }
  std::cout << "Context debug 5 -> " << *problem.mContext << std::endl;

  problem.println();
  if (lvl == 0) {
    if (!(*auxStack).isSecondEmpty()) {
      (*auxStack).compress();
    }
    mCompressed = (*auxStack).getCompressed();
  }

  std::cout << "Context debug 6 -> " << *problem.mContext << std::endl;
  mSecond.mExplicit = (*auxStack).getFirstExplicit();
  std::cout << "Context debug 7 -> " << *problem.mContext << std::endl;
  problem.println();
  std::cout << (*auxStack).toString() << std::endl;
  std::cout << "Context debug 8 -> " << *problem.mContext << std::endl;
  if (delta > 0) {
    propagateSecond((*mSecond.mExplicit.back()).mIndex, delta - 1);
  }
  std::cout << "Context debug 9 -> " << *problem.mContext << std::endl;
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
  if (mFirst.mExplicit.empty()) {
    int count = 0;
    while (mSecond.mExplicit.empty() && (count < 10)) {
      count++;
      // Reconstruct the compressed stack with the first available signature
      std::cout << "Before reconstruct :" << *problem.mContext << std::endl;
      problem.println();
      reconstruct(problem);
      std::cout << "After reconstruct : " << *problem.mContext << std::endl;
      problem.println();
    }
    std::cout << "Debug pop 1 :" << *problem.mContext << std::endl;
    elt = mSecond.mExplicit.back();
    std::cout << "Debug pop 2 :" << *problem.mContext << std::endl;
    popSecond((*elt).mIndex);
    std::cout << "Debug pop 3:" << *problem.mContext << std::endl;
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
      return mCompressed.back().mLast;
    }
    return (*(mSecond.mExplicit.back())).mIndex;
  }
  return (*(mFirst.mExplicit.back())).mIndex;
}

#endif /* COMPRESSEDSTACK */
