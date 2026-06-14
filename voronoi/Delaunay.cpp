#include "Delaunay.h"

#include <iostream>
#include <algorithm>

using namespace std;

vector<Triangulo> Delaunay::obtenerTriangulosInvalidos(
    const Punto& nuevoPunto
)
{
    vector<Triangulo> invalidos;
    for(const auto& t : triangulos)
    {
        if(t.contieneEnCircuncirculo(nuevoPunto))
        {
            invalidos.push_back(t);
        }
    }
    return invalidos;
}

bool Delaunay::aristaCompartida(
    const Arista& arista,
    const vector<Triangulo>& invalidos
)
{
    int contador = 0;

    for(const auto& t : invalidos)
    {
        vector<Arista> aristas = t.obtenerAristas();

        for(const auto& e : aristas)
        {
            if(e == arista)
            {
                contador++;
            }
        }
    }

    return contador > 1;
}

vector<Arista> Delaunay::construirCavidad(
    const vector<Triangulo>& invalidos
)
{
    vector<Arista> frontera;

    for(const auto& t : invalidos)
    {
        vector<Arista> aristas =
            t.obtenerAristas();

        for(const auto& e : aristas)
        {
            if(!aristaCompartida(
                    e,
                    invalidos))
            {
                frontera.push_back(e);
            }
        }
    }

    return frontera;
}

void Delaunay::eliminarTriangulos(
    const vector<Triangulo>& invalidos
)
{
    for(const auto& malo : invalidos)
    {
        triangulos.erase(
            remove_if(
                triangulos.begin(),
                triangulos.end(),
                [&](const Triangulo& t)
                {
                    return
                        t.a == malo.a &&
                        t.b == malo.b &&
                        t.c == malo.c;
                }
            ),
            triangulos.end()
        );
    }
}

void Delaunay::retriangular(
    const Punto& nuevoPunto,
    const vector<Arista>& cavidad
)
{
    for(const auto& arista : cavidad)
    {
        triangulos.push_back(
            Triangulo(
                arista.p1,
                arista.p2,
                nuevoPunto
            )
        );
    }
}

void Delaunay::construir(
    vector<Punto>& puntos
)
{
    double M = 10000;

    Punto p1(-M,-M);
    Punto p2( M,-M);
    Punto p3(0,M);

    Triangulo super(
        p1,
        p2,
        p3
    );

    triangulos.push_back(super);

    for(const Punto& punto : puntos)
    {
        cout << "\n=========================\n";
        cout << "Insertando punto ("
             << punto.x
             << ", "
             << punto.y
             << ")\n";

        // ACTIVIDAD 4
        auto invalidos =
            obtenerTriangulosInvalidos(
                punto
            );

        cout
            << "Triangulos eliminados: "
            << invalidos.size()
            << endl;

        // ACTIVIDAD 5
        auto cavidad =
            construirCavidad(
                invalidos
            );

        cout
            << "Aristas frontera: "
            << cavidad.size()
            << endl;

        eliminarTriangulos(
            invalidos
        );

        // ACTIVIDAD 6
        retriangular(
            punto,
            cavidad
        );

        cout
            << "Triangulos actuales: "
            << triangulos.size()
            << endl;
    }

    triangulos.erase(
        remove_if(
            triangulos.begin(),
            triangulos.end(),
            [&](const Triangulo& t)
            {
                return
                    t.comparteVertice(p1) ||
                    t.comparteVertice(p2) ||
                    t.comparteVertice(p3);
            }
        ),
        triangulos.end()
    );

    cout << "\n=========================\n";
    cout << "Triangulacion final: "
         << triangulos.size()
         << " triangulos\n";
}