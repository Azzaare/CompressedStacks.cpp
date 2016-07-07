#ifndef DATA
#define DATA

/*==============================================================================
  Includes
==============================================================================*/
#include <vector>
#include <string>
#include <iostream>
#include <memory>

/*==============================================================================
  Class      : template (D datas)
  Extensions :
  Aliases    : Explicit, ExplicitPointer
  Friends   ->
            <-
==============================================================================*/
template <class D>
class Data{
public:
  // Constructor
  Data<D>(int index, D data);

  // IO
  std::string toString();

  // Getters
  int getIndex();

private:
  int mIndex;
  D mData;
};

template<class D> using Explicit = std::vector<Data<D>>;
template<class D> Explicit<D> initExplicit();

template<class D> using ExplicitPointer = std::vector<std::shared_ptr<Data<D>>>;
template<class D> ExplicitPointer<D> initExplicitPointer();

/*==============================================================================
  Constructors
==============================================================================*/
template <class D>
Data<D>::Data(int index, D data)
{
  mIndex = index;
  mData = data;
}

template <class D>
Explicit<D> initExplicit()
{
  Explicit<D> xplicit;
  return xplicit;
}

template <class D>
ExplicitPointer<D> initExplicitPointer()
{
  ExplicitPointer<D> xpointer;
  return xpointer;
}

/*==============================================================================
  Getters
==============================================================================*/
template <class D>
int Data<D>::getIndex()
{
  return mIndex;
}

/*==============================================================================
  IO : toString
==============================================================================*/
template <class D>
std::string Data<D>::toString()
{
  std::string str;
  str = std::to_string(mIndex) + "<-" + std::to_string(mData);
  return str;
}

template <class D>
std::string explicitToString(Explicit<D> xplicit)
{
  std::string str;
  str = "{";
  for (typename Explicit<D>::iterator it = xplicit.begin() ; it != xplicit.end(); ++it)
    str += (*it).toString() + ",";
  str.back() = '}';
  return str;
}

template <class D>
std::string explicitPointerToString(ExplicitPointer<D> xpointer)
{
  std::string str;
  str = "{";
  for (typename ExplicitPointer<D>::iterator it = xpointer.begin() ; it != xpointer.end(); ++it)
    str += (*(*it)).toString() + ",";
  str.back() = '}';
  return str;
}

#endif /* DATA */
