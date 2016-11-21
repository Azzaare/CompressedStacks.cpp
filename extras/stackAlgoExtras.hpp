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
  void runCompare(I buffer = 0);

  // IO
  void printCompare();

  // Walking the stack
  void walk(I steps = 1);
  void walkCompare(I steps = 1);

  // Redefinition for comparison
  void readPushCompare(I iter = 1);

private:
  // Stack Functions: defined by user
  virtual D readInput(std::vector<std::string> line) = 0;
  virtual std::shared_ptr<T> initStack() = 0;
  virtual bool popCondition(D data) = 0;
  virtual void popAction(Data<T, D, I> elt){};
  virtual bool pushCondition(D data) = 0;
  virtual void pushAction(Data<T, D, I> elt){};

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

  std::cout << "Debug compress = " << usecompressed
            << ", classic = " << useclassic << std::endl;

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
void StackAlgoExtras<T, D, I>::walk(I steps) {
  for (I i = 0; i < steps; i++) {
    if (StackAlgo<T, D, I>::step() == 0 || StackAlgo<T, D, I>::mInput.good()) {
      break;
    }
  }
}

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::printCompare() {
  std::string str = StackAlgo<T, D, I>::toString();
  str += "\n" + (*mClassicStack).toString();
  std::cout << str << std::endl;
}

template <class T, class D, class I>
Data<T, D, I> StackAlgoExtras<T, D, I>::popCompare() {
  return StackAlgo<T, D, I>::mStack->pop(*this);
}

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::runCompare(I buffer) {
  try {
    StackAlgo<T, D, I>::initStackIntern();
    while ((StackAlgo<T, D, I>::mInput.good())) {
      std::streampos position = StackAlgo<T, D, I>::mInput.tellg();
      (*StackAlgo<T, D, I>::mStack).setPosition(position);
      for (I i = 1; i <= buffer; i++) {
        bool bIndex =
            StackAlgo<T, D, I>::top(i).mIndex == mClassicStack->top(i).mIndex;
        bool bData =
            StackAlgo<T, D, I>::top(i).mData == mClassicStack->top(i).mData;
        if (!bIndex || !bData) {
          StackAlgo<T, D, I>::println();
          std::cout << mClassicStack->toString() << std::endl;
          throw "The top $(i)st elements are different";
        }
      }
      std::vector<std::string> line = StackAlgo<T, D, I>::readLine();
      if ((line.front() == "-1") || (line.front() == "")) {
        break;
      }
      D data = readInput(line);
      StackAlgo<T, D, I>::mIndex++;
      if ((*StackAlgo<T, D, I>::mStack).empty() != mClassicStack->empty()) {
        (*StackAlgo<T, D, I>::mStack).empty();
        StackAlgo<T, D, I>::println();
        std::cout << mClassicStack->toString() << std::endl;
        (*StackAlgo<T, D, I>::mStack).empty();
        throw "One stack is empty and not the other";
      }
      while ((!(StackAlgo<T, D, I>::emptystack())) && (popCondition(data))) {
        Data<T, D, I> elt = StackAlgo<T, D, I>::pop();
        Data<T, D, I> eltNormal = mClassicStack->pop();
        popAction(elt);
        bool bIndex = elt.mIndex == eltNormal.mIndex;
        bool bData = elt.mData == eltNormal.mData;
        if (!bIndex || !bData) {
          StackAlgo<T, D, I>::println();
          // std::cout << *StackAlgo<T, D, I>::mContext << std::endl;
          std::cout << mClassicStack->toString() << std::endl;
          throw "The two elements popped are different";
        }
      }
      if (pushCondition(data)) {
        Data<T, D, I> elt(StackAlgo<T, D, I>::mIndex, data);
        pushAction(elt);
        StackAlgo<T, D, I>::push(elt);
        mClassicStack->push(elt);
      }
      if (StackAlgo<T, D, I>::mStack->getBufferSize() > 0) {
        std::cout << "Is it working" << std::endl;
        for (I k = 1; k <= StackAlgo<T, D, I>::mStack->getBufferSize(); k++) {
          if (StackAlgo<T, D, I>::mStack->top(k).mIndex ==
              mClassicStack->top(k).mIndex) {
            StackAlgo<T, D, I>::println();
            // std::cout << *StackAlgo<T, D, I>::mContext << std::endl;
            std::cout << mClassicStack->toString() << std::endl;
            throw "The two elements at the k = $(k) position are different";
          }
        }
      }
    }
  } catch (char const *e) {
    std::cout << e << std::endl;
  }
}

template <class T, class D, class I>
void StackAlgoExtras<T, D, I>::readPushCompare(I iter) {
  for (I i = 0; i < iter; i++) {
    std::streampos position = StackAlgo<T, D, I>::mInput.tellg();
    StackAlgo<T, D, I>::mStack->setPosition(position);
    std::vector<std::string> line = StackAlgo<T, D, I>::readLine();
    D data = StackAlgo<T, D, I>::readInput(line);
    StackAlgo<T, D, I>::mIndex++;
    Data<T, D, I> elt(StackAlgo<T, D, I>::mIndex, data);
    pushAction(elt);
    StackAlgo<T, D, I>::push(elt);
    mClassicStack->push(elt);
  }
}

#endif /* SAEXTRAS */
