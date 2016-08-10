//
// Created by yago on 16/08/04.
//

#include "../include/convexHull.hpp"

Point2D convexHull::readInput(std::vector<std::string> line) {
double x = std::stof(line[0]);
double y = std::stof(line[1]);

    Point2D p(x,y);

    std::cout<<"I JUST READ "<<p<<std::endl;

//setContext(std::stoi(line[1]));
return p;
}

std::shared_ptr<emptyContext> convexHull::initStack() {

    std::cout<<"going to read two values "<<std::endl;

    //first, read and push two values
    readPush(2);

    std::cout<<"done reading two values "<<std::endl;

    // then initialize context (which in this case is NULL everything
    std::shared_ptr<emptyContext> context;
    return context;
}
bool convexHull::popCondition(Point2D last) {
    Point2D minus1,minus2;

    std::cout << last << " <<<< pop condition enter " << std::endl;

    // read the two previous elements
    minus1=top(1).getData();
    minus2=top(2).getData();

    std::cout << last << " <<<< pop condition read two befoer "<<minus1<<minus2 << std::endl;


    if (Point2D::orientation(minus2,minus1,last)==2 ) {
        std::cout << last << " <<<< pop condition returning true " << std::endl;

        return true;
    }
    std::cout << last << " <<<< pop condition returning false " << std::endl;

    return false;
}
void convexHull::popAction(Data<emptyContext, Point2D> elt) {
     std::cout << elt.getData() << " <<<< Pop!" << std::endl;
   // setContext(getContext() );
}
bool convexHull::pushCondition(Point2D data) {
    std::cout << data << " <<<< push condition returning true " << std::endl;
        return true;
}
void convexHull::pushAction(Data<emptyContext, Point2D> elt) {
    std::cout << "convexHullProblem::pushAction Nothing to see here "<<elt.getData()<< std::endl;
}


// copy the same functions again to be able to perform comparisons
Point2D comparisonConvexHull::readInput(std::vector<std::string> line) {
double x = std::stof(line[0]);
double y = std::stof(line[1]);

    Point2D p(x,y);

    std::cout<<"I JUST READ "<<p<<std::endl;

//setContext(std::stoi(line[1]));
return p;
}

std::shared_ptr<emptyContext> comparisonConvexHull::initStack() {

    std::cout<<"going to read two values "<<std::endl;

    //first, read and push two values
    readPush(2);

    std::cout<<"done reading two values "<<std::endl;

    // then initialize context (which in this case is NULL everything
    std::shared_ptr<emptyContext> context;
    return context;
}

bool comparisonConvexHull::popCondition(Point2D last) {
    Point2D minus1,minus2;

    std::cout << last << " <<<< pop condition enter " << std::endl;

    // read the two previous elements
    minus1=top(1).getData();
    minus2=top(2).getData();

    std::cout << last << " <<<< pop condition read two befoer "<<minus1<<minus2 << std::endl;


    if (Point2D::orientation(minus2,minus1,last)==2 ) {
        std::cout << last << " <<<< pop condition returning true " << std::endl;

        return true;
    }
    std::cout << last << " <<<< pop condition returning false " << std::endl;

    return false;
}

void comparisonConvexHull::popAction(Data<emptyContext, Point2D> elt) {
     std::cout << elt.getData() << " <<<< Pop!" << std::endl;
   // setContext(getContext() );
}
bool comparisonConvexHull::pushCondition(Point2D data) {
    std::cout << data << " <<<< push condition returning true " << std::endl;
        return true;
}
void comparisonConvexHull::pushAction(Data<emptyContext, Point2D> elt) {
    std::cout << "convexHullProblem::pushAction Nothing to see here "<<elt.getData()<< std::endl;
}
