// Test Run : Definition
#ifndef TESTRUNEXTRAS
#define TESTRUNEXTRAS

/*==============================================================================
  Includes
==============================================================================*/
#include "../../../extras/stackAlgoExtras.hpp"

/*==============================================================================
  Class      : implements TestExtras
  Extensions : (indirect) TestRun
  Aliases    :
  Friends   ->
            <- ClassicStack, CompressedStack, Data
==============================================================================*/
template <class I> class TestExtras : public StackAlgoExtras<int, int, I> {
public:
  TestExtras(std::string filePath, bool usecompressed, bool useclassic)
      : StackAlgoExtras<int, int, I>(filePath, usecompressed, useclassic) {}

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
  TestExtras implements TestExtrasStack<int, int, I>
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
template <class I> int TestExtras<I>::readInput(std::vector<std::string> line) {

  int value = std::stoi(line[0]);
  StackAlgoExtras<int, int, I>::setContext(std::stoi(line[1]));
  return value;
}
template <class I> std::shared_ptr<int> TestExtras<I>::initStack() {
  std::shared_ptr<int> context(new int(0));
  return context;
}
template <class I> bool TestExtras<I>::popCondition(int data) {
  if (StackAlgoExtras<int, int, I>::getContext() > 0) {
    return true;
  }
  return false;
}
template <class I> void TestExtras<I>::prePop(int data) {}
template <class I>
void TestExtras<I>::postPop(int data, Data<int, int, I> elt) {
  StackAlgoExtras<int, int, I>::setContext(
      StackAlgoExtras<int, int, I>::getContext() - 1);
}
template <class I> void TestExtras<I>::noPop(int data) {}

template <class I> bool TestExtras<I>::pushCondition(int data) {
  if (data > 0) {
    return true;
  }
  return false;
}
template <class I> void TestExtras<I>::prePush(Data<int, int, I> elt) {}
template <class I> void TestExtras<I>::postPush(Data<int, int, I> elt) {}
template <class I> void TestExtras<I>::noPush(int data) {}

template <class I> void TestExtras<I>::reportStack() {}

#endif // TESTRUNEXTRAS
