/** Data class : implementation **/
#include "data.hpp"

/** Getters **/
template <class D>
int Data<D>::getIndex()
{
  return mIndex;
}
template <class D>
D Data<D>::getData()
{
  return mData;
}
