#include "Experiment3.h"

#include <iostream>
#include <iomanip>
#include <vector>

#include "Experiments.h"
#include "RTree.h"
#include "RStarTree.h"


using namespace std;

void printCapacityResult(
    string treeName,
    int capacity,
    int height,
    double searchTime,
    double visited
)
{

    cout
        << left
        << setw(12)
        << treeName

        << setw(12)
        << capacity

        << setw(12)
        << height

        << setw(15)
        << fixed
        << setprecision(3)
        << searchTime

        << setw(15)
        << visited

        << endl;

}

void runExperimentNodeCapacity()
{

    cout << "\n====================================\n";
    cout << " EXPERIMENTO 3: CAPACIDAD DE NODO\n";
    cout << "====================================\n\n";

    vector<int> capacities =
    {
        4,
        8,
        16,
        32
    };

    int N = 10000;


    vector<Rect> data =
        generateUniformPoints(
            N,
            0,
            1000
        );
    vector<Rect> queries =
        generateQueries(
            100,
            0,
            1000
        );

    cout
        << left
        << setw(12)
        << "Arbol"
        << setw(12)
        << "Capacidad"
        << setw(12)
        << "Altura"
        << setw(15)
        << "Busqueda"
        << setw(15)
        << "Nodos"
        << endl;

    cout << "-----------------------------------------------------\n";

    for(int capacity : capacities)
    {
        cout << "\nMax entries = "<< capacity<< endl;
        {
            RTree tree(capacity);
            for(const Rect& r : data)
            {
                tree.insert(r);
            }

            double visited;
            double search =
                measureSearchRTree(
                    tree,
                    queries,
                    visited
                );

            printCapacityResult(
                "RTree",
                capacity,
                tree.height(),
                search,
                visited
            );

        }

        {
            RStarTree tree(capacity);
            for(const Rect& r : data)
            {
                tree.insert(r);
            }

            double visited;
            double search =
                measureSearchRStar(
                    tree,
                    queries,
                    visited
                );

            printCapacityResult(
                "RStar",
                capacity,
                tree.height(),
                search,
                visited
            );

        }


    }

    cout << endl;

}