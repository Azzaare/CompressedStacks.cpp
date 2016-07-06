#ifndef PROBLEM
#define PROBLEM

/**** Problem : declaration ****/
#include "stack.hpp"
#include "compressedStack.hpp"
#include "normalStack.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <memory>

template <class T, class D>
class Problem
{
public:
  // Members functions
  Problem<T,D>(std::string fileName, int size);
  Problem<T,D>(std::string fileName, int size, int space, int buffer);
  virtual ~Problem<T,D>() {}

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
  void initStackIntern();
  std::vector<std::string> readLine();
  int mIndex;

private:
  // Input/Ouput
  std::ifstream mInput;
  std::shared_ptr<std::ofstream> mOutput; // output file is optional

  // Stack Functions: defined by user
  virtual D readInput(std::vector<std::string> line) = 0;
  virtual std::shared_ptr<T> initStack() = 0;
  virtual bool popCondition(D data) = 0;
  virtual void popAction(Data<D> elt) {};
  virtual bool pushCondition(D data) = 0;
  virtual void pushAction(Data<D> elt) {};

  // Problem internal during run
  std::shared_ptr<T> mContext;

  // Stack: Normal or Compressed
  std::shared_ptr<Stack<D>> mStack;
};

/** Constructors **/
template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size)
{
  mInput.open(fileName, std::ifstream::in);
  mOutput = nullptr;

  mContext = (nullptr);
  mIndex = 0;

  mStack = std::shared_ptr<Stack<D>> (new NormalStack<T> (size));
}

template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size, int space, int buffer)
{
  mInput.open(fileName, std::ifstream::in);
  mOutput = nullptr;

  mContext = (nullptr);
  mIndex = 0;

  mStack = std::shared_ptr<Stack<D>> (new CompressedStack<T,D> (size, space, buffer, mContext));
}

/** IO **/
template <class T, class D>
std::string Problem<T,D>::toString(){
  std::string str;
  std::cout << "Debug Problem::toString 1" << std::endl;
  str = "Problem with an actual index of " + std::to_string(mIndex);
  std::cout << "Debug Problem::toString 2" << std::endl;
  str += ", with a stack of type\n";
  str += mStack->toString();
  std::cout << "Debug Problem::toString 3" << std::endl;
  return str;
}

template <class T, class D>
void Problem<T,D>::print()
{
  std::cout << toString();
}

template <class T, class D>
void Problem<T,D>::println()
{
  print();
  std::cout << std::endl;
}

template <class T, class D>
std::vector<std::string> Problem<T,D>::readLine()
{
  std::string str;
  std::vector<std::string> line;
  size_t pos=std::string::npos;
  getline(mInput,str);
  while (true){
    pos=str.find_first_of(",");
    line.push_back(str.substr(0,pos));
    str.erase(0,pos+1);
    if (pos==std::string::npos){
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
  initStackIntern();
  while ((mInput.good())) {
    std::vector<std::string> line = readLine();
    if ( (line.front()== "-1") || (line.front()=="") ) {
      break;
    }
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
    }
  }
}

/** Push, pop, and top **/
template <class T, class D>
void Problem<T,D>::push(Data<D> elt){
  mStack->push(elt);
}
template <class T, class D>
Data<D> Problem<T,D>::pop(){
  return mStack->pop();
}
template <class T, class D>
Data<D> Problem<T,D>::top(int k){
  return mStack->top(k);
}
template <class T, class D>
bool Problem<T,D>::emptystack(){
  return mStack->isempty();
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
//  std::cout << "setContext, T = " << context << std::endl;
  *mContext = context;
//  std::cout << "setContext, *mContext = " << (*mContext) << std::endl;
//  std::cout << "setContext, *mContext = " << getContext() << std::endl;
}

template <class T, class D>
void Problem<T,D>::initStackIntern(){
  mContext = initStack();
}

/** Getters **/
template <class T, class D>
T Problem<T,D>::getContext(){
  return *mContext;
}

#endif /* PROBLEM */
