#ifndef STACK
#define STACK

/*==============================================================================
  Includes
==============================================================================*/
#include "data.hpp"
#include "block.hpp"
#include <memory>

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions : NormalStack, CompressedStack
  Aliases    :
  Friends   ->
            <-
==============================================================================*/
template <class T, class D> class Stack {
public:
  // Member functions
  //  virtual Stack<T,D>() = 0;
  virtual ~Stack<T, D>(){};

  // Common stack functions
  virtual Data<T, D> pop(StackAlgo<T, D> &problem) = 0;
  virtual void push(const Data<T, D> &data) = 0;
  virtual Data<T, D> top(int k = 1) = 0;

  virtual bool empty(int lvl = -1, int component = 0) = 0;

  virtual void compress() = 0;
  virtual void copyContent(CompressedStack<T, D> &stack) = 0;

  // Getters
  virtual Level<T, D> getFirstPartial(int lvl) = 0;
  virtual Level<T, D> getCompressed() = 0;
  virtual ExplicitPointer<T, D> getFirstExplicit() = 0;
  virtual Signature<T, D> getFirstSign() = 0;
  virtual int getBufferSize() = 0;

  // Setters
  virtual void setContext(std::shared_ptr<T> context) = 0;
  virtual void setPosition(std::streampos position) = 0;
  virtual void setCompressed(Level<T, D> level) = 0;

  // IO
  virtual std::string toString() = 0;
};

#endif /* STACK */
