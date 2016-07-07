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

  // Getters
  Signature<T> getSign();
  std::shared_ptr<Signature<T>> getSignPtr();
  int getLastSign();
  ExplicitPointer<D> getExplicit();

  // Setters
  void setSignature(std::shared_ptr<Signature<T>> sign);
  void setLastSign(int index);
  void setExplicit(ExplicitPointer<D> xplicit);
  void clearExplicit(int space);

  // Push and pop
  void pushExplicit(std::shared_ptr<Data<D>> elt);
  void push(Signature<T> sign, int lvl);
  Data<D> top();
  Signature<T> top(int lvl);
  int topIndex(int lvl);
  int topIndex();

  // IO
  std::string toString();
  void print();
  void println();

  // State
  bool isempty();
  bool isempty(int lvl);
  bool isExplicitEmpty();

private:
  Levels<T> mPartial;
  ExplicitPointer<D> mExplicit;
  std::shared_ptr<Signature<T>> mSign;
};

/** Constructors **/
template <class T, class D>
Component<T,D>::Component(int space, int depth)
{
  mSign = std::shared_ptr<Signature<T>> (nullptr);

  Levels<T> partial = initLevels<T>(space, depth);
  mPartial = partial;

  ExplicitPointer<D> xplicit = initExplicitPointer<D>();
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
  bool b = bool (mSign);
  return !b;
}

template <class T, class D>
bool Component<T,D>::isempty(int lvl){
  bool b;
  if (lvl > mPartial.size()) {
    b = isExplicitEmpty();
  } else {
    b = (mPartial[lvl]).empty();
  }
  return b;
}

template <class T, class D>
bool Component<T,D>::isExplicitEmpty(){
  return (mExplicit.empty());
}

/** Push and pop **/
template <class T, class D>
void Component<T,D>::pushExplicit(std::shared_ptr<Data<D>> elt){
  mExplicit.push_back(elt);
}
template <class T, class D>
void Component<T,D>::push(Signature<T> sign, int lvl){
  mPartial[lvl].push_back(sign);
}

template <class T, class D>
Data<D> Component<T,D>::top(){
  return *(mExplicit.back());
}
template <class T, class D>
Signature<T> Component<T,D>::top(int lvl){
  return mPartial[lvl].back();
}

template <class T, class D>
int Component<T,D>::topIndex(){
  Data<D> elt = top();
  int index = elt.getIndex();
  return index;
}
template <class T, class D>
int Component<T,D>::topIndex(int lvl){
  Signature<T> sign = top(lvl);
  int index = sign.getLast();
  return index;
}

/** Setters **/
template <class T, class D>
void Component<T,D>::setSignature(std::shared_ptr<Signature<T>> sign){
  mSign = sign;
}

template <class T, class D>
void Component<T,D>::setLastSign(int index){
  mSign->setLast(index);
}

template <class T, class D>
void Component<T,D>::setExplicit(ExplicitPointer<D> xplicit){
  mExplicit = xplicit;
}

template <class T, class D>
void Component<T,D>::clearExplicit(int space){
  mExplicit.clear();
  mExplicit.reserve(space);
}

// Getters
template <class T, class D>
Signature<T> Component<T,D>::getSign(){
  return *mSign;
}

template <class T, class D>
std::shared_ptr<Signature<T>> Component<T,D>::getSignPtr(){
  return mSign;
}

template <class T, class D>
int Component<T,D>::getLastSign(){
  return mSign->getLast();
}

template <class T, class D>
ExplicitPointer<D> Component<T,D>::getExplicit(){
  return mExplicit;
}

#endif /* COMPONENT */
