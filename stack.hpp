/** Stack : interface **/
#include "data.hpp"

template <class D>
class Stack
{
public:
  virtual Data<D> pop() = 0;
  virtual void push() = 0;
  virtual Data<D> top(int k) = 0;
};
