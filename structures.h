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
    Edge(Point p1, Point p2) : point1(p1), point2(p2) {}

private:
    Point point1, point2;
    Triangle *triangle1, *triangle2;
};

class Triangle {
public:
    Triangle(Point *p1, Point *p2, Point *p3) :
            point1(p1), point2(p2), point3(p3) {}
    std::vector<Point*> getPoints();

private:
    Point *point1, *point2, *point3;
    //Edge *edge1, *edge2, *edge3;
};

#endif // STRUCTURES_H
