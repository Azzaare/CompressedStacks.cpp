/**** Normal Stack: definition ****/
#include "normalStack.hpp"

template <class D>
  Data<D> NormalStack<D>::pop()
  {
    return mDatas.pop();
  }

template <class D>
  void NormalStack<D>::push(Data<D> data)
  {
    mDatas.push(data);
  }

template <class D>
  Data<D>* NormalStack<D>::top(int k)
  {
    int index = mDdatas.size() - k;
    return &(data[index]);
  }
