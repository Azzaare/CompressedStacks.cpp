#ifndef STACK
#define STACK

/** Stack : interface **/
#include "data.hpp"

template <class D>
class Stack
{
public:
  // Common stack functions
  virtual Data<D> pop() = 0;
  virtual void push(Data<D> data) = 0;
  virtual Data<D> top(int k) = 0;

  // IO
  virtual std::string toString() = 0;
  virtual void print() = 0;
  virtual void println() = 0;
};

#endif /* STACK */
