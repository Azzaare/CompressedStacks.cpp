#ifndef CLASSICSTACK
#define CLASSICSTACK

/*==============================================================================
  Includes
==============================================================================*/
#include "stack.hpp"
#include <memory>

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    :
  Friends   -> StackAlgo
            <-
==============================================================================*/
template <class T, class D> class StackAlgo;       // Required for the friendship
template <class T, class D> class CompareStacks; // Required for the friendship
class Comparison;                                // Required for the friendship
template <class T, class D> class ClassicStack : public Stack<T, D> {
  friend class StackAlgo<T, D>;
  friend class CompareStacks<T, D>;
  friend class Comparison;

public:
  // Constructors
  ClassicStack<T, D>();

  // IO
  std::string toString();

private:
  Explicit<T, D> mDatas; // vector of Data

  // Stack common methods
  Data<T, D> pop(StackAlgo<T, D> &problem);
  Data<T, D> pop();
  void push(const Data<T, D> &data);
  Data<T, D> top(int k = 1);
  bool empty(int lvl = -1, int component = 0);

  void compress();
  void copyContent(CompressedStack<T, D> &stack) {}

  // Setters
  void setContext(std::shared_ptr<T> context) {}
  void setPosition(std::streampos position) {}
  void setCompressed(Level<T, D> level) {}

  // Getters
  Level<T, D> getFirstPartial(int lvl);
  Level<T, D> getCompressed();
  ExplicitPointer<T, D> getFirstExplicit();
  Block<T, D> getFirstBlock();
  int getBufferSize ();
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D> ClassicStack<T, D>::ClassicStack() {
  Explicit<T, D> datas;
  datas = initExplicit<T, D>();
  mDatas = datas;
}

/*==============================================================================
  Stack Functions: push, pop, top, isempty
==============================================================================*/
template <class T, class D>
bool ClassicStack<T, D>::empty(int lvl, int component) {
  return mDatas.empty();
}

template <class T, class D>
Data<T, D> ClassicStack<T, D>::pop(StackAlgo<T, D> &problem) {
  Data<T, D> data = mDatas.back();
  mDatas.pop_back();
  return data;
}
template <class T, class D> Data<T, D> ClassicStack<T, D>::pop() {
  Data<T, D> data = mDatas.back();
  mDatas.pop_back();
  return data;
}

template <class T, class D>
void ClassicStack<T, D>::push(const Data<T, D> &elt) {
  mDatas.push_back(elt);
}

template <class T, class D> Data<T, D> ClassicStack<T, D>::top(int k) {
  int index = mDatas.size() - k;
  return mDatas.at(index);
}

template <class T, class D> void ClassicStack<T, D>::compress() {}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D>
Level<T, D> ClassicStack<T, D>::getFirstPartial(int lvl) {
  return initLevel<T, D>(0);
}

template <class T, class D> Level<T, D> ClassicStack<T, D>::getCompressed() {
  return initLevel<T, D>(0);
}

template <class T, class D>
ExplicitPointer<T, D> ClassicStack<T, D>::getFirstExplicit() {
  return initExplicitPointer<T, D>();
}

template <class T, class D>
int ClassicStack<T, D>::getBufferSize() {
  return 0;
}

template <class T, class D> Block<T, D> ClassicStack<T, D>::getFirstBlock() {
  Block<T, D> block(0, std::streampos(0), std::shared_ptr<T>(nullptr),
                       Buffer<T, D>(0));
  return block;
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D> std::string ClassicStack<T, D>::toString() {
  std::string str;
  str = "\tClassic Stack (Explicit Datas)\n\t\t";
  str += explicitToString(mDatas);
  return str;
}

#endif /* CLASSICSTACK */
