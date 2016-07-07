#ifndef STACK
#define STACK

/*==============================================================================
  Includes
==============================================================================*/
#include "data.hpp"

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions : Instance
  Aliases    :
  Friends   ->
            <- NormalStack
==============================================================================*/
template <class D>
class Stack{
public:
  // Member functions
//  virtual Stack<D>() = 0;
  virtual ~Stack<D>() {};

  // Common stack functions
  virtual Data<D> pop() = 0;
  virtual void push(Data<D> data) = 0;
  virtual Data<D> top(int k) = 0;
  virtual bool isempty() = 0;

  // IO
  virtual std::string toString() = 0;
};

#endif /* STACK */
