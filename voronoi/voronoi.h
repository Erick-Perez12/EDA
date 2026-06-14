#ifndef VORONOI_H
#define VORONOI_H

#include <vector>
#include "Triangulo.h"
#include "Arista.h"

class Voronoi{
public:
    std::vector<Punto> vertices;
    std::vector<Arista> aristas;
    void construir(
        const std::vector<Triangulo>& triangulos
    );

private:
    bool sonVecinos(
        const Triangulo& t1,
        const Triangulo& t2
    );
};

#endif