#ifndef NORMALSTACK
#define NORMALSTACK

/*==============================================================================
  Includes
==============================================================================*/
#include "stack.hpp"

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    :
  Friends   -> Problem
            <-
==============================================================================*/
template <class T, class D> class Problem; // Required for the friendship
template <class T, class D>
class NormalStack: public Stack<D>{
  friend class Problem<T,D>;

public:
  // Constructors
  NormalStack<T,D>(int size);

  // IO
  std::string toString();

private:
  Explicit<D> mDatas; // vector of Data

  // Stack common methods
  Data<D> pop();
  void push(Data<D> data);
  Data<D> top(int k);
  bool isempty();
};

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D>
NormalStack<T,D>::NormalStack(int size){
  Explicit<D> datas;
  datas = initExplicit<D>();
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
Data<D> NormalStack<T,D>::pop(){
  Data<D> data = mDatas.back();
  mDatas.pop_back();
  return data;
}

template <class T, class D>
void NormalStack<T,D>::push(Data<D> elt){
  mDatas.push_back(elt);
}

template <class T, class D>
Data<D> NormalStack<T,D>::top(int k){
  int index = mDatas.size() - k;
  return mDatas.at(index);
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
