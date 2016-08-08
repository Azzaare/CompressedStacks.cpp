#ifndef DATA
#define DATA

/*==============================================================================
  Includes
==============================================================================*/
#include <iostream>
#include <memory>
#include <string>
#include <vector>

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    : Explicit, ExplicitPointer
  Friends   -> Component, CompressedStack, Problem
            <-
==============================================================================*/
template <class T, class D> class Component; // Required for the friendship
template <class T, class D>
class CompressedStack;                           // Required for the friendship
template <class T, class D> class Problem;       // Required for the friendship
template <class T, class D> class CompareStacks; // Required for the friendship
class Comparison;                                // Required for the friendship
template <class T, class D> class Data {
  friend class Component<T, D>;
  friend class CompressedStack<T, D>;
  friend class Problem<T, D>;
  friend class CompareStacks<T, D>;
  friend class Comparison;

public:
  // IO
  std::string toString();

  // Getters
  D getData();

private:
  // Constructor
  Data<T, D>(int index, D data);

  int mIndex;
  D mData;
};

template <class T, class D> using SPData = std::shared_ptr<Data<T, D>>;

template <class T, class D> using Explicit = std::vector<Data<T, D>>;
template <class T, class D> Explicit<T, D> initExplicit();

template <class T, class D> using ExplicitPointer = std::vector<SPData<T, D>>;
template <class T, class D> ExplicitPointer<T, D> initExplicitPointer();

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D> Data<T, D>::Data(int index, D data) {
  mIndex = index;
  mData = data;
}

template <class T, class D> Explicit<T, D> initExplicit() {
  Explicit<T, D> xplicit;
  return xplicit;
}

template <class T, class D> ExplicitPointer<T, D> initExplicitPointer() {
  ExplicitPointer<T, D> xpointer;
  return xpointer;
}

/*==============================================================================
  IO : toString, explicitToString, explicitPointerToString
==============================================================================*/
template <class T, class D> std::string Data<T, D>::toString() {
  std::string str;
  str = std::to_string(mIndex) + "<-" /*+ std::to_string(mData)*/;
  return str;
}

template <class T, class D>
std::string explicitToString(Explicit<T, D> xplicit) {
  std::string str;
  str = "{";
  for (typename Explicit<T, D>::iterator it = xplicit.begin();
       it != xplicit.end(); ++it)
    str += (*it).toString() + ",";
  str.back() = '}';
  return str;
}

template <class T, class D>
std::string explicitPointerToString(ExplicitPointer<T, D> xpointer) {
  std::string str = "";
  for (typename ExplicitPointer<T, D>::iterator it = xpointer.begin();
       it != xpointer.end(); ++it)
    if (str == "") {
      str += "{" + (*(*it)).toString();
    } else {
      str += "," + (*(*it)).toString();
    }
  if (str != "") {
    str += '}';
  }
  return str;
}

/*==============================================================================
  Getters : getData
==============================================================================*/
template <class T, class D> D Data<T, D>::getData() {
  return mData;
}


#endif /* DATA */
