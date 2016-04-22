/**** Normal Stack: declaration ****/
#include "stack.hpp"

template <class D>
class NormalStack:Stack<D>
{
public:
  Data<D> pop();
  void push(Data<D> newdata);
  Data<D>* top(int k);

private:
  Explicit<D> mDatas; // vector of Data
};
