#ifndef STACK
#define STACK

/*==============================================================================
  Includes
==============================================================================*/
#include "data.hpp"
#include <memory>

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions : NormalStack, CompressedStack
  Aliases    :
  Friends   ->
            <-
==============================================================================*/
template <class T, class D>
class Stack{
public:
  // Member functions
//  virtual Stack<T,D>() = 0;
  virtual ~Stack<T,D>() {};

  // Common stack functions
  virtual Data<T,D> pop() = 0;
  virtual void push(const Data<T,D> &data) = 0;
  virtual Data<T,D> top(int k) = 0;
  virtual bool isempty() = 0;

  // Setters
  virtual void setContext(std::shared_ptr<T> context) = 0;

  // IO
  virtual std::string toString() = 0;
};

#endif /* STACK */
