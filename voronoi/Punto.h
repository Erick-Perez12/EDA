#ifndef PUNTO_H
#define PUNTO_H

class Punto {
public:
    double x;
    double y;

    Punto() : x(0), y(0) {}

    Punto(double x, double y)
        : x(x), y(y) {}

    bool operator==(const Punto& otro) const {
        return x == otro.x && y == otro.y;
    }
};

#endif