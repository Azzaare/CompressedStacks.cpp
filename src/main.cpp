// Main file of the compressed stack library
#include "../include/sign.hpp"
#include "../include/data.hpp"
#include "../include/normalStack.hpp"
#include "../include/compressedStack.hpp"
#include "../include/problem.hpp"
#include "../sources/createTestInput.h"
#include <string>

// Test Signature
void testSign()
{
  Signature<int> sign (1, 0, 1);
  sign.println();
}

// Test data
void testData()
{
  Data<int> d (1,2);
  d.println();

  Data<int> d1 (1,2);
  Data<int> d2 (2,3);
  Data<int> d3 (3,5);
  Data<int> d4 (4,12);
  Data<int> d5 (5,892);

  Explicit<int> xplicit = initExplicit<int>();
  xplicit.push_back(d1);
  xplicit.back();
  xplicit.pop_back();
}

// Test normal stack
void testNormalStack()
{
  NormalStack<int> stack (5);
  Data<int> d1 (1,2);
  Data<int> d2 (2,3);
  Data<int> d3 (3,5);
  Data<int> d4 (4,12);
  Data<int> d5 (5,892);
  stack.push(d1);
  stack.push(d2);
  stack.push(d3);
  stack.push(d4);
  stack.push(d5);
  stack.println();
}

// Test normal stack
void testCompressedStack()
{
  Block<int> block = initBlock<int>(5);
  Levels<int> lvls = initLevels<int>(3, 3);
  Component<int, int> comp(3,5);
  CompressedStack<int, int> stack (81,3);
}

// Test problem
void testProblem()
{
  std::string filePath = "../instances/test.csv";
  Problem<int, int> problem (filePath, 5);


}

// Main
int main(int argc, char const *argv[]) {
  testSign();
  testData();
  testNormalStack();
  testCompressedStack();

  createTestInput ct=createTestInput();
  ct.createTestInputFiles(0,"/home/yago/code/CompressedStacks.cpp/instances/pushOnlyInput.csv",1000,3, 5, 100, 0.2 );
  ct.createTestInputFiles(1,"/home/yago/code/CompressedStacks.cpp/instances/CTInput.csv",1000,3);


  return 0;
}
