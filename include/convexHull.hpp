#include "problem.hpp"
#include "compare.hpp"
#include "Point2D.hpp"


/*==============================================================================
  Empty type for the context (empty for the Convex Hull problem)
==============================================================================*/
class emptyContext{};

/*==============================================================================
  Instantiation of a problem
==============================================================================*/
class convexHull : public Problem<emptyContext, Point2D> {
public:
    convexHull(std::string filePath) : Problem<emptyContext, Point2D>(filePath) {}

private:
    // Functions to run the stack
    Point2D readInput(std::vector<std::string> line);

    std::shared_ptr<emptyContext> initStack();

    bool popCondition(Point2D data);

    void popAction(Data<emptyContext, Point2D> elt);

    bool pushCondition(Point2D data);

    void pushAction(Data<emptyContext, Point2D> elt);
};

/*==============================================================================
  Instantiation of a comparison
==============================================================================*/

class comparisonConvexHull : public CompareStacks<emptyContext, Point2D> {
public:
    comparisonConvexHull(std::string filePath) : CompareStacks<emptyContext, Point2D>(filePath) {}

private:
    // Functions to run the stack
    Point2D readInput(std::vector<std::string> line);

    std::shared_ptr<emptyContext> initStack();

    bool popCondition(Point2D data);

    void popAction(Data<emptyContext, Point2D> elt);

    bool pushCondition(Point2D data);

    void pushAction(Data<emptyContext, Point2D> elt);
};
