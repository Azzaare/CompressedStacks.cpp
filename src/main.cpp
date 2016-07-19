// Main file of the compressed stack library

/*==============================================================================
  Includes
==============================================================================*/
#include "../include/problem.hpp"
#include "../include/compare.hpp"
#include "../include/createTestInput.hpp"
#include <string>
#include <vector>
#include <memory>

/*==============================================================================
  Instantiation of a problem
==============================================================================*/
class Instance: public Problem<int,int>{
public:
  Instance(std::string filePath, int size):Problem<int,int>(filePath, size){}
  Instance(std::string filePath, int size, int space, int buffer):Problem<int,int>(filePath, size, space, buffer){}
private:
  // Functions to run the stack
  int readInput(std::vector<std::string> line){
    int value = std::stoi(line[0]);
    setContext(std::stoi(line[1]));
    return value;

  }
  std::shared_ptr<int> initStack(){
    std::shared_ptr<int> context (new int (0));
    std::vector<std::string> line = readLine(); // Temporary measure to get rid of the first line
    return context;
  }
  bool popCondition(int data){
    if (getContext() > 0) {
      return true;
    }
    return false;
  }
  void popAction(Data<int,int> elt){
    setContext(getContext() - 1);
//    std::cout << "Pop : " << elt.toString() << std::endl;
  }
  bool pushCondition(int data){
    if (data > 0) {
      return true;
    }
    return false;
  }
  void pushAction(Data<int,int> elt){
//  std::cout << "Push : " << elt.toString() << std::endl;
  }
};

/*==============================================================================
  Instantiation of a comparison
==============================================================================*/

class Comparison: public CompareStacks<int,int>{
public:
  Comparison(std::string filePath, int size, int space, int buffer):CompareStacks<int,int>(filePath, size, space, buffer){}
private:
  // Functions to run the stack
  int readInput(std::vector<std::string> line){
    int value = std::stoi(line[0]);
    setContext(std::stoi(line[1]));
    return value;
  }
  std::shared_ptr<int> initStack(){
    std::shared_ptr<int> context (new int (0));
    std::vector<std::string> line = readLine(); // Temporary measure to get rid of the first line
    return context;
  }
  bool popCondition(int data){
    if (getContext() > 0) {
      return true;
    }
    return false;
  }
  void popAction(Data<int,int> elt){
    std::cout << elt.toString() << " <<<< Pop!" << std::endl;
    setContext(getContext() - 1);
  }
  bool pushCondition(int data){
    if (data > 0) {
      return true;
    }
    return false;
  }
  void pushAction(Data<int,int> elt){
  std::cout << "Push >>>> " << elt.toString() << std::endl;
  }
};

/*==============================================================================
  Test functions
==============================================================================*/
// Test problem
void testProblem()
{
  std::string filePath = "../instances/CTInput.csv";

  // Test on normal stack
  Instance testNS(filePath, 1000);
  testNS.run();
  testNS.println();

  // Test on CompressedStack
//  Instance testCS(filePath, 1000, 3, 0);
//  testCS.println();
//  testCS.run();
//testCS.println();
}

void testCompare(){
  std::string filePath = "../instances/CTInput.csv";

  Comparison comp(filePath, 1000, 3, 0);
  comp.runCompare();
  comp.printCompare();
}
/*==============================================================================
  Main function
==============================================================================*/
// Main //int main(int argc, char const *argv[]) {
int main() {
//  testProblem();

  testCompare();

// Intances generation, please comment when not in use
/*  createTestInput ct=createTestInput();
  ct.createTestInputFiles(0,"/home/yago/code/CompressedStacks.cpp/instances/pushOnlyInput.csv",1000,3, 5, 100, 0.2 );
  ct.createTestInputFiles(1,"/home/yago/code/CompressedStacks.cpp/instances/CTInput.csv",1000,3);
  ct.createTestInputFiles(2,"/home/yago/code/CompressedStacks.cpp/instances/CHInput.csv",100,3,-1, 11);
*/

  return 0;
}
