// ConvexHull : Definition
#ifndef CONVEXHULL
#define CONVEXHULL

/*==============================================================================
  Includes
==============================================================================*/
//#include "../../../include/stackAlgo.hpp" // Not necessary with compare.hpp
#include "../../../include/compare.hpp"
#include "point2D.hpp"

/*==============================================================================
  Empty type for the context (empty for the Convex Hull problem)
==============================================================================*/
class emptyContext {};

/*==============================================================================
  Instantiation of a problem
==============================================================================*/
class ConvexHull : public StackAlgo<emptyContext, Point2D> {
public:
  ConvexHull(std::string filePath, bool useclassic = false)
      : StackAlgo<emptyContext, Point2D>(filePath, useclassic) {}

private:
  // Functions to run the stack
  Point2D readInput(std::vector<std::string> line);
  std::shared_ptr<emptyContext> initStack();

  bool popCondition(Point2D data);
  void prePop(Point2D data);
  void postPop(Point2D data, Data<emptyContext, Point2D> elt);
  void noPop(Point2D data);

  bool pushCondition(Point2D data);
  void prePush(Data<emptyContext, Point2D> elt);
  void postPush(Data<emptyContext, Point2D> elt);
  void noPush(Point2D data);

  void reportStack();
};

/*==============================================================================
  Instantiation of a comparison
==============================================================================*/

class ComparisonConvexHull : public CompareStacks<emptyContext, Point2D> {
public:
  ComparisonConvexHull(std::string filePath)
      : CompareStacks<emptyContext, Point2D>(filePath) {}

private:
  // Functions to run the stack
  Point2D readInput(std::vector<std::string> line);
  std::shared_ptr<emptyContext> initStack();

  bool popCondition(Point2D data);
  void prePop(Point2D data);
  void postPop(Point2D data, Data<emptyContext, Point2D> elt);
  void noPop(Point2D data);

  bool pushCondition(Point2D data);
  void prePush(Data<emptyContext, Point2D> elt);
  void postPush(Data<emptyContext, Point2D> elt);
  void noPush(Point2D data);

  void reportStack();
};

#endif // CONVEXHULL
