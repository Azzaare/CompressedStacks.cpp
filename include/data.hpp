#ifndef DATA
#define DATA

/** Data class : declaration **/
#include <vector>
#include <string>
#include <iostream>

template <class D>
class Data
{
public:
  // Constructor
  Data<D>(int index, D data);

  // Getters
  int getIndex();
  D getData();

  // IO
  std::string toString();
  void print();
  void println();

private:
  int mIndex;
  D mData;
};

template<class D> using Explicit = std::vector<Data<D>>;
template<class D> Explicit<D> initExplicit();

/** Constructors **/
template <class D>
Data<D>::Data(int index, D data)
{
  mIndex = index;
  mData = data;
}

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

/** IO **/
template <class D>
std::string Data<D>::toString()
{
  std::string str;
  str = std::to_string(mIndex);
  return str;
}
template <class D>
void Data<D>::print()
{
  std::string str;
  str = this->toString();
  std::cout << str;
}
template <class D>
void Data<D>::println()
{
  this->print();
  std::cout << "\n";
}

template <class D>
std::string toString(Explicit<D> xplicit)
{
  std::string str;
  str = "{";
  for (typename Explicit<D>::iterator it = xplicit.begin() ; it != xplicit.end(); ++it)
    str += (*it).toString() + ",";
  str.back() = '}';
  return str;
}

template <class D>
void printExplicit(Explicit<D> xplicit)
{
  std::string str;
  str = toString(xplicit);
  std::cout << str;
}
template <class D>
void printlnExplicit(Explicit<D> xplicit)
{
  printExplicit(xplicit);
  std::cout << "\n";
}

// Explicit constructor
template <class D>
Explicit<D> initExplicit()
{
  Explicit<D> xplicit;
  return xplicit;
}

#endif /* DATA */
