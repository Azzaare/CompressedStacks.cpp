#ifndef COMPRESSEDSTACK
#define COMPRESSEDSTACK

/**** Compressed Stack: declarations ****/
#include "sign.hpp"
#include "stack.hpp"
#include "component.hpp"
#include "data.hpp"
#include "buffer.hpp"
#include <string>
#include <cmath>
#include <memory>

/* Compressed Stack itself */
template <class T, class D>
class CompressedStack: public Stack<D>
{
public:
  CompressedStack<T,D>(int size, int space, int buffer, std::shared_ptr<T> context, std::streampos position);

  // Internals
  Data<D> top(int k);
  void push(Data<D> data);
  Data<D> pop();
  bool isempty();

  // IO
  std::string toString();
  void print();
  void println();

private:
  // Push Internals
  void pushExplicit(std::shared_ptr<Data<D>> elt);
  void pushCompressed(std::shared_ptr<Data<D>> elt, int lvl);
  Data<D> top();
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
  Block<T> mCompressed;

  // Buffer to read the top k elements
  Buffer<D> mBuffer;

  // Pointer to the context in Problem
  std::shared_ptr<T> mContext;
};

/** Constructors **/
template <class T, class D>
CompressedStack<T,D>::CompressedStack(int size, int space, int buffer, std::shared_ptr<T> context, std::streampos position)
: mFirst(size,space)
, mSecond(size,space)
, mBuffer(buffer)
{
  mSize = size;
  mSpace = space;
  mDepth = (int) ceil(log(size)/log(space)-.1); // - 1;

  mPosition = position;

  mCompressed = initBlock<T>(mSpace);

  mContext = context;
}

/** IO **/
template <class T, class D>
std::string CompressedStack<T,D>::toString()
{
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

template <class T, class D>
void CompressedStack<T,D>::print()
{
  std::cout << this->toString();
}

template <class T, class D>
void CompressedStack<T,D>::println()
{
  this->print();
  std::cout << std::endl;
}

/** Pushes, pops and accesses **/
template <class T, class D>
bool CompressedStack<T,D>::isempty(){
  return (mFirst.isempty() && mSecond.isempty());
}

// Function push that push the data in explicit and index in partial/compressed
template <class T, class D>
void CompressedStack<T,D>::push(Data<D> elt){
  // update the buffer (if buffer size is bigger than 0)
  std::shared_ptr<Data<D>> ptr_elt (new Data<D>(elt));
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
void CompressedStack<T,D>::pushExplicit(std::shared_ptr<Data<D>> elt){
  int index = elt->getIndex();
  std::shared_ptr<Data<D>> eltPtr = elt;
  Signature<T> sign (index, mPosition, mContext);

  // If the explicit datas of component 1 are empty we push
  if (mFirst.isExplicitEmpty()) {
    mFirst.pushExplicit(eltPtr);
    std::shared_ptr<Signature<T>> signPtr(&sign);
    mFirst.setSignature(signPtr);
  }
  // We check if thoses explicit datas are full
  else {
    int headIndex = mFirst.topIndex();
    int startBlock = headIndex - (headIndex - 1) % mSpace;
    if (index - startBlock < mSpace) {
      mFirst.pushExplicit(eltPtr);
      mFirst.setLastSign(index);
    } else {
      if ((mDepth == 1) && (!(mSecond.isExplicitEmpty()))) {
        if (mCompressed.empty()) {
          mCompressed.push_back(mSecond.getSign());
        } else {
          (mCompressed.back()).setLast(mSecond.getLastSign());
        }
      }
      std::shared_ptr<Signature<T>> signPtr = mFirst.getSignPtr();
      mSecond.setSignature(signPtr);
      mFirst.setSignature(signPtr);
      mSecond.setExplicit(mFirst.getExplicit());
      mFirst.clearExplicit(mSpace);
      mFirst.pushExplicit(eltPtr);
    }
  }
}

// Function push for the part. and fully compressed members of the stack
template <class T, class D>
void CompressedStack<T,D>::pushCompressed(std::shared_ptr<Data<D>> elt, int lvl){
  int distSubBlock = std::pow(mSpace,(mDepth - lvl));
  int distBlock = distSubBlock * mSpace;
  int index = elt->getIndex();

  if (mFirst.isempty(lvl)) {
    Signature<T> sign (index, mPosition, mContext);
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
Data<D> CompressedStack<T,D>::pop()
{
  Data<D> d (1,1);
  return d;
}

template <class T, class D>
Data<D> CompressedStack<T,D>::top(int k)
{
  if (k == 0) {
    return top();
  }
  return mBuffer.top(k);
}

template <class T, class D>
Data<D> CompressedStack<T,D>::top()
{
  if (mFirst.isExplicitEmpty()) {
    return mSecond.top();
  }
  return mFirst.top();
}
// Top index of a Compressed Stack
template <class T, class D>
int topIndex(){
  Data<D> elt = CompressedStack<T,D>::top();
  return elt.getIndex();
}

#endif /* COMPRESSEDSTACK */
