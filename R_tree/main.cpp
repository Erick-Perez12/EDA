#include <iostream>
#include "Experiment1.h"
#include "Experiment2.h"
#include "Experiment3.h"
using namespace std;
int main()
{

    cout << "====================================\n";
    cout << " COMPARACION R-TREE VS R*-TREE\n";
    cout << " EXPERIMENTOS\n";
    cout << "====================================\n";

    runExperimentDistribution();
    runExperimentScalability();
    runExperimentNodeCapacity();

    cout << "\n====================================\n";
    cout << " EXPERIMENTOS FINALIZADOS\n";
    cout << "====================================\n";
    return 0;
}