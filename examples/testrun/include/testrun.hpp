// Test Run : Definition
#ifndef TESTRUN
#define TESTRUN

/*==============================================================================
  Includes
==============================================================================*/
#include "../../../include/compare.hpp"
// #include "../include/stackAlgo.hpp" // Not necessary with compare.hpp

/*==============================================================================
  Class      : implements stackAlgo
  Extensions : (indirect) TestRunComparison
  Aliases    :
  Friends   ->
            <- ClassicStack, CompressedStack, Data
==============================================================================*/
class TestRun : public StackAlgo<int, int> {
public:
  TestRun(std::string filePath, bool useclassic = false)
      : StackAlgo<int, int>(filePath, useclassic) {}

private:
  int readInput(std::vector<std::string> line);
  std::shared_ptr<int> initStack();

  bool popCondition(int data);
  void prePop(int data);
  void postPop(int data, Data<int, int> elt);
  void noPop(int data);

  bool pushCondition(int data);
  void prePush(Data<int, int> elt);
  void postPush(Data<int, int> elt);
  void noPush(int data);

  void reportStack();
};

/*==============================================================================
  Class      : implements Comparison
  Extensions : (indirect) TestRun
  Aliases    :
  Friends   ->
            <- ClassicStack, CompressedStack, Data
==============================================================================*/
class Comparison : public CompareStacks<int, int> {
public:
  Comparison(std::string filePath) : CompareStacks<int, int>(filePath) {}

private:
  int readInput(std::vector<std::string> line);
  std::shared_ptr<int> initStack();

  bool popCondition(int data);
  void prePop(int data);
  void postPop(int data, Data<int, int> elt);
  void noPop(int data);

  bool pushCondition(int data);
  void prePush(Data<int, int> elt);
  void postPush(Data<int, int> elt);
  void noPush(int data);

  void reportStack();
};

#endif // TESTRUN
