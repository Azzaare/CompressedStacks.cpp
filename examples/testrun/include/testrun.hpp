// Test Run : Definition
#ifndef TESTRUN
#define TESTRUN

/*==============================================================================
  Includes
==============================================================================*/
#include "../../../include/stackAlgo.hpp"

/*==============================================================================
  Class      : implements stackAlgo
  Extensions : (indirect) TestRunComparison
  Aliases    :
  Friends   ->
            <- ClassicStack, CompressedStack, Data
==============================================================================*/
template <class I> class TestRun : public StackAlgo<int, int, I> {
public:
  TestRun(std::string filePath) : StackAlgo<int, int, I>(filePath) {}

private:
  int readInput(std::vector<std::string> line);
  std::shared_ptr<int> initStack();

  bool popCondition(int data);
  void prePop(int data);
  void postPop(int data, Data<int, int, I> elt);
  void noPop(int data);

  bool pushCondition(int data);
  void prePush(Data<int, int, I> elt);
  void postPush(Data<int, int, I> elt);
  void noPush(int data);

  void reportStack();
};

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
template <class I> int TestRun<I>::readInput(std::vector<std::string> line) {
  int value = std::stoi(line[0]);
  StackAlgo<int, int, I>::setContext(std::stoi(line[1]));
  return value;
}
template <class I> std::shared_ptr<int> TestRun<I>::initStack() {
  std::shared_ptr<int> context(new int(0));
  return context;
}
template <class I> bool TestRun<I>::popCondition(int data) {
  if ((StackAlgo<int, int, I>::getContext() > 0)) {
    return true;
  }
  return false;
}
template <class I> void TestRun<I>::prePop(int data) {}
template <class I> void TestRun<I>::postPop(int data, Data<int, int, I> elt) {
  StackAlgo<int, int, I>::setContext(StackAlgo<int, int, I>::getContext() - 1);
}
template <class I> void TestRun<I>::noPop(int data) {}

template <class I> bool TestRun<I>::pushCondition(int data) {
  if (data > 0) {
    return true;
  }
  return false;
}
template <class I> void TestRun<I>::prePush(Data<int, int, I> elt) {}
template <class I> void TestRun<I>::postPush(Data<int, int, I> elt) {}
template <class I> void TestRun<I>::noPush(int data) {}

template <class I> void TestRun<I>::reportStack() {}

#endif // TESTRUN
