// Main file of the compressed stack library
#include "../include/sign.hpp"
#include "../include/data.hpp"
#include "../include/normalStack.hpp"
#include "../include/compressedStack.hpp"
#include "../include/problem.hpp"
#include "../include/createTestInput.hpp"
#include <string>
#include <vector>

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
  stack.println();
}

// Class for test instance
class Instance: public Problem<int,int>{
public:
  Instance(std::string filePath, int size):Problem<int,int>(filePath, size){}
  Instance(std::string filePath, int size, int space):Problem<int,int>(filePath, size, space){}
private:
  // Functions to run the stack
  int readInput(std::vector<std::string> line){
    int value = std::stoi(line[0]);
    setContext(std::stoi(line[1]));
    return value;

  }
  void initStack(){
    setContext(10);
    std::vector<std::string> line = readLine(); // Temporary measure to get rid of the first line
  }
  bool popCondition(int data){
    if (getContext() > 0) {
      return true;
    }
    return false;
  }
  void popAction(Data<int> elt){
    setContext(getContext() - 1);
  }
  bool pushCondition(int data){
    if (data > 0) {
      return true;
    }
    return false;
  }
  void pushAction(Data<int> elt){
    std::cout << "Implement mPushAction for your instance" << std::endl;
  }
};


// Test problem
void testProblem()
{
  std::string filePath = "../instances/pushOnlyInput.csv";

  // Test on normal stack
  Instance testNS(filePath, 1000);
  testNS.run();
  testNS.println();

  // Test on CompressedStack
//  Instance testCS(filePath, 1000, 3);
//  testCS.println();
}

// Main
int main() {
  testProblem();

// Intances generation, please comment when not in use
/*  createTestInput ct=createTestInput();
  ct.createTestInputFiles(0,"/home/yago/code/CompressedStacks.cpp/instances/pushOnlyInput.csv",1000,3, 5, 100, 0.2 );
  ct.createTestInputFiles(1,"/home/yago/code/CompressedStacks.cpp/instances/CTInput.csv",1000,3);
  ct.createTestInputFiles(2,"/home/yago/code/CompressedStacks.cpp/instances/CHInput.csv",100,3,-1, 11);
*/

  return 0;
}
