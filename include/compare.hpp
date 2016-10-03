#ifndef COMPARE
#define COMPARE

/*==============================================================================
  Includes
==============================================================================*/
#include "data.hpp"
#include "stackAlgo.hpp"

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions :
  Aliases    :
  Friends   ->
            <- StackAlgo
==============================================================================*/
template <class T, class D> class CompareStacks : public StackAlgo<T, D> {
public:
  // Members functions
  CompareStacks<T, D>(std::string fileName);

  // Compare the stacks
  void runCompare(int buffer = 0);

  // IO
  void printCompare() {
    std::string str = StackAlgo<T, D>::toString();
    str += "\n" + (*mClassicStack).toString();
    std::cout << str << std::endl;
  }

  // Redefinition
  void readPush(int iter = 1);

private:
  // Stack Functions: defined by user
  virtual D readInput(std::vector<std::string> line) = 0;
  virtual std::shared_ptr<T> initStack() = 0;
  virtual bool popCondition(D data) = 0;
  virtual void popAction(Data<T, D> elt){};
  virtual bool pushCondition(D data) = 0;
  virtual void pushAction(Data<T, D> elt){};

  // pop
  Data<T, D> popCompare();

  // Stack: Normal Stack for comparison
  std::shared_ptr<ClassicStack<T, D>> mClassicStack;
};

/*==============================================================================
  Constructors :
==============================================================================*/

template <class T, class D>
CompareStacks<T, D>::CompareStacks(std::string fileName)
    : StackAlgo<T, D>(fileName), mClassicStack(new ClassicStack<T, D>()) {}

/*==============================================================================
  Stack Functions: run, push, pop, top
==============================================================================*/
template <class T, class D> Data<T, D> CompareStacks<T, D>::popCompare() {
  return StackAlgo<T, D>::mStack->pop(*this);
}

template <class T, class D> void CompareStacks<T, D>::runCompare(int buffer) {
  try {
    StackAlgo<T, D>::initStackIntern();
    while ((StackAlgo<T, D>::mInput.good())) {
      std::streampos position = StackAlgo<T, D>::mInput.tellg();
      (*StackAlgo<T, D>::mStack).setPosition(position);
      for (int i = 1; i <= buffer; i++) {
        bool bIndex =
            StackAlgo<T, D>::top(i).mIndex == mClassicStack->top(i).mIndex;
        bool bData =
            StackAlgo<T, D>::top(i).mData == mClassicStack->top(i).mData;
        if (!bIndex || !bData) {
          StackAlgo<T, D>::println();
          std::cout << mClassicStack->toString() << std::endl;
          throw "The top $(i)st elements are different";
        }
      }
      std::vector<std::string> line = StackAlgo<T, D>::readLine();
      if ((line.front() == "-1") || (line.front() == "")) {
        break;
      }
      D data = readInput(line);
      StackAlgo<T, D>::mIndex++;
      if ((*StackAlgo<T, D>::mStack).empty() != mClassicStack->empty()) {
        (*StackAlgo<T, D>::mStack).empty();
        StackAlgo<T, D>::println();
        std::cout << mClassicStack->toString() << std::endl;
        (*StackAlgo<T, D>::mStack).empty();
        throw "One stack is empty and not the other";
      }
      while ((!(StackAlgo<T, D>::emptystack())) && (popCondition(data))) {
        Data<T, D> elt = StackAlgo<T, D>::pop();
        Data<T, D> eltNormal = mClassicStack->pop();
        popAction(elt);
        bool bIndex = elt.mIndex == eltNormal.mIndex;
        bool bData = elt.mData == eltNormal.mData;
        if (!bIndex || !bData) {
          StackAlgo<T, D>::println();
          // std::cout << *StackAlgo<T, D>::mContext << std::endl;
          std::cout << mClassicStack->toString() << std::endl;
          throw "The two elements popped are different";
        }
      }
      if (pushCondition(data)) {
        Data<T, D> elt(StackAlgo<T, D>::mIndex, data);
        pushAction(elt);
        StackAlgo<T, D>::push(elt);
        mClassicStack->push(elt);
      }
      if (StackAlgo<T, D>::mStack->getBufferSize() > 0) {
        std::cout << "Is it working" << std::endl;
        for (int k = 1; k <= StackAlgo<T, D>::mStack->getBufferSize(); k++) {
          if (StackAlgo<T, D>::mStack->top(k).mIndex ==
              mClassicStack->top(k).mIndex) {
            StackAlgo<T, D>::println();
            // std::cout << *StackAlgo<T, D>::mContext << std::endl;
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

template <class T, class D> void CompareStacks<T, D>::readPush(int iter) {
  for (int i = 0; i < iter; i++) {
    std::streampos position = StackAlgo<T, D>::mInput.tellg();
    StackAlgo<T, D>::mStack->setPosition(position);
    std::vector<std::string> line = StackAlgo<T, D>::readLine();
    D data = readInput(line);
    StackAlgo<T, D>::mIndex++;
    Data<T, D> elt(StackAlgo<T, D>::mIndex, data);
    pushAction(elt);
    StackAlgo<T, D>::push(elt);
    mClassicStack->push(elt);
  }
}

#endif /* COMPARE */
