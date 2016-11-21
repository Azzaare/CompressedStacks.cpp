// ConvexHull : Definition
#ifndef CONVEXHULLEXTRAS
#define CONVEXHULLEXTRAS

/*==============================================================================
  Includes
==============================================================================*/
#include "../../../extras/stackAlgoExtras.hpp"
#include "point2D.hpp"

/*==============================================================================
  Empty type for the context (empty for the Convex Hull problem)
==============================================================================*/
class emptyContext {};

/*==============================================================================
  Instantiation of a comparison
==============================================================================*/

template <class I>
class ConvexHullExtras : public StackAlgoExtras<emptyContext, Point2D, I> {
public:
  ConvexHullExtras(std::string filePath, bool usecompressed, bool useclassic)
      : StackAlgoExtras<emptyContext, Point2D, I>(filePath, usecompressed,
                                                  useclassic) {}

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
template <class I>
Point2D ConvexHullExtras<I>::readInput(std::vector<std::string> line) {
  double x = std::stof(line[0]);
  double y = std::stof(line[1]);

  Point2D p(x, y);

  std::cout << "I JUST READ " << p << std::endl;

  return p;
}

template <class I>
std::shared_ptr<emptyContext> ConvexHullExtras<I>::initStack() {
  std::cout << "going to read two values " << std::endl;

  // first, read and push two values
  StackAlgoExtras<emptyContext, Point2D, I>::readPush(2);

  StackAlgoExtras<emptyContext, Point2D, I>::printCompare();

  std::cout << "done reading two values " << std::endl;
  // then initialize context (which in this case is NULL everything
  std::shared_ptr<emptyContext> context;
  return context;
}

template <class I> bool ConvexHullExtras<I>::popCondition(Point2D last) {
  Point2D minus1, minus2;

  std::cout << last << " <<<< pop condition enter " << std::endl;

  // read the two previous elements
  minus1 = StackAlgoExtras<emptyContext, Point2D, I>::top(1).getData();
  minus2 = StackAlgoExtras<emptyContext, Point2D, I>::top(2).getData();

  std::cout << last << " <<<< pop condition read two before " << minus1
            << minus2 << std::endl;

  if (Point2D::orientation(minus2, minus1, last) == 2) {
    std::cout << last << " <<<< pop condition returning true " << std::endl;

    return true;
  }
  std::cout << last << " <<<< pop condition returning false " << std::endl;

  return false;
}
template <class I> void ConvexHullExtras<I>::prePop(Point2D data) {}
template <class I>
void ConvexHullExtras<I>::postPop(Point2D data,
                                  Data<emptyContext, Point2D, I> elt) {
  std::cout << elt.getData() << " <<<< Pop!" << std::endl;
}
template <class I> void ConvexHullExtras<I>::noPop(Point2D data) {}

template <class I> bool ConvexHullExtras<I>::pushCondition(Point2D data) {
  std::cout << data << " <<<< push condition returning true " << std::endl;
  return true;
}
template <class I>
void ConvexHullExtras<I>::prePush(Data<emptyContext, Point2D, I> elt) {}
template <class I>
void ConvexHullExtras<I>::postPush(Data<emptyContext, Point2D, I> elt) {
  std::cout << "ConvexHullStackAlgo::pushAction Nothing to see here "
            << elt.getData() << std::endl;
}
template <class I> void ConvexHullExtras<I>::noPush(Point2D data) {}

template <class I> void ConvexHullExtras<I>::reportStack() {}

#endif // CONVEXHULLEXTRAS
