#include "problem.hpp"
#include "compare.hpp"
#include "Point2D.hpp"


/*==============================================================================
  Instantiation of a problem
==============================================================================*/
class convexHull : public Problem<std::nullptr_t, Point2D> {
public:
    convexHull(std::string filePath) : Problem<std::nullptr_t, Point2D>(filePath) {}

private:
    // Functions to run the stack
    Point2D readInput(std::vector<std::string> line);

    std::shared_ptr<std::nullptr_t> initStack();

    bool popCondition(Point2D data);

    void popAction(Data<std::nullptr_t, Point2D> elt);

    bool pushCondition(Point2D data);

    void pushAction(Data<std::nullptr_t, Point2D> elt);
};

/*==============================================================================
  Instantiation of a comparison
==============================================================================*/

class comparisonConvexHull : public CompareStacks<int, int> {
public:
    comparisonConvexHull(std::string filePath) : CompareStacks<int, int>(filePath) {}

private:
    // Functions to run the stack
    int readInput(std::vector<std::string> line);

    std::shared_ptr<int> initStack();

    bool popCondition(int data);

    void popAction(Data<int, int> elt);

    bool pushCondition(int data);

    void pushAction(Data<int, int> elt);

};