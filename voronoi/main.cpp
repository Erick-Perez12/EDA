#include <chrono>
#include <fstream>
#include <iostream>
#include <string>
#include <vector>
#include "voronoi.h"
#include "Delaunay.h"
#include "Triangulo.h"

using namespace std;
using namespace std::chrono;

void mostrarTitulo(const string& titulo)
{
    cout << "\n--- " << titulo << " ---\n";
}

vector<Punto> cargarPuntosDesdeArchivo(const string& nombreArchivo)
{
    vector<Punto> puntos;
    ifstream archivo(nombreArchivo);

    if(!archivo.is_open())
    {
        cout << "Error al abrir " << nombreArchivo << '\n';
        return puntos;
    }

    double x, y;

    while(archivo >> x >> y)
    {
        puntos.push_back(Punto(x, y));
    }

    return puntos;
}

void ejecutarActividad1y2()
{
    mostrarTitulo("Actividad 1 y 2");

    Triangulo t(
        Punto(0, 0),
        Punto(10, 0),
        Punto(5, 10)
    );

    Punto centro = t.circuncentro();

    cout << "Circuncentro: ("
         << centro.x << ", "
         << centro.y << ")\n";
    cout << "Radio: "
         << t.radioCircuncirculo()
         << '\n';

    Punto p(5, 5);

    if(t.contiene(p))
    {
        cout << "El punto esta dentro del circuncirculo\n";
    }
    else
    {
        cout << "El punto NO esta dentro del circuncirculo\n";
    }
}

void ejecutarActividad3()
{
    mostrarTitulo("Actividad 3");

    vector<Punto> puntos =
        cargarPuntosDesdeArchivo("puntos.txt");

    if(puntos.empty())
    {
        cout << "No se pudieron cargar puntos para la actividad 3\n";
        return;
    }

    Delaunay delaunay;
    delaunay.construir(puntos);

    cout << "Triangulos generados:\n\n";

    for(const auto& t : delaunay.triangulos)
    {
        cout
            << "(" << t.a.x << "," << t.a.y << ") "
            << "(" << t.b.x << "," << t.b.y << ") "
            << "(" << t.c.x << "," << t.c.y << ")\n";
    }
}

void ejecutarActividad4a6()
{
    mostrarTitulo("Actividad 4, 5 y 6");

    vector<Punto> puntos =
        cargarPuntosDesdeArchivo("puntos.txt");

    if(puntos.empty())
    {
        cout << "No se pudieron cargar puntos para las actividades 4, 5 y 6\n";
        return;
    }

    cout << "Puntos cargados: "
         << puntos.size()
         << '\n';

    Delaunay delaunay;
    delaunay.construir(puntos);

    cout << "\n===== TRIANGULOS FINALES =====\n";

    int id = 1;

    for(const auto& t : delaunay.triangulos)
    {
        cout << "T" << id++ << ": "
             << "(" << t.a.x << "," << t.a.y << ") "
             << "(" << t.b.x << "," << t.b.y << ") "
             << "(" << t.c.x << "," << t.c.y << ")\n";
    }
}

long long medirTiempoVoronoiMicrosegundos(
    const vector<Punto>& puntosEntrada
)
{
    vector<Punto> puntos = puntosEntrada;

    auto inicio = high_resolution_clock::now();

    Delaunay delaunay;
    delaunay.construir(puntos);

    Voronoi voronoi;
    voronoi.construir(delaunay.triangulos);

    auto fin = high_resolution_clock::now();

    return duration_cast<microseconds>(
        fin - inicio
    ).count();
}

void compararArchivoPorTiempo(
    const string& nombreArchivo,
    int cantidadEsperada
)
{
    vector<Punto> puntos =
        cargarPuntosDesdeArchivo(nombreArchivo);

    if(puntos.empty())
    {
        cout << cantidadEsperada
             << " puntos: no se pudo leer "
             << nombreArchivo
             << '\n';
        return;
    }

    long long tiempo =
        medirTiempoVoronoiMicrosegundos(
            puntos
        );

    cout << cantidadEsperada
         << " puntos (" << nombreArchivo << "): "
         << tiempo
         << " us\n";
}

void ejecutarActividad7()
{
    mostrarTitulo("Actividad 7 - Diagrama de Voronoi");
    vector<Punto> puntos = cargarPuntosDesdeArchivo("puntos.txt");
    if(puntos.empty())
    {
        cout << "No se pudieron cargar puntos para la actividad 7\n";
        return;
    }
    Delaunay delaunay;
    delaunay.construir(puntos);
    Voronoi voronoi;
    voronoi.construir(
        delaunay.triangulos
    );

    cout << "\n===== VORONOI =====\n";
    cout << "Numero de vertices: "<< voronoi.vertices.size()<< '\n';
    cout << "Numero de aristas: "<< voronoi.aristas.size() << '\n';
    cout << "\n===== VERTICES VORONOI =====\n";

    for(size_t i = 0; i < voronoi.vertices.size(); i++)
    {
        cout<< i + 1<< ": ("<< voronoi.vertices[i].x
            << ", "<< voronoi.vertices[i].y<< ")\n";
    }
    cout << "\n===== ARISTAS VORONOI =====\n";
    for(const auto& a : voronoi.aristas){
        cout<< "("<< a.p1.x<< ", "<< a.p1.y<< ") -> ("<< a.p2.x
            << ", "<< a.p2.y<< ")\n";
    }
}

void ejecutarActividad8()
{
    mostrarTitulo("Actividad 8 - Comparacion de tiempos");

    cout << "Tiempo de ejecucion en microsegundos:\n";

    compararArchivoPorTiempo(
        "puntos50.txt",
        50
    );
    compararArchivoPorTiempo(
        "puntos100.txt",
        100
    );
    compararArchivoPorTiempo(
        "puntos150.txt",
        150
    );
    compararArchivoPorTiempo(
        "puntos200.txt",
        200
    );
    compararArchivoPorTiempo(
        "puntos.250.txt",
        250
    );
}
int main()
{
    //ejecutarActividad1y2();
    //ejecutarActividad3();
    //ejecutarActividad4a6();
    //ejecutarActividad7();
    ejecutarActividad8();
    return 0;
}
