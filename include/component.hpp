#ifndef COMPONENT
#define COMPONENT

/**** Compressed Stack: declarations ****/
#include "sign.hpp"
#include "stack.hpp"
#include <string>

/* Component of a Compressed Stack */
template <class T, class D>
class Component
{
public:
  Component<T,D>(int space, int depth);

  // Setters
  void setSignature(Signature<T> sign);
  void setLastSign(int index);

  // IO
  std::string toString();
  void print();
  void println();

private:
  Levels<T> mPartial;
  Explicit<D> mExplicit;
  Signature<T>* mSign;
};

/** Constructors **/
template <class T, class D>
Component<T,D>::Component(int space, int depth)
{
  mSign = nullptr;

  Levels<T> partial = initLevels<T>(space, depth);
  mPartial = partial;

  Explicit<D> xplicit;
  xplicit = initExplicit<D>();
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
  str += explicitToString(mExplicit);
  str += "\t\t\tSignature->\n";
  //str += (&mSign).toString() + "\n";
  return str;
}

#endif /* COMPONENT */
