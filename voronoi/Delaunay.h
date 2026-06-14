#ifndef DELAUNAY_H
#define DELAUNAY_H

#include <vector>
#include <algorithm>
#include "Triangulo.h"

class Delaunay {
public:
    std::vector<Triangulo> triangulos;
    void construir(std::vector<Punto>& puntos);
    std::vector<Triangulo> obtenerTriangulosInvalidos(const Punto& nuevoPunto);

    std::vector<Arista> construirCavidad(
        const std::vector<Triangulo>& invalidos
    );

    void eliminarTriangulos(
        const std::vector<Triangulo>& invalidos
    );

    void retriangular(
        const Punto& nuevoPunto,
        const std::vector<Arista>& cavidad
    );

    bool aristaCompartida(
        const Arista& arista,
        const std::vector<Triangulo>& invalidos
    );
};
#endif