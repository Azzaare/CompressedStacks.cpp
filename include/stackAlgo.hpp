#ifndef STACKALGO
#define STACKALGO

/*==============================================================================
  Includes
==============================================================================*/
#include "classicStack.hpp"
#include "compressedStack.hpp"
#include "stack.hpp"
#include <exception>
#include <fstream>
#include <memory>
#include <string>
#include <vector>

/*==============================================================================
  Class      : abstract, template (T context, D datas)
  Extensions : Instance
  Aliases    :
  Friends   -> CompressedStack
            <- ClassicStack, CompressedStack, Data
==============================================================================*/
template <class T, class D>
class CompressedStack;                           // Required for the friendship
template <class T, class D> class CompareStacks; // Required for the friendship
class Comparison;                                // Required for the friendship
template <class T, class D> class StackAlgo {
  friend class CompressedStack<T, D>;
  friend class CompareStacks<T, D>;
  friend class Comparison;

public:
  // Members functions
  StackAlgo<T, D>(std::string fileName, bool useclassic = false);
  virtual ~StackAlgo<T, D>() {}

  // Walking the stack
  void walk(int steps = 1);
  int step();
  void popLoop(D data);
  void pushStep(D data);

  // Running the stack
  void run();
  void run(int limit);
  void push(Data<T, D> elt);
  Data<T, D> pop();
  Data<T, D> top(int k);
  bool emptystack();

  // Setters
  void setContext(const T &context);
  void setIndex(int index);

  // Getters
  T getContext();

  int getIndex();

  // IO
  std::string toString();
  void print();
  void println();
  void readPush(int iter = 1);

protected:
  void initStackIntern();
  std::vector<std::string> readLine();
  std::vector<std::string> readHeader();
  int mIndex;

private:
  // Input/Ouput
  std::ifstream mInput;
  std::ofstream mOutput; // output file is optional

  // Stack Functions: defined by user
  virtual std::shared_ptr<T> initStack() = 0;
  virtual D readInput(std::vector<std::string> line) = 0;

  virtual bool popCondition(D data) = 0;
  virtual void prePop(D data) = 0;
  virtual void postPop(D data, Data<T, D> elt) = 0;
  virtual void noPop(D data) = 0;

  virtual bool pushCondition(D data) = 0;
  virtual void prePush(Data<T, D> elt) = 0;
  virtual void postPush(Data<T, D> elt) = 0;
  virtual void noPush(D data) = 0;

  virtual void reportStack() = 0;

  // StackAlgo internal during run
  std::shared_ptr<T> mContext;

  // Stack: Classic or Compressed
  std::shared_ptr<Stack<T, D>> mStack;
};

/*==============================================================================
  Constructors : with ClassicStack or CompressedStack
==============================================================================*/
template <class T, class D>
StackAlgo<T, D>::StackAlgo(std::string fileName, bool useclassic)
    : mIndex(0), mContext(nullptr) {
  mInput.open(fileName, std::ifstream::in);

  std::vector<std::string> parameters = readHeader();

  int p, b, n = 0;
  bool foundP = false;
  bool foundBuffer = false;

  for (unsigned int i = 0; i < parameters.size(); i = i + 2) {
    if (parameters[i].compare("n") == 0)
      n = stoi(parameters[i + 1]);
    if (parameters[i].compare("p") == 0) {
      foundP = true;
      p = stoi(parameters[i + 1]);
    }
    if (parameters[i].compare("b") == 0) {
      foundBuffer = true;
      b = stoi(parameters[i + 1]);
    }
  }

  if (foundBuffer && !foundP)
    throw("StackAlgo<T,D>::StackAlgo(std::string fileName), wrong header "
          "format ");
  if (!foundP || useclassic)
    mStack = std::shared_ptr<Stack<T, D>>(
        new ClassicStack<T, D>()); // space not provided, classic stack
  else                             // space was provided, compressed stack
  {
    if (!foundBuffer)
      b = 0;
    mStack = std::shared_ptr<Stack<T, D>>(
        new CompressedStack<T, D>(n, p, b, mContext));
  }
}

/*==============================================================================
  IO : toString, print, println
==============================================================================*/
template <class T, class D> std::string StackAlgo<T, D>::toString() {
  std::string str;
  str = "Instance with an actual index of " + std::to_string(mIndex);
  str += ", with a stack of type\n";
  str += mStack->toString();
  return str;
}

template <class T, class D> void StackAlgo<T, D>::print() {
  std::cout << toString();
}

template <class T, class D> void StackAlgo<T, D>::println() {
  print();
  std::cout << std::endl;
}

template <class T, class D>
std::vector<std::string> StackAlgo<T, D>::readLine() {
  std::string str;
  std::vector<std::string> line;
  size_t pos = std::string::npos;
  getline(mInput, str);
  while (true) {
    pos = str.find_first_of(",");
    line.push_back(str.substr(0, pos));
    str.erase(0, pos + 1);
    if (pos == std::string::npos) {
      line.push_back(str.substr(0, pos));
      str.erase(0, pos);
      break;
    }
  }
  return line;
}

template <class T, class D>
std::vector<std::string> StackAlgo<T, D>::readHeader() {
  std::string str;
  std::vector<std::string> line;
  size_t pos = std::string::npos;

  getline(mInput, str); // to read the first HEADER LINE

  getline(mInput, str);
  while (str.compare("/HEADER ") != 0) {
    pos = str.find_first_of(" ");
    line.push_back(str.substr(0, pos));
    line.push_back(str.substr(pos + 1, str.size() - pos - 1));
    getline(mInput, str);
  }
  return line;
}

template <class T, class D> void StackAlgo<T, D>::readPush(int iter) {
  for (int i = 0; i < iter; i++) {
    std::streampos position = mInput.tellg();
    (*mStack).setPosition(position);
    std::vector<std::string> line = readLine();
    D data = readInput(line);
    mIndex++;
    Data<T, D> elt(mIndex, data);
    prePush(elt);
    push(elt);
  }
}

/*==============================================================================
  Stack Functions: run, push, pop, top, readPush
==============================================================================*/
template <class T, class D> void StackAlgo<T, D>::walk(int steps) {
  for (int i = 0; i < steps; i++) {
    if (step() == 0 || mInput.good()) {
      break;
    }
  }
}

template <class T, class D> void StackAlgo<T, D>::popLoop(D data) {
  while (!emptystack()) {
    if (popCondition(data)) {
      prePop(data);
      Data<T, D> elt = pop();
      postPop(data, elt);
    } else {
      noPop(data);
      break;
    }
  }
}

template <class T, class D> void StackAlgo<T, D>::pushStep(D data) {
  if (pushCondition(data)) {
    Data<T, D> elt(mIndex, data);
    prePush(elt);
    push(elt);
    postPush(elt);
  } else {
    noPush(data);
  }
}

template <class T, class D> int StackAlgo<T, D>::step() {
  // Storing the position in the input file
  std::streampos position = mInput.tellg();
  (*mStack).setPosition(position);

  // Reading a new element
  std::vector<std::string> line = readLine();
  if (line.front() == "") {
    // Return 0 if it has to stop (EOF)
    return 0;
  }
  D data = readInput(line);

  // Increasing index of the number of elements
  mIndex++;

  // Call the pop loop
  popLoop(data);

  // Call the conditional push
  pushStep(data);

  // Return 1 in case of success
  return 1;
}

template <class T, class D> void StackAlgo<T, D>::run() {
  initStackIntern();
  while (mInput.good()) {
    if (step() == 0) {
      break;
    }
  }

  reportStack();
}

template <class T, class D> void StackAlgo<T, D>::run(int limit) {
  // int testIndex = mIndex;
  while (mInput.good() && mIndex < limit) {
    if (step() == 0) {
      break;
    }
  }
}

template <class T, class D> void StackAlgo<T, D>::push(Data<T, D> elt) {
  mStack->push(elt);
}
template <class T, class D> Data<T, D> StackAlgo<T, D>::pop() {
  return mStack->pop(*this);
}
template <class T, class D> Data<T, D> StackAlgo<T, D>::top(int k) {
  return mStack->top(k);
}
template <class T, class D> bool StackAlgo<T, D>::emptystack() {
  return mStack->empty();
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D> void StackAlgo<T, D>::setContext(const T &context) {
  mContext = std::make_shared<T>(context);
  mStack->setContext(mContext);
}

template <class T, class D> void StackAlgo<T, D>::setIndex(int index) {
  mIndex = index;
}

template <class T, class D> void StackAlgo<T, D>::initStackIntern() {
  mContext = initStack();
}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D> T StackAlgo<T, D>::getContext() {
  return *mContext;
}

template <class T, class D> int StackAlgo<T, D>::getIndex() { return mIndex; }

#endif /* STACKALGO */
