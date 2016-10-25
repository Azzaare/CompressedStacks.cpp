#ifndef STACKALGO
#define STACKALGO

/*==============================================================================
  Includes
==============================================================================*/
#include "../extras/classicStack.hpp"
#include "compressedStack.hpp"
#include "stack.hpp"
#include <cstdint>
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
template <class T, class D, class I> class StackAlgo {
  friend class CompressedStack<T, D, I>;
  friend class StackAlgoExtras<T, D, I>;

public:
  // Members functions
  StackAlgo<T, D, I>(std::string fileName, bool usecompressed = true);
  virtual ~StackAlgo<T, D, I>() {}

  // Running the stack
  void run();
  void run(I limit);
  I step();
  void popLoop(D data);
  void pushStep(D data);
  void push(Data<T, D, I> elt);
  Data<T, D, I> pop();
  Data<T, D, I> top(I k);
  bool emptystack();

  // Setters
  void setContext(const T &context);
  void setIndex(I index);

  // Getters
  T getContext();

  I getIndex();

  // IO
  std::string toString();
  void print();
  void println();
  void readPush(I iter = 1);

  // protected:
  void initStackIntern();
  std::vector<std::string> readLine();
  std::vector<std::string> readHeader();
  I mIndex;

  // private:
  // Input/Ouput
  std::ifstream mInput;
  std::ofstream mOutput; // output file is optional

  // Stack Functions: defined by user
  virtual std::shared_ptr<T> initStack() = 0;
  virtual D readInput(std::vector<std::string> line) = 0;

  virtual bool popCondition(D data) = 0;
  virtual void prePop(D data) = 0;
  virtual void postPop(D data, Data<T, D, I> elt) = 0;
  virtual void noPop(D data) = 0;

  virtual bool pushCondition(D data) = 0;
  virtual void prePush(Data<T, D, I> elt) = 0;
  virtual void postPush(Data<T, D, I> elt) = 0;
  virtual void noPush(D data) = 0;

  virtual void reportStack() = 0;

  // StackAlgo internal during run
  std::shared_ptr<T> mContext;

  // Stack: Classic or Compressed
  std::shared_ptr<Stack<T, D, I>> mStack;
};

/*==============================================================================
  Constructors : with ClassicStack or CompressedStack
==============================================================================*/
template <class T, class D, class I>
StackAlgo<T, D, I>::StackAlgo(std::string fileName, bool usecompressed)
    : mIndex(0), mContext(nullptr) {
  mInput.open(fileName, std::ifstream::in);

  std::vector<std::string> parameters = readHeader();

  if (usecompressed) {

    I p = 0, b = 0, n = 0;

    for (unsigned int i = 0; i < parameters.size(); i = i + 2) {
      if (parameters[i].compare("n") == 0)
        n = stoi(parameters[i + 1]);
      if (parameters[i].compare("p") == 0) {
        p = stoi(parameters[i + 1]);
      }
      if (parameters[i].compare("b") == 0) {
        b = stoi(parameters[i + 1]);
      }
    }

    if (p < 2)
      throw("StackAlgo<T,D>::StackAlgo(std::string fileName), wrong header "
            "format ");
    mStack = std::shared_ptr<Stack<T, D, I>>(
        new CompressedStack<T, D, I>(n, p, b, mContext));
  } else {
    mStack = std::shared_ptr<Stack<T, D, I>>(nullptr);
  }
}

/*==============================================================================
  IO : toString, print, println
==============================================================================*/
template <class T, class D, class I>
std::string StackAlgo<T, D, I>::toString() {
  std::string str;
  str = "Instance with an actual index of " + std::to_string(mIndex);
  str += ", with a stack of type\n";
  str += mStack->toString();
  return str;
}

template <class T, class D, class I> void StackAlgo<T, D, I>::print() {
  std::cout << toString();
}

template <class T, class D, class I> void StackAlgo<T, D, I>::println() {
  print();
  std::cout << std::endl;
}

template <class T, class D, class I>
std::vector<std::string> StackAlgo<T, D, I>::readLine() {
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

template <class T, class D, class I>
std::vector<std::string> StackAlgo<T, D, I>::readHeader() {
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

template <class T, class D, class I> void StackAlgo<T, D, I>::readPush(I iter) {
  for (I i = 0; i < iter; i++) {
    std::streampos position = mInput.tellg();
    (*mStack).setPosition(position);
    std::vector<std::string> line = readLine();
    D data = readInput(line);
    mIndex++;
    Data<T, D, I> elt(mIndex, data);
    prePush(elt);
    push(elt);
    postPush(elt);
  }
}

/*==============================================================================
  Stack Functions: run, push, pop, top, readPush
==============================================================================*/
template <class T, class D, class I> void StackAlgo<T, D, I>::popLoop(D data) {
  while (!emptystack()) {
    if (popCondition(data)) {
      prePop(data);
      Data<T, D, I> elt = pop();
      postPop(data, elt);
    } else {
      noPop(data);
      break;
    }
  }
}

template <class T, class D, class I> void StackAlgo<T, D, I>::pushStep(D data) {
  if (pushCondition(data)) {
    Data<T, D, I> elt(mIndex, data);
    prePush(elt);
    push(elt);
    postPush(elt);
  } else {
    noPush(data);
  }
}

template <class T, class D, class I> I StackAlgo<T, D, I>::step() {
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

template <class T, class D, class I> void StackAlgo<T, D, I>::run() {
  initStackIntern();

  while (mInput.good()) {
    if (step() == 0) {
      break;
    }
  }

  reportStack();
}

template <class T, class D, class I> void StackAlgo<T, D, I>::run(I limit) {
  // I testIndex = mIndex;
  while (mInput.good() && mIndex < limit) {
    if (step() == 0) {
      break;
    }
  }
}

template <class T, class D, class I>
void StackAlgo<T, D, I>::push(Data<T, D, I> elt) {
  mStack->push(elt);
}
template <class T, class D, class I> Data<T, D, I> StackAlgo<T, D, I>::pop() {
  return mStack->pop(*this);
}
template <class T, class D, class I>
Data<T, D, I> StackAlgo<T, D, I>::top(I k) {
  return mStack->top(k);
}
template <class T, class D, class I> bool StackAlgo<T, D, I>::emptystack() {
  return mStack->empty();
}

/*==============================================================================
  Setters
==============================================================================*/
template <class T, class D, class I>
void StackAlgo<T, D, I>::setContext(const T &context) {
  mContext = std::make_shared<T>(context);
  mStack->setContext(mContext);
}

template <class T, class D, class I>
void StackAlgo<T, D, I>::setIndex(I index) {
  mIndex = index;
}

template <class T, class D, class I>
void StackAlgo<T, D, I>::initStackIntern() {
  mContext = initStack();
}

/*==============================================================================
  Getters
==============================================================================*/
template <class T, class D, class I> T StackAlgo<T, D, I>::getContext() {
  return *mContext;
}

template <class T, class D, class I> I StackAlgo<T, D, I>::getIndex() {
  return mIndex;
}

#endif /* STACKALGO */
