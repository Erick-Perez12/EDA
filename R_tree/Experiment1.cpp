#include "Experiment1.h"

#include <iostream>
#include <iomanip>

#include "Experiments.h"
#include "RTree.h"
#include "RStarTree.h"
using namespace std;

void printResult(
    string treeName,
    string distribution,
    double buildTime,
    double searchTime,
    double visited
)
{

    cout 
        << left
        << setw(12)
        << treeName

        << setw(15)
        << distribution

        << setw(15)
        << fixed
        << setprecision(3)
        << buildTime

        << setw(15)
        << searchTime

        << setw(15)
        << visited

        << endl;

}

void runExperimentDistribution()
{
    cout << "\n====================================\n";
    cout << " EXPERIMENTO 1: DISTRIBUCION DATOS\n";
    cout << "====================================\n\n";
    int N = 5000;

    int maxEntries = 8;
    vector<Rect> uniform =
        generateUniformPoints(
            N,
            0,
            1000
        );


    vector<Rect> clusters =
        generateClusterPoints(
            N,
            5,
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

        << setw(15)
        << "Distrib."

        << setw(15)
        << "Construccion"

        << setw(15)
        << "Busqueda"

        << setw(15)
        << "Nodos"

        << endl;

    cout<< "---------------------------------------------------------\n";
    {

        RTree tree(maxEntries);

        double build =
            measureBuildRTree(
                tree,
                uniform
            );

        double visited;

        double search =
            measureSearchRTree(
                tree,
                queries,
                visited
            );

        printResult(
            "RTree",
            "Uniforme",
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
                uniform
            );
        double visited;
        double search =
            measureSearchRStar(
                tree,
                queries,
                visited
            );

        printResult(
            "RStar",
            "Uniforme",
            build,
            search,
            visited
        );
    }

    {
        RTree tree(maxEntries);
        double build =
            measureBuildRTree(
                tree,
                clusters
            );

        double visited;
        double search =
            measureSearchRTree(
                tree,
                queries,
                visited
            );

        printResult(
            "RTree",
            "Clusters",
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
                clusters
            );

        double visited;

        double search =
            measureSearchRStar(
                tree,
                queries,
                visited
            );

        printResult(
            "RStar",
            "Clusters",
            build,
            search,
            visited
        );

    }
    cout << endl;

}