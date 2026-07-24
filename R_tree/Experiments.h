#ifndef EXPERIMENTS_H
#define EXPERIMENTS_H

#include <vector>
#include <chrono>
#include <random>
#include "Rect.h"
#include "RTree.h"
#include "RStarTree.h"

std::vector<Rect> generateUniformPoints(
    int n,
    double minCoord,
    double maxCoord
);

std::vector<Rect> generateClusterPoints(
    int n,
    int clusters,
    double minCoord,
    double maxCoord
);

std::vector<Rect> generateQueries(
    int n,
    double minCoord,
    double maxCoord
);

double measureBuildRTree(
    RTree& tree,
    const std::vector<Rect>& data
);

double measureBuildRStar(
    RStarTree& tree,
    const std::vector<Rect>& data
);

double measureSearchRTree(
    RTree& tree,
    const std::vector<Rect>& queries,
    double& averageVisited
);

double measureSearchRStar(
    RStarTree& tree,
    const std::vector<Rect>& queries,
    double& averageVisited
);


#endif