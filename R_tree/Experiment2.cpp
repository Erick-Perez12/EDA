#include "Experiment2.h"
#include <iostream>
#include <iomanip>
#include <vector>
#include "Experiments.h"
#include "RTree.h"
#include "RStarTree.h"
using namespace std;

void printScaleResult(
    string treeName,
    int n,
    double build,
    double search,
    double visited
)
{

    cout
        << left
        << setw(12)
        << treeName
        << setw(12)
        << n
        << setw(15)
        << fixed
        << setprecision(3)
        << build
        << setw(15)
        << search
        << setw(15)
        << visited
        << endl;

}

void runExperimentScalability()
{

    cout << "\n====================================\n";
    cout << " EXPERIMENTO 2: ESCALABILIDAD\n";
    cout << "====================================\n\n";

    vector<int> sizes =
    {
        500,
        1000,
        2000,
        5000,
        10000
    };
    int maxEntries = 8;
    cout
        << left
        << setw(12)
        << "Arbol"
        << setw(12)
        << "N"
        << setw(15)
        << "Construccion"
        << setw(15)
        << "Busqueda"
        << setw(15)
        << "Nodos"
        << endl;

    cout<< "----------------------------------------------------------\n";

    for(int n : sizes){

        cout << "\nDataset N = "<< n<< endl;
        vector<Rect> data =
            generateUniformPoints(
                n,
                0,
                1000
            );

        vector<Rect> queries =
            generateQueries(
                100,
                0,
                1000
            );

        {
            RTree tree(maxEntries);
            double build =
                measureBuildRTree(
                    tree,
                    data
                );


            double visited;

            double search =
                measureSearchRTree(
                    tree,
                    queries,
                    visited
                );

            printScaleResult(
                "RTree",
                n,
                build,
                search,
                visited
            );
        }

        {
            RStarTree tree(maxEntries);
            double build =
                measureBuildRStar(
                    tree,
                    data
                );


            double visited;


            double search =
                measureSearchRStar(
                    tree,
                    queries,
                    visited
                );

            printScaleResult(
                "RStar",
                n,
                build,
                search,
                visited
            );


        }
    }
    cout << endl;

}