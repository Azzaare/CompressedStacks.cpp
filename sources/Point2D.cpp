//
// Created by yago on 16/07/05.
//

#include "Point2D.h"

Point2D::Point2D() {}

Point2D::Point2D(double xi, double yi) {
    this->x=xi;
    this->y=yi;
}

Point2D::Point2D(const Point2D &other) {
    this->x=other.x;
    this->y=other.y;
}

Point2D::~Point2D() {
}

void Point2D::Set(const double xi, const double yi) {
    this->x=xi;
    this->y=yi;
}

void Point2D::SetX(double xi) {
    this->x=xi;
}

void Point2D::SetY(double yi) {
    this->y=yi;
}

double Point2D::GetX() const {
    return x;
}

double Point2D::GetY() const {
    return y;
}

//std::string Point2D::ToString() const {
//    return std::string("%f",x)
//}

void Point2D::operator=(const Point2D &other) {
    x=other.x;
    y=other.y;
}

bool Point2D::operator==(const Point2D &other) const {
    return (x==other.x)&&(y==other.y);
}

bool Point2D::operator!=(const Point2D &other) const {
    return !(*this==other);
}

// x order
bool Point2D::operator>(const Point2D &other) const {
    if(x!=other.x )return x>other.x;
    else return y>other.y;//equal x
}

bool Point2D::operator < (const Point2D &other) const {
    //std::cout<<" comparing "<<x<<" and "<<other.x<<" output "<<(x < other.x)<<std::endl;
    if(x!=other.x ) return x < other.x;
    else return y<other.y;//equal x
}

bool Point2D::operator>=(const Point2D &other) const {
    return (*this==other)||(*this>other);
}

bool Point2D::operator<=(const Point2D &other) const {
    return (*this==other)||(*this<other);
}
