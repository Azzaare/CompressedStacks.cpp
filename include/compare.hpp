#ifndef COMPARE
#define COMPARE

/*==============================================================================
  Includes
==============================================================================*/
#include "data.hpp"
#include "problem.hpp"

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions :
  Aliases    :
  Friends   ->
            <- Problem
==============================================================================*/
template <class T, class D> class CompareStacks : public Problem<T, D> {
public:
  // Members functions
  CompareStacks<T, D>(std::string fileName);

  // Compare the stacks
  void runCompare(int buffer = 0);

  // IO
  void printCompare() {
    std::string str = Problem<T, D>::toString();
    str += "\n" + (*mNormalStack).toString();
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
  std::shared_ptr<NormalStack<T, D>> mNormalStack;
};

/*==============================================================================
  Constructors :
==============================================================================*/

template <class T, class D>
CompareStacks<T, D>::CompareStacks(std::string fileName)
    : Problem<T, D>(fileName), mNormalStack(new NormalStack<T, D>()) {}

/*==============================================================================
  Stack Functions: run, push, pop, top
==============================================================================*/
template <class T, class D> Data<T, D> CompareStacks<T, D>::popCompare() {
  return Problem<T, D>::mStack->pop(*this);
}

template <class T, class D> void CompareStacks<T, D>::runCompare(int buffer) {
  try {
    Problem<T, D>::initStackIntern();
    while ((Problem<T, D>::mInput.good())) {
      std::streampos position = Problem<T, D>::mInput.tellg();
      (*Problem<T, D>::mStack).setPosition(position);
      for (int i = 1; i <= buffer; i++) {
        bool bIndex = Problem<T, D>::top(i).mIndex == mNormalStack->top(i).mIndex;
        bool bData = Problem<T, D>::top(i).mData == mNormalStack->top(i).mData;
        if (!bIndex || !bData) {
          Problem<T, D>::println();
          std::cout << mNormalStack->toString() << std::endl;
          throw "The top $(i)st elements are different";
        }
      }
      std::vector<std::string> line = Problem<T, D>::readLine();
      if ((line.front() == "-1") || (line.front() == "")) {
        break;
      }
      D data = readInput(line);
      Problem<T, D>::mIndex++;
      if ((*Problem<T, D>::mStack).empty() != mNormalStack->empty()) {
        (*Problem<T, D>::mStack).empty();
        Problem<T, D>::println();
        std::cout << mNormalStack->toString() << std::endl;
        (*Problem<T, D>::mStack).empty();
        throw "One stack is empty and not the other";
      }
      while ((!(Problem<T, D>::emptystack())) && (popCondition(data))) {
        Data<T, D> elt = Problem<T, D>::pop();
        Data<T, D> eltNormal = mNormalStack->pop();
        popAction(elt);
        bool bIndex = elt.mIndex == eltNormal.mIndex;
        bool bData = elt.mData == eltNormal.mData;
        if (!bIndex || !bData) {
          Problem<T, D>::println();
          //std::cout << *Problem<T, D>::mContext << std::endl;
          std::cout << mNormalStack->toString() << std::endl;
          throw "The two elements popped are different";
        }
      }
      if (pushCondition(data)) {
        Data<T, D> elt(Problem<T, D>::mIndex, data);
        pushAction(elt);
        Problem<T, D>::push(elt);
        mNormalStack->push(elt);
      }
      if (Problem<T, D>::mStack->getBufferSize() > 0) {
        std::cout << "Is it working" << std::endl;
        for (int k = 1; k <= Problem<T, D>::mStack->getBufferSize(); k++) {
          if (Problem<T, D>::mStack->top(k).mIndex == mNormalStack->top(k).mIndex) {
            Problem<T, D>::println();
            //std::cout << *Problem<T, D>::mContext << std::endl;
            std::cout << mNormalStack->toString() << std::endl;
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
    std::streampos position = Problem<T, D>::mInput.tellg();
    Problem<T, D>::mStack->setPosition(position);
    std::vector<std::string> line = Problem<T, D>::readLine();
    D data = readInput(line);
    Problem<T, D>::mIndex++;
    Data<T, D> elt(Problem<T, D>::mIndex, data);
    pushAction(elt);
    Problem<T, D>::push(elt);
    mNormalStack->push(elt);
  }
}

#endif /* COMPARE */
