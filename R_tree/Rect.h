#ifndef RECT_H
#define RECT_H

#include <algorithm>
#include <cmath>

class Rect
{
public:
    double xmin;
    double ymin;
    double xmax;
    double ymax;
    Rect();
    Rect(double x1, double y1, double x2, double y2);

    double area() const;
    bool contains(double x, double y) const;
    bool intersects(const Rect& other) const;

    static Rect combine(const Rect& a, const Rect& b);

    double enlargement(const Rect& other) const;
    double centerX() const;
    double centerY() const;
};

#endif