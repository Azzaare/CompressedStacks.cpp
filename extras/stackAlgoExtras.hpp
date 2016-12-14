#ifndef SAEXTRAS
#define SAEXTRAS

/*==============================================================================
  Includes
==============================================================================*/
#include "../include/compressedStack.hpp"
#include "../include/stackAlgo.hpp"

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions : Instance
  Aliases    :
  Friends   -> CompressedStack
            <- ClassicStack, CompressedStack, Data
==============================================================================*/
template <class T, class D, class I>
class StackAlgoExtras : public StackAlgo<T, D, I> {
  friend class CompressedStack<T, D, I>;

public:
  // Constructor
  StackAlgoExtras<T, D, I>(std::string fileName, bool usecompressed = true,
                           bool useclassic = false);
  virtual ~StackAlgoExtras<T, D, I>() {}

  // Compare the stacks
  // void runCompare(I buffer = 0);
  void runCompare();

  // IO
  void printCompare();

  // Redefinition for comparison
  void readPushCompare(I iter = 1);

private:
  // Internal functions for runCompare
  void pushStepCompare(D data);
  void popLoopCompare(D data);
  I stepCompare();

  // Stack Functions : defined by user virtual std::shared_ptr<T> initStack() =
  // 0;
  virtual D readInput(std::vector<std::string> line) = 0;

  virtual bool popCondition(D data) = 0;
  virtual void prePop(D data) = 0;
  virtual void postPop(D data, Data<T, D, I> elt) = 0;
  virtual void noPop(D data) = 0;

  virtual bool pushCondition(D data) = 0;
  virtual void prePush(Data<T, D, I> elt) = 0;
  virtual void postPush(Data<T, D, I> elt) = 0;
  virtual void noPush(D data) = 0;

  virtual void reportStack() = 0;

  // pop
  Data<T, D, I> popCompare();

  // Stack: Normal Stack for comparison
  std::shared_ptr<ClassicStack<T, D, I>> mClassicStack;
};

/*==============================================================================
  Constructors :
==============================================================================*/
template <class T, class D, class I>
StackAlgoExtras<T, D, I>::StackAlgoExtras(std::string fileName,
                                          bool usecompressed, bool useclassic)
    : StackAlgo<T, D, I>(fileName, usecompressed) {

  if (usecompressed) {
    if (useclassic) {
      mClassicStack =
          std::shared_ptr<ClassicStack<T, D, I>>(new ClassicStack<T, D, I>());
    } else {
      mClassicStack = std::shared_ptr<ClassicStack<T, D, I>>(nullptr);
    }
  } else {
    if (useclassic) {
      StackAlgo<T, D, I>::mStack =
          std::shared_ptr<Stack<T, D, I>>(new ClassicStack<T, D, I>());
    } else {
      throw "A type of stack needs to be given ...";
    }
  }
}

/*==============================================================================
  Extras Functions: run, push, pop, top
==============================================================================*/

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::printCompare() {
  std::string str = StackAlgo<T, D, I>::toString();
  str += "\n" + (*mClassicStack).toString();
  std::cout << str << std::endl;
}

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::readPushCompare(I iter) {
  for (I i = 0; i < iter; i++) {
    std::streampos position = StackAlgo<T, D, I>::mInput.tellg();
    StackAlgo<T, D, I>::mStack->setPosition(position);
    std::vector<std::string> line = StackAlgo<T, D, I>::readLine();
    D data = readInput(line);
    StackAlgo<T, D, I>::mIndex++;
    Data<T, D, I> elt(StackAlgo<T, D, I>::mIndex, data);
    prePush(elt);
    StackAlgo<T, D, I>::push(elt);
    mClassicStack->push(elt);
    postPush(elt);
  }
}

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::pushStepCompare(D data) {
  if (pushCondition(data)) {
    Data<T, D, I> elt(StackAlgo<T, D, I>::mIndex, data);
    prePush(elt);
    StackAlgo<T, D, I>::push(elt);
    mClassicStack->push(elt);
    postPush(elt);
  } else {
    noPush(data);
  }
  if (StackAlgo<T, D, I>::mStack->getBufferSize() > 0) {
    for (I k = 1; k <= StackAlgo<T, D, I>::mStack->getBufferLength(); k++) {
      if (StackAlgo<T, D, I>::mStack->top(k).mIndex !=
          mClassicStack->top(k).mIndex) {
        StackAlgo<T, D, I>::println();
        throw "The two elements at the k = $(k) position are different";
      }
    }
  }
}

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::popLoopCompare(D data) {
  while (!StackAlgo<T, D, I>::emptystack()) {
    if (popCondition(data)) {
      prePop(data);
      Data<T, D, I> elt = StackAlgo<T, D, I>::pop();
      Data<T, D, I> eltNormal = mClassicStack->pop();
      postPop(data, elt);
      bool bIndex = elt.mIndex == eltNormal.mIndex;
      bool bData = elt.mData == eltNormal.mData;
      if (!bIndex || !bData) {
        StackAlgo<T, D, I>::println();
        throw "The two elements popped are different";
      }
    } else {
      noPop(data);
      break;
    }
  }
}

template <class T, class D, class I> I StackAlgoExtras<T, D, I>::stepCompare() {
  // Storing the position in the input file
  std::streampos position = StackAlgo<T, D, I>::mInput.tellg();
  (*StackAlgo<T, D, I>::mStack).setPosition(position);
  for (I i = 1; i <= StackAlgo<T, D, I>::mStack->getBufferSize(); i++) {
    bool bIndex =
        StackAlgo<T, D, I>::top(i).mIndex == mClassicStack->top(i).mIndex;
    bool bData =
        StackAlgo<T, D, I>::top(i).mData == mClassicStack->top(i).mData;
    if (!bIndex || !bData) {
      StackAlgo<T, D, I>::println();
      throw "The top $(i)st elements are different";
    }
  }

  // Reading a new element
  std::vector<std::string> line = StackAlgo<T, D, I>::readLine();
  if (line.front() == "") {
    // Return 0 if it has to stop (EOF)
    return 0;
  }
  D data = readInput(line);

  // Increasing index of the number of elements
  StackAlgo<T, D, I>::mIndex++;
  if ((*StackAlgo<T, D, I>::mStack).empty() != mClassicStack->empty()) {
    (*StackAlgo<T, D, I>::mStack).empty();
    (*StackAlgo<T, D, I>::mStack).empty();
    throw "One stack is empty and not the other";
  }

  // Call the pop loop
  popLoopCompare(data);

  // Call the conditional push
  pushStepCompare(data);

  // Return 1 in case of success
  return 1;
}

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::runCompare() {
  try {
    StackAlgo<T, D, I>::initStackIntern();

    while (StackAlgo<T, D, I>::mInput.good()) {
      if (stepCompare() == 0) {
        break;
      }
    }

    reportStack();
  } catch (char const *e) {
    std::cout << e << std::endl;
    StackAlgo<T, D, I>::println();
    std::cout << mClassicStack->toString() << std::endl;
  }
}

#endif /* SAEXTRAS */
