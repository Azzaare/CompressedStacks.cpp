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
  Friends   -> Component, CompressedStack, StackAlgo, CompareStacks, Comparison
            <-
==============================================================================*/
template <class T, class D, class I>
class Component; // Required for the friendship
template <class T, class D, class I>
class CompressedStack; // Required for the friendship
template <class T, class D, class I>
class CompressedStackExtras; // Required for the friendship
template <class T, class D, class I>
class StackAlgo; // Required for the friendship
template <class T, class D, class I>
class StackAlgoExtras; // Required for the friendship

template <class T, class D, class I> class Data {
  friend class Component<T, D, I>;
  friend class CompressedStack<T, D, I>;
  friend class StackAlgo<T, D, I>;
  friend class CompressedStackExtras<T, D, I>;
  friend class StackAlgoExtras<T, D, I>;

public:
  // IO
  std::string toString();

  // Getters
  D getData();

private:
  // Constructor
  Data<T, D, I>(I index, D data);

  I mIndex;
  D mData;
};

template <class T, class D, class I>
using SPData = std::shared_ptr<Data<T, D, I>>;

template <class T, class D, class I>
using Explicit = std::vector<Data<T, D, I>>;
template <class T, class D, class I> Explicit<T, D, I> initExplicit();

template <class T, class D, class I>
using ExplicitPointer = std::vector<SPData<T, D, I>>;
template <class T, class D, class I>
ExplicitPointer<T, D, I> initExplicitPointer();

/*==============================================================================
  Constructors
==============================================================================*/
template <class T, class D, class I> Data<T, D, I>::Data(I index, D data) {
  mIndex = index;
  mData = data;
}

template <class T, class D, class I> Explicit<T, D, I> initExplicit() {
  Explicit<T, D, I> xplicit;
  return xplicit;
}

template <class T, class D, class I>
ExplicitPointer<T, D, I> initExplicitPointer() {
  ExplicitPointer<T, D, I> xpointer;
  return xpointer;
}

/*==============================================================================
  IO : toString, explicitToString, explicitPointerToString
==============================================================================*/
template <class T, class D, class I> std::string Data<T, D, I>::toString() {
  std::string str;
  str = std::to_string(mIndex) + "<-" /*+ std::to_string(mData)*/;
  return str;
}

template <class T, class D, class I>
std::string explicitToString(Explicit<T, D, I> xplicit) {
  std::string str;
  str = "{";
  for (typename Explicit<T, D, I>::iterator it = xplicit.begin();
       it != xplicit.end(); ++it)
    str += (*it).toString() + ",";
  str.back() = '}';
  return str;
}

template <class T, class D, class I>
std::string explicitPointerToString(ExplicitPointer<T, D, I> xpointer) {
  std::string str = "";
  for (typename ExplicitPointer<T, D, I>::iterator it = xpointer.begin();
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
template <class T, class D, class I> D Data<T, D, I>::getData() {
  return mData;
}

#endif /* DATA */
