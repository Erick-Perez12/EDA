#include "Experiments.h"
#include <cmath>
using std::milli;

std::vector<Rect> generateUniformPoints(
    int n,
    double minCoord,
    double maxCoord)
{

    std::vector<Rect> points;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> dist(
        minCoord,
        maxCoord
    );

    for(int i = 0; i < n; i++)
    {
        double x = dist(gen);
        double y = dist(gen);

        points.push_back(
            Rect(x,y,x,y)
        );
    }
    return points;
}

std::vector<Rect> generateClusterPoints(int n,int clusters,double minCoord,double maxCoord){
    std::vector<Rect> points;
    std::random_device rd;
    std::mt19937 gen(rd());

    std::uniform_real_distribution<double> centerDist( minCoord,maxCoord);
    std::normal_distribution<double> gaussian(0,5);

    std::vector<std::pair<double,double>> centers;

    for(int i=0;i<clusters;i++){
        centers.push_back(
            {
                centerDist(gen),
                centerDist(gen)
            }
        );
    }
    for(int i=0;i<n;i++){
        auto center = centers[i % clusters];
        double x = center.first + gaussian(gen);
        double y = center.second + gaussian(gen);
        points.push_back( Rect(x,y,x,y));
    }
    return points;
}


std::vector<Rect> generateQueries(
    int n,
    double minCoord,
    double maxCoord)
{
    std::vector<Rect> queries;

    std::random_device rd;
    std::mt19937 gen(rd());
    std::uniform_real_distribution<double> dist(minCoord, maxCoord);
    std::uniform_real_distribution<double> sizeDist(1,10);

    for(int i=0;i<n;i++){

        double x1 = dist(gen);
        double y1 = dist(gen);
        double width = sizeDist(gen);
        double height = sizeDist(gen);

        queries.push_back(
            Rect(
                x1,
                y1,
                x1+width,
                y1+height
            )
        );

    }
    return queries;
}

double measureBuildRTree(
    RTree& tree,
    const std::vector<Rect>& data)
{

    auto start = std::chrono::high_resolution_clock::now();

    for(const Rect& r:data){
        tree.insert(r);
    }

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double,milli>(
        end-start
    ).count();

}

double measureBuildRStar(
    RStarTree& tree,
    const std::vector<Rect>& data)
{

    auto start = std::chrono::high_resolution_clock::now();

    for(const Rect& r:data){
        tree.insert(r);
    }

    auto end = std::chrono::high_resolution_clock::now();

    return std::chrono::duration<double,milli>(
        end-start
    ).count();

}

double measureSearchRTree(
    RTree& tree,
    const std::vector<Rect>& queries,
    double& averageVisited)
{

    int totalVisited = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(const Rect& q:queries) {
        int visited = 0;
        tree.search(q, visited );
        totalVisited += visited;

    }
    auto end = std::chrono::high_resolution_clock::now();
    averageVisited = (double)totalVisited / queries.size();
    return std::chrono::duration<double,milli>(
        end-start
    ).count();

}

double measureSearchRStar(
    RStarTree& tree,
    const std::vector<Rect>& queries,
    double& averageVisited)
{
    int totalVisited = 0;
    auto start = std::chrono::high_resolution_clock::now();
    for(const Rect& q:queries)
    {
        int visited = 0;
        tree.search(
            q,
            visited
        );
        totalVisited += visited;

    }

    auto end = std::chrono::high_resolution_clock::now();

    averageVisited = (double)totalVisited / queries.size();

    return std::chrono::duration<double,milli>(
        end-start
    ).count();

}