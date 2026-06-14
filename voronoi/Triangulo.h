#ifndef TRIANGULO_H
#define TRIANGULO_H

#include <cmath>
#include <vector>
#include "Punto.h"
#include "Arista.h"

class Triangulo {
public:
    Punto a, b, c;

    Triangulo(Punto a, Punto b, Punto c)
        : a(a), b(b), c(c) {}

    std::vector<Arista> obtenerAristas() const {
        return {
            Arista(a,b),
            Arista(b,c),
            Arista(c,a)
        };
    }
    Punto circuncentro() const {
        double d = 2 * (
            a.x * (b.y - c.y) +
            b.x * (c.y - a.y) +
            c.x * (a.y - b.y)
        );

        double ux =
            ((a.x * a.x + a.y * a.y) * (b.y - c.y) +
             (b.x * b.x + b.y * b.y) * (c.y - a.y) +
             (c.x * c.x + c.y * c.y) * (a.y - b.y))
            / d;

        double uy =
            ((a.x * a.x + a.y * a.y) * (c.x - b.x) +
             (b.x * b.x + b.y * b.y) * (a.x - c.x) +
             (c.x * c.x + c.y * c.y) * (b.x - a.x))
            / d;

        return Punto(ux, uy);
    }

    double radioCircuncirculo() const {
        Punto centro = circuncentro();

        return std::sqrt(
            (centro.x - a.x) * (centro.x - a.x) +
            (centro.y - a.y) * (centro.y - a.y)
        );
    }

    bool contiene(Punto p) const {
        Punto centro = circuncentro();
        double radio = radioCircuncirculo();

        double distancia =
            std::sqrt(
                (p.x - centro.x) * (p.x - centro.x) +
                (p.y - centro.y) * (p.y - centro.y)
            );

        return distancia <= radio;
    }

    bool contieneEnCircuncirculo(Punto p) const {
        Punto centro = circuncentro();
        double r =
            sqrt(
                pow(a.x-centro.x,2)
                + pow(a.y-centro.y,2)
            );

        double d =
            sqrt(
                pow(p.x-centro.x,2)
                + pow(p.y-centro.y,2)
            );

        return d <= r;
    }
    bool comparteVertice(const Punto& p) const {
        return a==p || b==p || c==p;
    }
};

#endif