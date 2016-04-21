/**** Problem : implementation ****/
#include "problem.hpp"


/** Constructors **/
template <class T, class D>
Problem<T,D>::Problem(std::string fileName)
{
  std::ifstream ifstr;
  ifstr.open(fileName, std::ifstream::in);
  mInput = ifstr;
}
