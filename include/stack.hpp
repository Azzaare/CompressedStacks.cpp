#ifndef STACK
#define STACK

/*==============================================================================
  Includes
==============================================================================*/
#include "block.hpp"
#include "data.hpp"
#include <memory>

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions : NormalStack, CompressedStack
  Aliases    :
  Friends   ->
            <-
==============================================================================*/
template <class T, class D, class I> class Stack {
public:
  // Member functions
  //  virtual Stack<T,D>() = 0;
  virtual ~Stack<T, D, I>(){};

  // Common stack functions
  virtual Data<T, D, I> pop(StackAlgo<T, D, I> &problem) = 0;
  virtual void push(const Data<T, D, I> &data) = 0;
  virtual Data<T, D, I> top(I k = 1) = 0;

  virtual bool empty(std::intmax_t lvl = -1, I component = 0) = 0;

  virtual void compress() = 0;
  virtual void copyContent(CompressedStack<T, D, I> &stack) = 0;

  // Getters
  virtual Level<T, D, I> getFirstPartial(std::intmax_t lvl) = 0;
  virtual Level<T, D, I> getCompressed() = 0;
  virtual ExplicitPointer<T, D, I> getFirstExplicit() = 0;
  virtual Block<T, D, I> getFirstBlock() = 0;
  virtual I getBufferSize() = 0;

  // Setters
  virtual void setContext(std::shared_ptr<T> context) = 0;
  virtual void setPosition(std::streampos position) = 0;
  virtual void setCompressed(Level<T, D, I> level) = 0;

  // IO
  virtual std::string toString() = 0;
};

#endif /* STACK */
