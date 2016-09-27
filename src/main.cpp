// Main file of the compressed stack library

/*==============================================================================
  Includes
==============================================================================*/
#include "../include/compare.hpp"
#include "../include/convexHull.hpp"
#include "../include/createTestInput.hpp"
#include "../include/stackAlgo.hpp"
#include <memory>
#include <string>
#include <vector>

/*==============================================================================
  Instantiation of a problem
==============================================================================*/
class Instance : public StackAlgo<int, int> {
public:
  Instance(std::string filePath) : StackAlgo<int, int>(filePath) {}

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
  void prePop(int data) {}
  void postPop(Data<int, int> elt) {
    setContext(getContext() - 1);
    //  if(getContext()==0) std::cerr << elt.toString() << " <<<< Pop!" <<
    //  std::endl;
  }
  void noPop(int data) {}

  bool pushCondition(int data) {
    if (data > 0) {
      return true;
    }
    return false;
  }
  void prePush(Data<int, int> elt) {}
  void postPush(Data<int, int> elt) {
    // std::cout << "Push >>>> " << elt.toString() << std::endl;
  }
  void noPush(int data) {}

  void reportStack() {}
};

/*==============================================================================
  Instantiation of a comparison
==============================================================================*/

class Comparison : public CompareStacks<int, int> {
public:
  Comparison(std::string filePath) : CompareStacks<int, int>(filePath) {}

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
    if (getContext() > 0) {
      return true;
    }
    return false;
  }
  void prePop(int data) {}
  void postPop(Data<int, int> elt) {
    setContext(getContext() - 1);
    //  if(getContext()==0) std::cerr << elt.toString() << " <<<< Pop!" <<
    //  std::endl;
  }
  void noPop(int data) {}

  bool pushCondition(int data) {
    if (data > 0) {
      return true;
    }
    return false;
  }
  void prePush(Data<int, int> elt) {}
  void postPush(Data<int, int> elt) {
    // std::cout << "Push >>>> " << elt.toString() << std::endl;
  }
  void noPush(int data) {}

  void reportStack() {}
};

/*==============================================================================
  Test functions
==============================================================================*/
void test(std::string filePath, int code2) {

  switch (code2) {
  case 0: {
    Instance stack(filePath);
    // Test on normal stack
    stack.run();
    // testNS.println();
    break;
  }
  case 1: {
    convexHull ch(filePath);
    ch.run();
    ch.println();
    break;
  }
  case 2: {
    comparisonConvexHull chComp(filePath);
    chComp.runCompare();
    chComp.println();
    break;
  }
  default: {
    std::cout << "WRONG PROGRAM CODE";
    exit(-1);
    break;
  }
  }
}

/*==============================================================================
  Main function
==============================================================================*/
int main(int argc, char *argv[]) {

  // argv[0] is the file name, argv[1] is the code of what is to be done, 0 run
  // the example contained in the file, others are create input: 1 push only, 2
  // CT, 3 CH
  createTestInput ct = createTestInput();
  std::string filename = argv[1];

  //  std::cout<<argv[1]<<" "<<argv[2]<<" "<<argv[3]<<std::endl;

  switch (atoi(argv[2])) {
  case 0:
    test(filename, atoi(argv[3])); // in this case the second code holds the
                                   // type of problem being run 0 pushonly/CT 1
                                   // CH 2 CH(comp)
    break;
  case 1:
    ct.createTestInputFiles(0, atoi(argv[3]), filename, atoi(argv[4]),
                            atoi(argv[5]), atoi(argv[6]), atoi(argv[7]),
                            atof(argv[8]));

    break;
  case 2:
    ct.createTestInputFiles(1, atoi(argv[3]), filename, atoi(argv[4]),
                            atoi(argv[5]));

    break;
  case 3:
    ct.createTestInputFiles(2, atoi(argv[3]), filename, atoi(argv[4]),
                            atoi(argv[5]), atoi(argv[6]), atoi(argv[7]));

    break;
  default:
    std::cout << "WRONG PROGRAM CODE";
    exit(-1);
    break;
  }
  return 0;
}
