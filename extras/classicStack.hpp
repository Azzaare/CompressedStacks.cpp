#ifndef CLASSICSTACK
#define CLASSICSTACK

/*==============================================================================
  Includes
==============================================================================*/
#include "../include/stack.hpp"
#include <memory>

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    :
  Friends   -> StackAlgo
            <-
==============================================================================*/
template <class T, class D, class I>
class ClassicStack : public Stack<T, D, I> {
  friend class StackAlgo<T, D, I>;
  friend class StackAlgoExtras<T, D, I>;

public:
  // Constructors
  ClassicStack<T, D, I>();

  // IO
  std::string toString();

private:
  Explicit<T, D, I> mDatas; // vector of Data

  // Stack common methods
  Data<T, D, I> pop(StackAlgo<T, D, I> &problem);
  Data<T, D, I> pop();
  void push(const Data<T, D, I> &data);
  Data<T, D, I> top(I k = 1);
  bool empty(std::intmax_t lvl = -1, I component = 0);

  void compress();
  void copyContent(CompressedStack<T, D, I> &stack) {}

  // Setters
  void setContext(std::shared_ptr<T> context) {}
  void setPosition(std::streampos position) {}
  void setCompressed(Level<T, D, I> level) {}

  // Getters
  Level<T, D, I> getFirstPartial(std::intmax_t lvl);
  Level<T, D, I> getCompressed();
  ExplicitPointer<T, D, I> getFirstExplicit();
  Block<T, D, I> getFirstBlock();
  I getBufferSize();
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D, class I> ClassicStack<T, D, I>::ClassicStack() {
  Explicit<T, D, I> datas;
  datas = initExplicit<T, D, I>();
  mDatas = datas;
}

/*==============================================================================
  Stack Functions: push, pop, top, isempty
==============================================================================*/
template <class T, class D, class I>
bool ClassicStack<T, D, I>::empty(std::intmax_t lvl, I component) {
  return mDatas.empty();
}

template <class T, class D, class I>
Data<T, D, I> ClassicStack<T, D, I>::pop(StackAlgo<T, D, I> &problem) {
  Data<T, D, I> data = mDatas.back();
  mDatas.pop_back();
  return data;
}
template <class T, class D, class I>
Data<T, D, I> ClassicStack<T, D, I>::pop() {
  Data<T, D, I> data = mDatas.back();
  mDatas.pop_back();
  return data;
}

template <class T, class D, class I>
void ClassicStack<T, D, I>::push(const Data<T, D, I> &elt) {
  mDatas.push_back(elt);
}

template <class T, class D, class I>
Data<T, D, I> ClassicStack<T, D, I>::top(I k) {
  I index = mDatas.size() - k;
  return mDatas.at(index);
}

template <class T, class D, class I> void ClassicStack<T, D, I>::compress() {}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D, class I>
Level<T, D, I> ClassicStack<T, D, I>::getFirstPartial(std::intmax_t lvl) {
  return initLevel<T, D, I>(0);
}

template <class T, class D, class I>
Level<T, D, I> ClassicStack<T, D, I>::getCompressed() {
  return initLevel<T, D, I>(0);
}

template <class T, class D, class I>
ExplicitPointer<T, D, I> ClassicStack<T, D, I>::getFirstExplicit() {
  return initExplicitPointer<T, D, I>();
}

template <class T, class D, class I> I ClassicStack<T, D, I>::getBufferSize() {
  return 0;
}

template <class T, class D, class I>
Block<T, D, I> ClassicStack<T, D, I>::getFirstBlock() {
  Block<T, D, I> block(0, std::streampos(0), std::shared_ptr<T>(nullptr),
                       Buffer<T, D, I>(0));
  return block;
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D, class I>
std::string ClassicStack<T, D, I>::toString() {
  std::string str;
  str = "\tClassic Stack (Explicit Datas)\n\t\t";
  str += explicitToString(mDatas);
  return str;
}

#endif /* CLASSICSTACK */
