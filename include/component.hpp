#ifndef COMPONENT
#define COMPONENT

/**** Compressed Stack: declarations ****/
#include "sign.hpp"
#include "stack.hpp"
#include <string>
#include <memory>

/* Component of a Compressed Stack */
template <class T, class D>
class Component
{
public:
  Component<T,D>(int space, int depth);

  // Setters
  void setSignature(Signature<T> sign);
  void setLastSign(int index);

  // Push and pop
  void push(std::shared_ptr<Data<D>> elt);
  Data<D> top();

  // IO
  std::string toString();
  void print();
  void println();

  // State
  bool isempty();
  bool isExplicitEmpty();

private:
  Levels<T> mPartial;
  ExplicitPointer<D> mExplicit;
  Signature<T>* mSign;
};

/** Constructors **/
template <class T, class D>
Component<T,D>::Component(int space, int depth)
{
  mSign = nullptr;

  Levels<T> partial = initLevels<T>(space, depth);
  mPartial = partial;

  ExplicitPointer<D> xplicit =
  xplicit = initExplicitPointer<D>();
  xplicit.reserve(space);
  mExplicit = xplicit;
}

/** IO **/
template <class T>
std::string levelsToStringInComponent(Levels<T> levels)
{
  std::string str;
  str = "";
  int index = 0;
  for (typename Levels<T>::iterator it = levels.begin() ; it != levels.end(); ++it)
  {
    index++;
    str += "\t\t\tLevel" + std::to_string(index) + "->\n";
    str += "\t\t\t\t" + blockToString(*it) + "\n";
  }
  return str;
}

template <class T, class D>
std::string Component<T,D>::toString()
{
  std::string str;
  str = levelsToStringInComponent(mPartial);
  str += "\t\t\tExplicit->\n";
  str += explicitPointerToString(mExplicit);
  str += "\t\t\tSignature->\n";
  //str += (&mSign).toString() + "\n";
  return str;
}

/* State of the component */
template <class T, class D>
bool Component<T,D>::isempty(){
  if (mSign == nullptr) {
    return true;
  }
  return false;
}

template <class T, class D>
bool Component<T,D>::isExplicitEmpty(){
  return (mExplicit.empty());
}

/** Push and pop **/
template <class T, class D>
void Component<T,D>::push(std::shared_ptr<Data<D>> elt){
  mExplicit.push_back(elt);
}

template <class T, class D>
Data<D> Component<T,D>::top(){
  return *(mExplicit.back());
}

/** Setters **/
template <class T, class D>
void Component<T,D>::setSignature(Signature<T> sign){
  *mSign = sign;
}

#endif /* COMPONENT */
