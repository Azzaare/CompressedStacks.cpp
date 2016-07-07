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

private:
  CompressedStack<T,D>(int size, int space, int buffer, std::shared_ptr<T> context, std::streampos position);

  // Internals
  Data<T,D> top(int k);
  void push(Data<T,D> data);
  Data<T,D> pop();
  bool isempty();

  // IO
  std::string toString();

  // Push Internals
  void pushExplicit(std::shared_ptr<Data<T,D>> elt);
  void pushCompressed(std::shared_ptr<Data<T,D>> elt, int lvl);
  Data<T,D> top();
  int topIndex();

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
:mFirst(size,space), mSecond(size,space), mBuffer(buffer){
  mSize = size;
  mSpace = space;
  mDepth = (int) ceil(log(size)/log(space)-.1); // - 1;

  mPosition = position;

  mCompressed = initBlock<T,D>(mSpace);

  mContext = context;
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
  Stack Functions: push, pop, isempty
==============================================================================*/
template <class T, class D>
bool CompressedStack<T,D>::isempty(){
  return (mFirst.isempty() && mSecond.isempty());
}

// Function push that push the data in explicit and index in partial/compressed
template <class T, class D>
void CompressedStack<T,D>::push(Data<T,D> elt){
  // update the buffer (if buffer size is bigger than 0)
  std::shared_ptr<Data<T,D>> ptr_elt (new Data<T,D>(elt));
  mBuffer.push(ptr_elt);
  // update the explicit Blocks, with possibly shifting first to second
  pushExplicit(ptr_elt);
  // update the compressed Blocks at each levels (including fully compressed)
  for (int lvl = 1; lvl < mDepth - 1; lvl++) {
    pushCompressed(ptr_elt, lvl);
  }
}

// Function push for the Explicit members of the stack
template <class T, class D>
void CompressedStack<T,D>::pushExplicit(std::shared_ptr<Data<T,D>> elt){
  int index = elt->mIndex;
  std::shared_ptr<Data<T,D>> eltPtr = elt;
  Signature<T,D> sign (index, mPosition, mContext);

  // If the explicit datas of component 1 are empty we push
  if (mFirst.isExplicitEmpty()) {
    mFirst.pushExplicit(eltPtr);
    std::shared_ptr<Signature<T,D>> signPtr(&sign);
    mFirst.mSign = signPtr;
  }
  // We check if thoses explicit datas are full
  else {
    int headIndex = mFirst.topIndex();
    int startBlock = headIndex - (headIndex - 1) % mSpace;
    if (index - startBlock < mSpace) {
      mFirst.pushExplicit(eltPtr);
      (*mFirst.mSign).mLast = index;
    } else {
      if ((mDepth == 1) && (!(mSecond.isExplicitEmpty()))) {
        if (mCompressed.empty()) {
          mCompressed.push_back(*mSecond.mSign);
        } else {
          // Compress mSecond into mCompressed
          (mCompressed.back()).mLast = (*mSecond.mSign).mLast;
        }
      }
      std::shared_ptr<Signature<T,D>> signPtr = mFirst.mSign;
      mSecond.mSign = signPtr;
      mFirst.mSign = signPtr;
      mSecond.mExplicit = mFirst.mExplicit;
      mFirst.clearExplicit(mSpace);
      mFirst.pushExplicit(eltPtr);
    }
  }
}

// Function push for the part. and fully compressed members of the stack
template <class T, class D>
void CompressedStack<T,D>::pushCompressed(std::shared_ptr<Data<T,D>> elt, int lvl){
  int distSubBlock = std::pow(mSpace,(mDepth - lvl));
  int distBlock = distSubBlock * mSpace;
  int index = elt->mIndex;

  if (mFirst.isempty(lvl)) {
    Signature<T,D> sign (index, mPosition, mContext);
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
        int lengthSubBlock = 0;
      } else {
        /* code */
      }
    } else {
      /* code */
    }
  }
}

template <class T, class D>
Data<T,D> CompressedStack<T,D>::pop(){
  Data<T,D> d (1,1);
  return d;
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
int topIndex(){
  Data<T,D> elt = CompressedStack<T,D>::top();
  return elt.getIndex();
}

#endif /* COMPRESSEDSTACK */
