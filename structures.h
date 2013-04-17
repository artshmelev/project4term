#ifndef STRUCTURES_H
#define STRUCTURES_H

#include <vector>

//class Triangle;

class Point {
public:
    Point() : x(0), y(0), t(0) {}
    Point(float x1, float y1, float t1) : x(x1), y(y1), t(t1) {}
    float getX() const;
    float getY() const;
    float getT() const;
    void setX(float x1);
    void setY(float y1);
    void setT(float t1);

    //bool operator<(const Point &p1) const;

private:
    float x, y, t;
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

inline float Point::getX() const {
    return x;
}

inline float Point::getY() const {
    return y;
}

inline float Point::getT() const {
    return t;
}

inline void Point::setX(float x1) {
    x = x1;
}

inline void Point::setY(float y1) {
    y = y1;
}

inline void Point::setT(float t1) {
    t = t1;
}

inline Point* Edge::getPoint1() const {
    return point1;
}

inline Point* Edge::getPoint2() const {
    return point2;
}

#endif // STRUCTURES_H
