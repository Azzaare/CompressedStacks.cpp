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
  void setReadInput(D (*functionPointer)());
  void setInitStack(void (*functionPointer)());
  void setPopCondition(bool (*functionPointer)());
  void setPopAction(void (*functionPointer)());
  void setPushCondition(bool (*functionPointer)());
  void setPushAction(void (*functionPointer)());

  void setOutput(std::string fileName);

  // IO
  std::string toString();
  void print();
  void println();

private:
  // Input/Ouput
  std::ifstream* mInput;
  std::ofstream* mOutput; // output file is optional

  // Stack Functions: defined by user
  D    (*mReadInput)();
  void (*mInitStack)();
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

  mStack = new CompressedStack<T,D> (size, space);
}

/** IO **/
template <class T, class D>
std::string Problem<T,D>::toString(){
  std::string str;
  str = "Problem with an actual index of " + std::to_string(mIndex);
  str += ", with a stack of type\n";
  str += (*mStack).toString();
  return str;
}

template <class T, class D>
void Problem<T,D>::print()
{
  std::cout << this->toString();
}

template <class T, class D>
void Problem<T,D>::println()
{
  this->print();
  std::cout << std::endl;
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

/** Setters **/
template <class T, class D>
void Problem<T,D>::setReadInput(D (*functionPointer)()){
  mReadInput = functionPointer;
}
template <class T, class D>
void Problem<T,D>::setInitStack(void (*functionPointer)()){
  mInitStack = functionPointer;
}
template <class T, class D>
void Problem<T,D>::setPopAction(void (*functionPointer)()){
  mPopAction = functionPointer;
}
template <class T, class D>
void Problem<T,D>::setPopCondition(bool (*functionPointer)()){
  mPopConditon = functionPointer;
}
template <class T, class D>
void Problem<T,D>::setPushAction(void (*functionPointer)()){
  mPushAction = functionPointer;
}
template <class T, class D>
void Problem<T,D>::setPushCondition(bool (*functionPointer)()){
  mPushConditon = functionPointer;
}

template <class T, class D>
void Problem<T,D>::setOutput(std::string fileName){
  std::ofstream ofstr;
  ofstr.open(fileName, std::ofstream::out);
  mOutput = &ofstr;
}

#endif /* PROBLEM */
