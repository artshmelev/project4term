#include "structures.h"

int Point::getX() const {
    return x;
}

int Point::getY() const {
    return y;
}

void Point::setX(int x1) {
    x = x1;
}

void Point::setY(int y1) {
    y = y1;
}
/*
bool Point::operator<(const Point &p1) const {
    return (y < p1.getY() ||
            y == p1.getY() && x < p1.getX());
}*/

std::vector<Point*> Triangle::getPoints() {
    std::vector<Point*> pnts;
    pnts.push_back(point1);
    pnts.push_back(point2);
    pnts.push_back(point3);
    return pnts;
}
