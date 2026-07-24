#ifndef RSTARTREE_H
#define RSTARTREE_H

#include "RTree.h"
#include <vector>
#include <set>

class RStarTree : public RTree
{
private:
    std::set<int> reinsertedLevels;
    Node* chooseSubtree(Node* current, const Rect& rect, int depth, int targetDepth);
    void forcedReinsert(Node* node, int level);
    void splitRStar(Node* node);
    void insertInternal(const Rect& rect, int level, Node* childNode = nullptr);
    void adjustTreeRStar(Node* node, Node* newNode);

public:
    RStarTree(int maxEntries = 4);
    ~RStarTree() override = default;

    void insert(const Rect& rect) override;
    void insert(double x, double y) override;
};

#endif 