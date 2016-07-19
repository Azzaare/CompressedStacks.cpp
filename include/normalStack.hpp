#ifndef NORMALSTACK
#define NORMALSTACK

/*==============================================================================
  Includes
==============================================================================*/
#include "stack.hpp"
#include <memory>

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    :
  Friends   -> Problem
            <-
==============================================================================*/
template <class T, class D> class Problem; // Required for the friendship
template <class T, class D> class CompareStacks; // Required for the friendship
class Comparison; // Required for the friendship
template <class T, class D>
class NormalStack: public Stack<T,D>{
  friend class Problem<T,D>;
  friend class CompareStacks<T,D>;
  friend class Comparison;

public:
  // Constructors
  NormalStack<T,D>();

  // IO
  std::string toString();

private:
  Explicit<T,D> mDatas; // vector of Data

  // Stack common methods
  Data<T,D> pop(Problem<T,D> &problem);
  Data<T,D> pop();
  void push(const Data<T,D> &data);
  Data<T,D> top(int k);
  Data<T,D> top();
  bool isempty();

  bool isSecondEmpty();

  void compress();
  void copyContent(CompressedStack<T,D> &stack) {}

  // Setters
  void setContext(std::shared_ptr<T> context){}
  void setPosition(std::streampos position){}
  void setCompressed(Block<T,D> block){}

  // Getters
  Block<T,D> getFirstPartial(int lvl);
  Block<T,D> getCompressed();
  ExplicitPointer<T,D> getFirstExplicit();
  Signature<T,D> getFirstSign();
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D>
NormalStack<T,D>::NormalStack(){
  Explicit<T,D> datas;
  datas = initExplicit<T,D>();
  mDatas = datas;
}

/*==============================================================================
  Stack Functions: push, pop, top, isempty
==============================================================================*/
template <class T, class D>
bool NormalStack<T,D>::isempty(){
  return mDatas.empty();
}

template <class T, class D>
Data<T,D> NormalStack<T,D>::pop(Problem<T,D> &problem){
  Data<T,D> data = mDatas.back();
  mDatas.pop_back();
  return data;
}
template <class T, class D>
Data<T,D> NormalStack<T,D>::pop(){
  Data<T,D> data = mDatas.back();
  mDatas.pop_back();
  return data;
}

template <class T, class D>
void NormalStack<T,D>::push(const Data<T,D> &elt){
  mDatas.push_back(elt);
}

template <class T, class D>
Data<T,D> NormalStack<T,D>::top(int k){
  int index = mDatas.size() - k;
  return mDatas.at(index);
}

template <class T, class D>
Data<T,D> NormalStack<T,D>::top(){
  return top(1);
}

template <class T, class D>
bool NormalStack<T,D>::isSecondEmpty(){
  return isempty();
}

template <class T, class D>
void NormalStack<T,D>::compress(){
}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D>
Block<T,D> NormalStack<T,D>::getFirstPartial(int lvl){
  return initBlock<T,D>(0);
}

template <class T, class D>
Block<T,D> NormalStack<T,D>::getCompressed(){
  return initBlock<T,D>(0);
}

template <class T, class D>
ExplicitPointer<T,D> NormalStack<T,D>::getFirstExplicit(){
  return initExplicitPointer<T,D>();
}

template <class T, class D>
Signature<T,D> NormalStack<T,D>::getFirstSign(){
  Signature<T,D> sign(0, std::streampos (0), std::shared_ptr<T>(nullptr), Buffer<T,D>(0));
  return sign;
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D>
std::string NormalStack<T,D>::toString()
{
  std::string str;
  str = "\tNormal Stack (Explicit Datas)\n\t\t";
  str += explicitToString(mDatas);
  return str;
}

#endif /* NORMALSTACK */
