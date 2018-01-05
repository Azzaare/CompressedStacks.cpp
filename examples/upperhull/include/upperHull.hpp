// UpperHull : Definition
#ifndef UPPERHULL
#define UPPERHULL

/*==============================================================================
  Includes
==============================================================================*/
#include "../../../include/stackAlgo.hpp"
#include "point2D.hpp"

double pops = 0;
double total = 0;

/*==============================================================================
  Empty type for the context (empty for the Convex Hull problem)
==============================================================================*/
class emptyContext {};

/*==============================================================================
  Instantiation of a problem
==============================================================================*/
template <class I>
class UpperHull : public StackAlgo<emptyContext, Point2D, I> {
public:
  UpperHull(std::string filePath)
      : StackAlgo<emptyContext, Point2D, I>(filePath) {}

private:
  // Functions to run the stack
  Point2D readInput(std::vector<std::string> line);
  std::shared_ptr<emptyContext> initStack();

  bool popCondition(Point2D data);
  void prePop(Point2D data);
  void postPop(Point2D data, Data<emptyContext, Point2D, I> elt);
  void noPop(Point2D data);

  bool pushCondition(Point2D data);
  void prePush(Data<emptyContext, Point2D, I> elt);
  void postPush(Data<emptyContext, Point2D, I> elt);
  void noPush(Point2D data);

  void reportStack();
};

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
template <class I>
Point2D UpperHull<I>::readInput(std::vector<std::string> line) {
  double x = std::stof(line[0]);
  double y = std::stof(line[1]);

  Point2D p(x, y);

  // std::cout << "I JUST READ " << p << std::endl;
  return p;
}

template <class I> std::shared_ptr<emptyContext> UpperHull<I>::initStack() {

  std::cout << "going to read two values " << std::endl;

  // first, read and push two values
  StackAlgo<emptyContext, Point2D, I>::readPush(2);

  std::cout << "done reading two values " << std::endl;

  // then initialize context (which in this case is NULL everything
  std::shared_ptr<emptyContext> context;
  return context;
}

template <class I> bool UpperHull<I>::popCondition(Point2D last) {
  Point2D minus1, minus2;
  total++;
  std::cout << std::endl << last << " <<<< pop condition enter " << std::endl;
  StackAlgo<emptyContext, Point2D, I>::println();

  // read the two previous elements
  minus1 = StackAlgo<emptyContext, Point2D, I>::top(1).getData();
  if (StackAlgo<emptyContext, Point2D, I>::mStack->getBufferLength() < 2) {
    return true;
  }

  minus2 = StackAlgo<emptyContext, Point2D, I>::top(2).getData();

  std::cout << last << " <<<< pop condition read two before " << minus2
            << minus1 << std::endl;

  if (Point2D::orientation(minus2, minus1, last) == 1) {
    pops++;
    std::cout << last
              << " <<<<                                                        "
                 "                 pop condition returning true "
              << pops / total << " tot " << total << std::endl;

    return true;
  }
  std::cout << last << "                                                       "
                       "                  <<<< pop condition returning false "
            << pops / total << " tot " << total << std::endl;

  return false;
}
template <class I> void UpperHull<I>::prePop(Point2D data) {}
template <class I>
void UpperHull<I>::postPop(Point2D data, Data<emptyContext, Point2D, I> elt) {
  std::cout << elt.getData() << " <<<< (post-)Pop!" << std::endl;
}
template <class I> void UpperHull<I>::noPop(Point2D data) {}

template <class I> bool UpperHull<I>::pushCondition(Point2D data) {
  std::cout << data << " <<<< push condition returning true " << std::endl;
  return true;
}
template <class I>
void UpperHull<I>::prePush(Data<emptyContext, Point2D, I> elt) {}
template <class I>
void UpperHull<I>::postPush(Data<emptyContext, Point2D, I> elt) {
  std::cout << "UpperHullStackAlgo::pushAction Nothing to see here "
            << elt.getData() << std::endl;
}
template <class I> void UpperHull<I>::noPush(Point2D data) {}

template <class I> void UpperHull<I>::reportStack() {}

#endif // UPPERHULL
