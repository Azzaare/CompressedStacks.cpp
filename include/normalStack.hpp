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
template <class T, class D>
class NormalStack: public Stack<T,D>{
  friend class Problem<T,D>;

public:
  // Constructors
  NormalStack<T,D>(int size);

  // IO
  std::string toString();

private:
  Explicit<T,D> mDatas; // vector of Data

  // Stack common methods
  Data<T,D> pop(Problem<T,D> &problem);
  void push(const Data<T,D> &data, std::streampos position);
  Data<T,D> top(int k);
  bool isempty();

  bool isSecondEmpty();

  void compress();

  // Setters
  void setContext(std::shared_ptr<T> context){}

  // Getters
  Block<T,D> getFirstPartial(int lvl);
  Block<T,D> getCompressed();
  ExplicitPointer<T,D> getFirstExplicit();
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D>
NormalStack<T,D>::NormalStack(int size){
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
void NormalStack<T,D>::push(const Data<T,D> &elt, std::streampos position){
  mDatas.push_back(elt);
}

template <class T, class D>
Data<T,D> NormalStack<T,D>::top(int k){
  int index = mDatas.size() - k;
  return mDatas.at(index);
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

/*==============================================================================
  IO : toString
==============================================================================*/
template <class T, class D>
std::string NormalStack<T,D>::toString()
{
  std::string str;
  str = "\tNormal Stack (Explicit Datas)\n\t\t";
  return str;
}

#endif /* NORMALSTACK */
