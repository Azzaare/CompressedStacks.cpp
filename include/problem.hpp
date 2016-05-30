#ifndef PROBLEM
#define PROBLEM

/**** Problem : declaration ****/
#include "stack.hpp"
#include "compressedStack.hpp"
#include "normalStack.hpp"
#include <stdbool.h>
#include <fstream>
#include <string>

template <class T, class D>
class Problem
{
public:
  // Problem<T,D>(std::string fileName);
  Problem<T,D>(std::string fileName, int size);
  Problem<T,D>(std::string fileName, int size, int space);

  // Running the stack
  void run();

  // Setters
  void setInitStack(void (*functionPointer)());
  void setReadInput(D (*functionPointer)());
  void setPopCondition(bool (*functionPointer)());
  void setPopAction(void (*functionPointer)());
  void setPushCondition(bool (*functionPointer)());
  void setPushAction(void (*functionPointer)());

private:
  // Input/Ouput
  std::ifstream* mInput;
  std::ofstream* mOutput; // output file is optional

  // Stack Functions: defined by user
  void (*mInitStack)();
  D    (*mReadInput)();
  bool (*mPopCondition)();
  void (*mPopAction)();
  bool (*mPushCondition)();
  void (*mPushAction)();

  // Problem internal during run
  T* mContext;
  int mIndex;

  // Stack: Normal or Compressed
  Stack<D>* mStack;
};

/** Constructors **/
template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size)
{
  std::ifstream ifstr;
  ifstr.open(fileName, std::ifstream::in);
  mInput = &ifstr;
  mOutput = nullptr;

  mInitStack = nullptr;
  mReadInput = nullptr;
  mPopCondition = nullptr;
  mPopAction = nullptr;
  mPushCondition = nullptr;
  mPushAction = nullptr;

  mContext = nullptr;
  mIndex = 0;

  mStack = new NormalStack<T> (size);
}

template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size, int space)
{
  std::ifstream ifstr;
  ifstr.open(fileName, std::ifstream::in);
  mInput = ifstr;
  mOutput = nullptr;

  mInitStack = nullptr;
  mReadInput = nullptr;
  mPopCondition = nullptr;
  mPopAction = nullptr;
  mPushCondition = nullptr;
  mPushAction = nullptr;

  mContext = nullptr;
  mIndex = 0;

  mStack = new CompressedStack<T,D> (size, space);
}

/** Running the stack **/
template <class T, class D>
void Problem<T,D>::run() {
  while (true) {
    D elt = mInput();
    while (true && mPopConditon(elt)) {
      (*mStack).pop();
    }
    if (mPushCondition()) {
      (*mStack).push(elt);
    }
  }
}

#endif /* PROBLEM */
