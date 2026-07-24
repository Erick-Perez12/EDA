#include "Rect.h"

Rect::Rect()
{
    xmin = ymin = xmax = ymax = 0.0;
}

Rect::Rect(double x1, double y1,
           double x2, double y2)
{
    xmin = std::min(x1, x2);
    ymin = std::min(y1, y2);

    xmax = std::max(x1, x2);
    ymax = std::max(y1, y2);
}

double Rect::area() const
{
    return (xmax - xmin) * (ymax - ymin);
}

bool Rect::contains(double x, double y) const
{
    return x >= xmin &&
           x <= xmax &&
           y >= ymin &&
           y <= ymax;
}

bool Rect::intersects(const Rect& other) const
{
    if (xmax < other.xmin) return false;
    if (xmin > other.xmax) return false;

    if (ymax < other.ymin) return false;
    if (ymin > other.ymax) return false;

    return true;
}

Rect Rect::combine(const Rect& a,
                   const Rect& b)
{
    return Rect(
        std::min(a.xmin, b.xmin),
        std::min(a.ymin, b.ymin),
        std::max(a.xmax, b.xmax),
        std::max(a.ymax, b.ymax)
    );
}

double Rect::enlargement(const Rect& other) const
{
    Rect r = combine(*this, other);

    return r.area() - area();
}

double Rect::centerX() const
{
    return (xmin + xmax) / 2.0;
}

double Rect::centerY() const
{
    return (ymin + ymax) / 2.0;
}