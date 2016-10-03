// Point2D : Definition. To be used by ConvexHull.
//
#ifndef POINT2D
#define POINT2D

/*==============================================================================
  Includes
==============================================================================*/
#include <iostream>
#include <string>

class Point2D {

public:
  // Members : Coordinates
  double x;
  double y;

  // Constructor
  Point2D();
  Point2D(double x, double y);

  // copy constructor
  Point2D(const Point2D &other);

  // Destructor
  ~Point2D();

  void Set(const double x, const double y);

  void SetX(double x);
  void SetY(double y);
  double GetX() const;
  double GetY() const;

  static int orientation(Point2D p1, Point2D p2, Point2D p3);

  // assignment operator
  void operator=(const Point2D &other);

  bool operator==(const Point2D &other) const;
  bool operator!=(const Point2D &other) const;
  bool operator>(const Point2D &other) const;
  bool operator<(const Point2D &other) const;
  bool operator>=(const Point2D &other) const;
  bool operator<=(const Point2D &other) const;

  void write(std::ostream &os);

  friend std::ostream &operator<<(std::ostream &os, Point2D p) {
    p.write(os);
    return os;
  }
};

/*==============================================================================
  Methods implementation
==============================================================================*/
Point2D::Point2D() {}

Point2D::Point2D(double xi, double yi) {
  this->x = xi;
  this->y = yi;
}

Point2D::Point2D(const Point2D &other) {
  this->x = other.x;
  this->y = other.y;
}

Point2D::~Point2D() {}

void Point2D::Set(const double xi, const double yi) {
  this->x = xi;
  this->y = yi;
}

void Point2D::SetX(double xi) { this->x = xi; }

void Point2D::SetY(double yi) { this->y = yi; }

double Point2D::GetX() const { return x; }

double Point2D::GetY() const { return y; }

// To find orientation of ordered triplet (p1, p2, p3).
// The function returns following values
// 0 --> p, q and r are colinear
// 1 --> Clockwise
// 2 --> Counterclockwise
// taken from http://www.geeksforgeeks.org/orientation-3-ordered-points/
int Point2D::orientation(Point2D p1, Point2D p2, Point2D p3) {
  int val = (p2.y - p1.y) * (p3.x - p2.x) - (p2.x - p1.x) * (p3.y - p2.y);

  if (val == 0)
    return 0;               // colinear
  return (val > 0) ? 1 : 2; // clock or counterclock wise
}

void Point2D::operator=(const Point2D &other) {
  x = other.x;
  y = other.y;
}

bool Point2D::operator==(const Point2D &other) const {
  return (x == other.x) && (y == other.y);
}

bool Point2D::operator!=(const Point2D &other) const {
  return !(*this == other);
}

// x order
bool Point2D::operator>(const Point2D &other) const {
  if (x != other.x)
    return x > other.x;
  else
    return y > other.y; // equal x
}

bool Point2D::operator<(const Point2D &other) const {
  if (x != other.x)
    return x < other.x;
  else
    return y < other.y; // equal x
}

bool Point2D::operator>=(const Point2D &other) const {
  return (*this == other) || (*this > other);
}

bool Point2D::operator<=(const Point2D &other) const {
  return (*this == other) || (*this < other);
}

void Point2D::write(std::ostream &os) {
  os << "( " << x << " , " << y << " )" << std::endl;
}

#endif // POINT2D
