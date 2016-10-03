// ConvexHull : Implementation

/*==============================================================================
  Includes
==============================================================================*/
#include "include/convexHull.hpp"

/*==============================================================================
  Stack functions to define the Convex Hull algorithm with Compressed Stack
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
Point2D ConvexHull::readInput(std::vector<std::string> line) {
  double x = std::stof(line[0]);
  double y = std::stof(line[1]);

  Point2D p(x, y);

  std::cout << "I JUST READ " << p << std::endl;
  return p;
}

std::shared_ptr<emptyContext> ConvexHull::initStack() {

  std::cout << "going to read two values " << std::endl;

  // first, read and push two values
  readPush(2);

  std::cout << "done reading two values " << std::endl;

  // then initialize context (which in this case is NULL everything
  std::shared_ptr<emptyContext> context;
  return context;
}

bool ConvexHull::popCondition(Point2D last) {
  Point2D minus1, minus2;

  std::cout << last << " <<<< pop condition enter " << std::endl;

  // read the two previous elements
  minus1 = top(1).getData();
  minus2 = top(2).getData();

  std::cout << last << " <<<< pop condition read two before " << minus1
            << minus2 << std::endl;

  if (Point2D::orientation(minus2, minus1, last) == 2) {
    std::cout << last << " <<<< pop condition returning true " << std::endl;

    return true;
  }
  std::cout << last << " <<<< pop condition returning false " << std::endl;

  return false;
}
void ConvexHull::prePop(Point2D data) {}
void ConvexHull::postPop(Point2D data, Data<emptyContext, Point2D> elt) {
  std::cout << elt.getData() << " <<<< Pop!" << std::endl;
}
void ConvexHull::noPop(Point2D data) {}

bool ConvexHull::pushCondition(Point2D data) {
  std::cout << data << " <<<< push condition returning true " << std::endl;
  return true;
}
void ConvexHull::prePush(Data<emptyContext, Point2D> elt) {}
void ConvexHull::postPush(Data<emptyContext, Point2D> elt) {
  std::cout << "ConvexHullStackAlgo::pushAction Nothing to see here "
            << elt.getData() << std::endl;
}
void ConvexHull::noPush(Point2D data) {}

void ConvexHull::reportStack() {}

/*==============================================================================
  Stack functions to define the Convex Hull algorithm with Stack comparison
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
Point2D ComparisonConvexHull::readInput(std::vector<std::string> line) {
  double x = std::stof(line[0]);
  double y = std::stof(line[1]);

  Point2D p(x, y);

  std::cout << "I JUST READ " << p << std::endl;

  return p;
}

std::shared_ptr<emptyContext> ComparisonConvexHull::initStack() {
  std::cout << "going to read two values " << std::endl;

  // first, read and push two values
  readPush(2);

  printCompare();

  std::cout << "done reading two values " << std::endl;
  // then initialize context (which in this case is NULL everything
  std::shared_ptr<emptyContext> context;
  return context;
}

bool ComparisonConvexHull::popCondition(Point2D last) {
  Point2D minus1, minus2;

  std::cout << last << " <<<< pop condition enter " << std::endl;

  // read the two previous elements
  minus1 = top(1).getData();
  minus2 = top(2).getData();

  std::cout << last << " <<<< pop condition read two before " << minus1
            << minus2 << std::endl;

  if (Point2D::orientation(minus2, minus1, last) == 2) {
    std::cout << last << " <<<< pop condition returning true " << std::endl;

    return true;
  }
  std::cout << last << " <<<< pop condition returning false " << std::endl;

  return false;
}
void ComparisonConvexHull::prePop(Point2D data) {}
void ComparisonConvexHull::postPop(Point2D data,
                                   Data<emptyContext, Point2D> elt) {
  std::cout << elt.getData() << " <<<< Pop!" << std::endl;
}
void ComparisonConvexHull::noPop(Point2D data) {}

bool ComparisonConvexHull::pushCondition(Point2D data) {
  std::cout << data << " <<<< push condition returning true " << std::endl;
  return true;
}
void ComparisonConvexHull::prePush(Data<emptyContext, Point2D> elt) {}
void ComparisonConvexHull::postPush(Data<emptyContext, Point2D> elt) {
  std::cout << "ConvexHullStackAlgo::pushAction Nothing to see here "
            << elt.getData() << std::endl;
}
void ComparisonConvexHull::noPush(Point2D data) {}

void ComparisonConvexHull::reportStack() {}

/*==============================================================================
  How to use
    * argv[1] is the file name
    * argv[2] is the code of what is to be done (is no space provided -> 0)
      * 0 is a convex hull problem with a classical stack
      * 1 is a convex hull problem with a compressed stack
      * 2 is a convex hull problem with both stacks comparison (check errors)
==============================================================================*/
int main(int argc, char *argv[]) {
  // Getting the path of the instance to test
  std::string filepath = argv[1];

  switch (atoi(argv[2])) {
  case 0: {
    ConvexHull stack(filepath, true);
    stack.run();
    stack.println();
    break;
  }
  case 1: {
    ConvexHull stack(filepath);
    stack.run();
    stack.println();
    break;
  }
  default: {
    ComparisonConvexHull stack(filepath);
    stack.runCompare();
    stack.println();
    break;
  }
  }
  return 0;
}
