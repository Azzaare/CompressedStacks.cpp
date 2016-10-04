# CompressedStacks.cpp
[![Build Status](https://travis-ci.org/Azzaare/CompressedStacks.cpp.svg?branch=master)](https://travis-ci.org/Azzaare/CompressedStacks.cpp)

The CompressedStacks.cpp module/library implements a time-space trade-off structure for stack's algorithms.

<!-- TODO: Write down a better description -->

## Category of algorithms
<p>
This compressed stack structure works correctly as a normal stack for any problems that read input from a file. However, the running time is optimal when the input would be read sequentially with a classical stack structure. For this reason, the only function implemented in the Problem template to solve it (to do a run) is the one presented below in a simplified version.
</p>

```cpp
template <class T, class D> void Problem<T, D>::run() {
  initStack();
  while (notEndOfFile()) {
    D data = readInput(line);
    while (notEmptystack() && popCondition(data)) {
      elt = pop();
      popAction(elt);
    }
    if (pushCondition(data)) {
      pushAction(data);
      push(data);
    }
  }
}
```

## Characterization of a problem
<p>In the followwing examples, implementations of the Problem interface are given.</p>

### General example : ```Instance<T,D>```

```cpp
#include <string>
#include <vector>
#include <memory>

// T is the type of the context and D is the type of the input data.
class Instance: public Problem<T,D>{
public:
  Instance(std::string filePath) : Problem<T, D>(filePath) {}
private:
  // Functions to implement according to the problem and input
  D readInput(std::vector<std::string> line){
    std::cout << "Implement readInput for your instance" << std::endl;
    return 0;
  }
  std::shared_ptr<T> initStack(){
    std::cout << "Implement initStack for your instance" << std::endl;
    std::shared_ptr<T> context(nullptr);
    return context;
  }
  bool popCondition(D data){
    std::cout << "Implement mPopCondition for your instance" << std::endl;
    return false;
  }
  void popAction(Data<T, D> elt){
    std::cout << "Implement mPopAction for your instance" << std::endl;
  }
  bool pushCondition(D data){
    std::cout << "Implement mPushCondition for your instance" << std::endl;
    return true;
  }
  void pushAction(Data<T, D> elt){
    std::cout << "Implement mPushAction for your instance" << std::endl;
  }
};
```

### Example with ```T = int``` and ```D = int``` : ```Instance<int,int>```
The context is initialized at 0. The data (in cvs format) is read as a pair of string such that the first string is the data  and the second is used to update the context. While the context is more than 0, the stack is poped and the context decreased by 1. If the data is more than 0 then it is pushed.
```cpp
class Instance : public Problem<int, int> {
public:
  Instance(std::string filePath) : Problem<int, int>(filePath) {}

private:
  // Functions to run the stack
  int readInput(std::vector<std::string> line) {
    int value = std::stoi(line[0]);
    setContext(std::stoi(line[1]));
    return value;
  }
  std::shared_ptr<int> initStack() {
    std::shared_ptr<int> context(new int(0));
    return context;
  }
  bool popCondition(int data) {
    if ((getContext() > 0)) {
      return true;
    }
    return false;
  }
  void popAction(Data<int, int> elt) {
    std::cout << elt.toString() << " <<<< Pop!" << std::endl;
    setContext(getContext() - 1);
  }
  bool pushCondition(int data) {
    if (data > 0) {
      return true;
    }
    return false;
  }
  void pushAction(Data<int, int> elt) {
    std::cout << "Push >>>> " << elt.toString() << std::endl;
  }
};
```

## How to run your problem
Suppose the class Instance implement the interface Problem<T,D> (as in some examples above). You can run an instance of your problem described in the input located at <i>filepath</i>. The last command just print an output in th console of your compressed stack after the run.

```cpp
Instance stack(filePath);
stack.run();
stack.println();
```
