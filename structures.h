#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

class Triangle;

class Point {
public:
    Point() : x(0), y(0) {}
    Point(int x1, int y1) : x(x1), y(y1) {}
    int getX() const;
    int getY() const;
    void setX(int x1);
    void setY(int y1);

    //bool operator<(const Point &p1) const;

private:
    int x, y;
};

class Edge {
public:
    Edge(Point *p1, Point *p2) : point1(p1), point2(p2) {}

    Point* getPoint1() const;
    Point* getPoint2() const;

private:
    Point *point1, *point2;
    //Triangle *triangle1, *triangle2;
};

class Triangle {
public:
    Triangle(Point *p1, Point *p2, Point *p3) :
            point1(p1), point2(p2), point3(p3) {}

    std::vector<Point*> getPoints();
    std::vector<Edge> getEdges();

private:
    Point *point1, *point2, *point3;
    //Edge *edge1, *edge2, *edge3;
};

struct classCmp {
    bool operator()(Point *p1, Point *p2) {
        return p1->getY() < p2->getY() ||
                p1->getY() == p2->getY() && p1->getX() < p2->getX();
    }
};

inline int Point::getX() const {
    return x;
}

inline int Point::getY() const {
    return y;
}

inline void Point::setX(int x1) {
    x = x1;
}

inline void Point::setY(int y1) {
    y = y1;
}

inline Point* Edge::getPoint1() const {
    return point1;
}

inline Point* Edge::getPoint2() const {
    return point2;
}

#endif // STRUCTURES_H
