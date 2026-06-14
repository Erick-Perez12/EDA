#include "Voronoi.h"

bool Voronoi::sonVecinos(
    const Triangulo& t1,
    const Triangulo& t2
){
    int compartidos = 0;
    std::vector<Punto> p1 ={
        t1.a, t1.b, t1.c
    };

    std::vector<Punto> p2 ={
        t2.a, t2.b, t2.c
    };

    for(auto& a : p1){
        for(auto& b : p2){
            if(a == b){
                compartidos++;
            }
        }
    }
    return compartidos == 2;
}

void Voronoi::construir(
    const std::vector<Triangulo>& triangulos
)
{
    vertices.clear();
    aristas.clear();

    std::vector<Punto> centros;
    for(const auto& t : triangulos)
    {
        Punto centro =
            t.circuncentro();

        centros.push_back(centro);

        vertices.push_back(centro);
    }

    for(size_t i=0;
        i<triangulos.size();
        i++)
    {
        for(size_t j=i+1;
            j<triangulos.size();
            j++)
        {
            if(
                sonVecinos(
                    triangulos[i],
                    triangulos[j]
                )
            )
            {
                aristas.push_back(
                    Arista(
                        centros[i],
                        centros[j]
                    )
                );
            }
        }
    }
}