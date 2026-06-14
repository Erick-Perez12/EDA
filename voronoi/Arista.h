#ifndef ARISTA_H
#define ARISTA_H

#include "Punto.h"

class Arista {
public:
    Punto p1;
    Punto p2;

    Arista(Punto p1, Punto p2)
        : p1(p1), p2(p2) {}

    bool operator==(const Arista& otra) const {
        return (
            (p1.x == otra.p1.x && p1.y == otra.p1.y &&
             p2.x == otra.p2.x && p2.y == otra.p2.y)
            ||
            (p1.x == otra.p2.x && p1.y == otra.p2.y &&
             p2.x == otra.p1.x && p2.y == otra.p1.y)
        );
    }
};

#endif