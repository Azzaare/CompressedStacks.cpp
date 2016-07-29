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
  Instance(std::string filePath):Problem<int,int>(filePath){ }
private:
  // Functions to run the stack
  int readInput(std::vector<std::string> line){
    int value = std::stoi(line[0]);
    setContext(std::stoi(line[1]));
    return value;

  }
  std::shared_ptr<int> initStack(){
    std::shared_ptr<int> context (new int (0));
    return context;
  }
  bool popCondition(int data){
    if ((getContext() > 0)){
      return true;
    }
    return false;
  }
  void popAction(Data<int,int> elt){
    std::cout << elt.toString() << " <<<< Pop!" << std::endl;
    println();
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
    println();
  }
};

/*==============================================================================
  Instantiation of a comparison
==============================================================================*/

class Comparison: public CompareStacks<int,int>{
public:
  Comparison(std::string filePath):CompareStacks<int,int>(filePath){}
private:
  // Functions to run the stack
  int readInput(std::vector<std::string> line){
    int value = std::stoi(line[0]);
    setContext(std::stoi(line[1]));
    return value;
  }
  std::shared_ptr<int> initStack(){
    std::shared_ptr<int> context (new int (0));
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
void testProblem(std::string filePath)
{
  // Test on normal stack
//  Instance testNS(filePath);
//  testNS.run();
//  testNS.println();

  // Test on CompressedStack
  std::cout << "Debug testProblem 1" << std::endl;
  Instance testCS(filePath);
  std::cout << "Debug testProblem 2" << std::endl;
  testCS.run();
  std::cout << "Debug testProblem 3" << std::endl;
  testCS.println();
}

void testCompare(std::string filePath){

  Comparison comp(filePath);
  comp.runCompare();
  comp.printCompare();
}
/*==============================================================================
  Main function
==============================================================================*/
int main(int argc, char *argv[]) {

  // argv[0] is the file name, argv[1] is the code of what is to be done, 0 run the example contained in the file, others are create input: 1 push only, 2 CT, 3 CH
  createTestInput ct=createTestInput();
  std::string filename=argv[1];

  switch(atoi(argv[2]))
  {
    case 0:
          testProblem(filename);
//          testCompare(filename);
    break;
    case 1:
      ct.createTestInputFiles(0,filename,atoi(argv[3]),atoi(argv[4]), atoi(argv[5]), atoi(argv[6]), atof(argv[7]) );

         break;
    case 2:
      ct.createTestInputFiles(1,filename,atoi(argv[3]),atoi(argv[4]));

      break;
    case 3:
      ct.createTestInputFiles(2,filename,atoi(argv[3]),atoi(argv[4]), atoi(argv[5]), atoi(argv[6]));

     break;
     default:
          std::cout << "WRONG PROGRAM CODE";
          exit(-1);
      break;
  }

// Intances generation, please comment when not in use
/*  createTestInput ct=createTestInput();
  ct.createTestInputFiles(0,"/home/yago/code/CompressedStacks.cpp/instances/pushOnlyInput.csv",1000,3, 5, 100, 0.2 );
  ct.createTestInputFiles(1,"/home/yago/code/CompressedStacks.cpp/instances/CTInput.csv",1000,3);
  ct.createTestInputFiles(2,"/home/yago/code/CompressedStacks.cpp/instances/CHInput.csv",100,3,-1, 11);
*/


  return 0;
}
