#ifndef RTREE_H
#define RTREE_H

#include <vector>
#include "Node.h"

class RTree
{
protected:
    Node* root;
    int maxEntries;
    int minEntries;
    Node* chooseLeaf(Node* current, const Rect& rect);

    void adjustTree(Node* node);
    Node* split(Node* node);

    void pickSeeds(
        const std::vector<Rect>& rects,
        int& seed1,
        int& seed2
    );

    void searchRecursive(
        Node* node,
        const Rect& query,
        std::vector<Rect>& result,
        int& visitedNodes
    );

public:
    RTree(int maxEntries = 4);
    virtual ~RTree();
    virtual void insert(double x, double y);

    virtual void insert(const Rect& rect);
    std::vector<Rect> search(
        const Rect& query,
        int& visitedNodes
    );

    Node* getRoot() const;
    int height() const;

};

#endif