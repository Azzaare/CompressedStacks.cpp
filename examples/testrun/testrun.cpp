// Test Run : Implementation

/*==============================================================================
  Includes
==============================================================================*/
#include "include/testrun.hpp"

/*==============================================================================
  TestRun implements StackAlgo<int, int>
  Stack functions to define the a simple Test Run  algo with Compressed Stack
    * readInput
    * initStack
    * popCondition
    * prePop
    * postPop
    * noPop
    * pushCondition
    * prePush
    * postPush
    * noPush
    * reportStack
==============================================================================*/
int TestRun::readInput(std::vector<std::string> line) {
  int value = std::stoi(line[0]);
  setContext(std::stoi(line[1]));
  return value;
}
std::shared_ptr<int> TestRun::initStack() {
  std::shared_ptr<int> context(new int(0));
  return context;
}
bool TestRun::popCondition(int data) {
  if ((getContext() > 0)) {
    return true;
  }
  return false;
}
void TestRun::prePop(int data) {}
void TestRun::postPop(int data, Data<int, int> elt) {
  setContext(getContext() - 1);
}
void TestRun::noPop(int data) {}

bool TestRun::pushCondition(int data) {
  if (data > 0) {
    return true;
  }
  return false;
}
void TestRun::prePush(Data<int, int> elt) {}
void TestRun::postPush(Data<int, int> elt) {}
void TestRun::noPush(int data) {}

void TestRun::reportStack() {}

/*==============================================================================
  Comparison implements ComparisonStack<int, int>
  Stack functions to define the a simple Test Run algo with stack comparison
    * readInput
    * initStack
    * popCondition
    * prePop
    * postPop
    * noPop
    * pushCondition
    * prePush
    * postPush
    * noPush
    * reportStack
==============================================================================*/
int Comparison::readInput(std::vector<std::string> line) {
  int value = std::stoi(line[0]);
  setContext(std::stoi(line[1]));
  return value;
}
std::shared_ptr<int> Comparison::initStack() {
  std::shared_ptr<int> context(new int(0));
  return context;
}
bool Comparison::popCondition(int data) {
  if (getContext() > 0) {
    return true;
  }
  return false;
}
void Comparison::prePop(int data) {}
void Comparison::postPop(int data, Data<int, int> elt) {
  setContext(getContext() - 1);
}
void Comparison::noPop(int data) {}

bool Comparison::pushCondition(int data) {
  if (data > 0) {
    return true;
  }
  return false;
}
void Comparison::prePush(Data<int, int> elt) {}
void Comparison::postPush(Data<int, int> elt) {}
void Comparison::noPush(int data) {}

void Comparison::reportStack() {}

/*==============================================================================
  How to use
    * argv[1] is the file name
    * argv[2] is the code of what is to be done (is no space provided -> 0)
      * 0 is a test run problem with a classical stack
      * 1 is a test run hull problem with a compressed stack
      * 2 is a test run hull problem with both stacks comparison (check errors)
==============================================================================*/
int main(int argc, char *argv[]) {
  // Getting the path of the instance to test
  std::string filepath = argv[1];

  switch (atoi(argv[2])) {
  case 0: {
    TestRun stack(filepath, true);
    stack.run();
    stack.println();
    break;
  }
  case 1: {
    TestRun stack(filepath);
    stack.run();
    stack.println();
    break;
  }
  default: {
    Comparison stack(filepath);
    stack.runCompare();
    stack.println();
    break;
  }
  }
  return 0;
}
