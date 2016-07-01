#ifndef PROBLEM
#define PROBLEM

/**** Problem : declaration ****/
#include "stack.hpp"
#include "compressedStack.hpp"
#include "normalStack.hpp"
//#include <stdbool.h>
#include <fstream>
#include <vector>
#include <string>

template <class T, class D>
class Problem
{
public:
  // Problem<T,D>(std::string fileName);
  Problem<T,D>(std::string fileName, int size);
  Problem<T,D>(std::string fileName, int size, int space);

  // Running the stack
  void run();
  void push(Data<D>);
  Data<D> pop();
  Data<D> top(int k);
  bool emptystack();

  // Setters
  void setOutput(std::string fileName);
  void setContext(T context);

  //Getters
  T getContext();

  // IO
  std::string toString();
  void print();
  void println();

protected:
  std::vector<std::string> readLine();
  int mIndex;

private:
  // Input/Ouput
  std::ifstream mInput;
  std::ofstream* mOutput; // output file is optional

  // Stack Functions: defined by user
  virtual D readInput(std::vector<std::string> line) = 0;
  virtual void initStack() = 0;
  virtual bool popCondition(D data) = 0;
  virtual void popAction(Data<D> elt) {};
  virtual bool pushCondition(D data) = 0;
  //virtual void pushAction() = 0;
  virtual void pushAction(Data<D> elt) {};

  // Problem internal during run
  T* mContext;

  // Stack: Normal or Compressed
  Stack<D>* mStack;
};

/** Constructors **/
template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size)
{
  mInput.open(fileName, std::ifstream::in);
  mOutput = nullptr;

  mContext = nullptr;
  mIndex = 0;

  mStack = new NormalStack<T> (size);
}

template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size, int space)
{
  mInput.open(fileName, std::ifstream::in);
  mOutput = nullptr;

  mContext = nullptr;
  mIndex = 0;

  mStack = new CompressedStack<T,D> (size, space);
}

/** IO **/
template <class T, class D>
std::string Problem<T,D>::toString(){
  std::string str;
  str = "Problem with an actual index of " + std::to_string(mIndex);
  str += ", with a stack of type\n";
  str += (*mStack).toString();
  return str;
}

template <class T, class D>
void Problem<T,D>::print()
{
  std::cout << this->toString();
}

template <class T, class D>
void Problem<T,D>::println()
{
  this->print();
  std::cout << std::endl;
}

template <class T, class D>
std::vector<std::string> Problem<T,D>::readLine()
{
  std::string str;
  std::vector<std::string> line;
  size_t pos=std::string::npos;
  getline(mInput,str);
  std::cout << "Debug 4 : " << str << std::endl;
  while (true){
    pos=str.find_first_of(",");
    line.push_back(str.substr(0,pos));
    std::cout << "Debug 4.2 : " << str << " and pos = " << pos << std::endl;
    str.erase(0,pos+1);
    std::cout << "Debug 4.1 : " << str << " and pos = " << pos << std::endl;
    if (pos=std::string::npos){
      line.push_back(str.substr(0,pos));
      str.erase(0,pos);
      break;
    }
  }
  return line;
}


/** Running the stack **/
template <class T, class D>
void Problem<T,D>::run() {
  initStack();
  while ((mInput.good())) {
    std::cout << "Debug 1" << std::endl;
    std::vector<std::string> line = readLine();
    if ( (line.front()== "-1") || (line.front()=="") ) {
      break;
    }
    std::cout << "Debug 2 : size = " << line.size() << std::endl;
    std::cout << "Context = " << (line.back()) << std::endl;
    D data = readInput(line);
    mIndex++; // Might have to move
    while ( (emptystack()) && (popCondition(data)) ) {
      Data<D> elt = pop();
      popAction(elt);
    }
    if (pushCondition(data)) {
      Data<D> elt (mIndex,data);
      pushAction(elt);
      push(elt);
      println();
    }
  }
}

/** Push, pop, and top **/
template <class T, class D>
void Problem<T,D>::push(Data<D> elt){
  (*mStack).push(elt);
}
template <class T, class D>
Data<D> Problem<T,D>::pop(){
  return (*mStack).pop();
}
template <class T, class D>
Data<D> Problem<T,D>::top(int k){
  return (*mStack).top(k);
}
template <class T, class D>
bool Problem<T,D>::emptystack(){
  return (*mStack).isempty();
}

/** Setters **/
template <class T, class D>
void Problem<T,D>::setOutput(std::string fileName){
  std::ofstream ofstr;
  ofstr.open(fileName, std::ofstream::out);
  mOutput = &ofstr;
}

template <class T, class D>
void Problem<T,D>::setContext(T context){
  std::cout << "setContext, T = " << context << std::endl;
  mContext = &context;
  std::cout << "setContext, *mContext = " << (*mContext) << std::endl;
  std::cout << "setContext, *mContext = " << getContext() << std::endl;
}

/** Getters **/
template <class T, class D>
T Problem<T,D>::getContext(){
  return *mContext;
}

#endif /* PROBLEM */
