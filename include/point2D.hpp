//
// Created by yago on 16/07/05.
//

#include <iostream>
#include <string>


#ifndef SMARTSTACK_POINT2D
#define SMARTSTACK_POINT2D

class Point2D {

public:
  double x;
  double y;

  Point2D();
  Point2D(double x, double y);
  // copy constructor
  Point2D(const Point2D &other);

  ~Point2D();

  void Set(const double x, const double y);

  void SetX(double x);
  void SetY(double y);
  double GetX() const;
  double GetY() const;

  //std::string toString(Point2D&) ;

  static int orientation(Point2D p1, Point2D p2, Point2D p3);

  // assignment operator
  void operator=(const Point2D &other);

  bool operator==(const Point2D &other) const;
  bool operator!=(const Point2D &other) const;
  bool operator>(const Point2D &other) const;
  bool operator<(const Point2D &other) const;
  bool operator>=(const Point2D &other) const;
  bool operator<=(const Point2D &other) const;

  void write(std::ostream& os);

  friend std::ostream& operator<<(std::ostream& os,Point2D p)
  {
      p.write(os);
      return os;
  }

};

#endif // SMARTSTACK_POINT2D
