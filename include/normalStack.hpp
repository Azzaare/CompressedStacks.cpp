#ifndef NORMALSTACK
#define NORMALSTACK

/**** Normal Stack: declaration ****/
#include "stack.hpp"

template <class D>
class NormalStack: public Stack<D>
{
public:
  // Constructors
  NormalStack<D>(int size);

  // Stack common methods
  Data<D> pop();
  void push(Data<D> data);
  Data<D> top(int k);

  // IO
  std::string toString();
  void print();
  void println();

private:
  Explicit<D> mDatas; // vector of Data
};

/** Constructors **/
template <class D>
NormalStack<D>::NormalStack(int size)
{
  Explicit<D> datas;
  datas = initExplicit<D>();
  mDatas = datas;
}

/** Stack internal methods **/
template <class D>
  Data<D> NormalStack<D>::pop()
  {
    Data<D> data = mDatas.back();
    mDatas.pop_back();
    return data;
  }

template <class D>
  void NormalStack<D>::push(Data<D> data)
  {
    mDatas.push_back(data);
  }

template <class D>
  Data<D> NormalStack<D>::top(int k)
  {
    int index = mDatas.size() - k;
    return mDatas.at(index);
  }

/** IO **/
template <class D>
std::string NormalStack<D>::toString()
{
  std::string str;
  str = "\tNormal Stack (Explicit Datas)\n\t\t";
  return str;
}
template <class D>
void NormalStack<D>::print()
{
  std::cout << this->toString();
  printExplicit(mDatas);
}
template <class D>
  void NormalStack<D>::println()
{
  this->print();
  std::cout << std::endl;
}

#endif /* NORMALSTACK */
