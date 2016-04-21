/**** Problem : declaration ****/
#include "stack.hpp"
#include <stdbool.h>
#include <fstream>
#include <string>

template <class T, class D>
class Problem
{
public:
  Problem<T,D>(std::string fileName);

  // Functions of the problem
  //void init();
  //D read();

private:
  // Input/Ouput
  std::ifstream mInput;
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
  Stack<D> mStack;
};
