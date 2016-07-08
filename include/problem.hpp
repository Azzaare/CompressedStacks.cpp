#ifndef PROBLEM
#define PROBLEM

/*==============================================================================
  Includes
==============================================================================*/
#include "stack.hpp"
#include "compressedStack.hpp"
#include "normalStack.hpp"
#include <fstream>
#include <vector>
#include <string>
#include <memory>

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions : Instance
  Aliases    :
  Friends   -> CompressedStack
            <- NormalStack, CompressedStack, Data
==============================================================================*/
template <class T, class D> class CompressedStack; // Required for the friendship
template <class T, class D>
class Problem{
  friend class CompressedStack<T,D>;

public:
  // Members functions
  Problem<T,D>(std::string fileName, int size);
  Problem<T,D>(std::string fileName, int size, int space, int buffer);
  virtual ~Problem<T,D>() {}

  // Running the stack
  void run();
  void run(int limit);
  void push(Data<T,D>);
  Data<T,D> pop();
  Data<T,D> top(int k);
  bool emptystack();

  // Setters
  void setContext(const T &context);
  void setIndex(int index);

  //Getters
  T getContext();
  int getIndex();

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
  std::ofstream mOutput; // output file is optional

  // Stack Functions: defined by user
  virtual D readInput(std::vector<std::string> line) = 0;
  virtual std::shared_ptr<T> initStack() = 0;
  virtual bool popCondition(D data) = 0;
  virtual void popAction(Data<T,D> elt) {};
  virtual bool pushCondition(D data) = 0;
  virtual void pushAction(Data<T,D> elt) {};

  // Problem internal during run
  std::shared_ptr<T> mContext;

  // Stack: Normal or Compressed
  std::shared_ptr<Stack<T,D>> mStack;
};

/*==============================================================================
  Constructors : with NormalStack or CompressedStack
==============================================================================*/
template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size)
: mIndex(0)
, mContext(nullptr)
, mStack(new NormalStack<T,D> (size)){
  mInput.open(fileName, std::ifstream::in);
}

template <class T, class D>
Problem<T,D>::Problem(std::string fileName, int size, int space, int buffer)
: mIndex(0)
, mContext(nullptr){
  mInput.open(fileName, std::ifstream::in);
  std::streampos position = mInput.tellg();
  mStack = std::shared_ptr<Stack<T,D>> (new CompressedStack<T,D> (size, space, buffer, mContext));
}

/*==============================================================================
  IO : toString, print, println
==============================================================================*/
template <class T, class D>
std::string Problem<T,D>::toString(){
  std::string str;
  str = "Problem with an actual index of " + std::to_string(mIndex);
  str += ", with a stack of type\n";
  str += mStack->toString();
  return str;
}

template <class T, class D>
void Problem<T,D>::print(){
  std::cout << toString();
}

template <class T, class D>
void Problem<T,D>::println(){
  print();
  std::cout << std::endl;
}

template <class T, class D>
std::vector<std::string> Problem<T,D>::readLine(){
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


/*==============================================================================
  Stack Functions: run, push, pop, top
==============================================================================*/
template <class T, class D>
void Problem<T,D>::run(){
  initStackIntern();
  while ((mInput.good())) {
    std::vector<std::string> line = readLine();
    if ( (line.front()== "-1") || (line.front()=="") ) {
      break;
    }
    D data = readInput(line);
    mIndex++; // Might have to move
    while ( (emptystack()) && (popCondition(data)) ) {
      Data<T,D> elt = pop();
      popAction(elt);
    }
    if (pushCondition(data)) {
      Data<T,D> elt (mIndex,data);
      pushAction(elt);
      push(elt);
    }
  }
}

template <class T, class D>
void Problem<T,D>::run(int limit){
  for (int i = 0; i < limit; i++) {
    std::vector<std::string> line = readLine();
    if ( (line.front()== "-1") || (line.front()=="") ) {
      break;
    }
    D data = readInput(line);
    mIndex++; // Might have to move
    while ((emptystack()) && (popCondition(data))) {
      Data<T,D> elt = pop();
      popAction(elt);
    }
    if (pushCondition(data)) {
      Data<T,D> elt (mIndex,data);
      pushAction(elt);
      push(elt);
    }
  }
}

template <class T, class D>
void Problem<T,D>::push(Data<T,D> elt){
  std::streampos position = mInput.tellg();
  mStack->push(elt, position);
}
template <class T, class D>
Data<T,D> Problem<T,D>::pop(){
  return mStack->pop(*this);
}
template <class T, class D>
Data<T,D> Problem<T,D>::top(int k){
  return mStack->top(k);
}
template <class T, class D>
bool Problem<T,D>::emptystack(){
  return mStack->isempty();
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D>
void Problem<T,D>::setContext(const T &context){
  mContext = std::make_shared<T>(context);
  mStack->setContext(mContext);
}

template <class T, class D>
void Problem<T,D>::setIndex(int index){
  mIndex = index;
}

template <class T, class D>
void Problem<T,D>::initStackIntern(){
  mContext = initStack();
}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D>
T Problem<T,D>::getContext(){
  return *mContext;
}

template <class T, class D>
int Problem<T,D>::getIndex(){
  return mIndex;
}

#endif /* PROBLEM */
