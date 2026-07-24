#ifndef NODE_H
#define NODE_H

#include <vector>
#include "Rect.h"

class Node
{
public:
    bool isLeaf;
    Node* parent;
    std::vector<Node*> children;
    std::vector<Rect> entries;
    Rect mbr;
    Node(bool leaf = true);
    void updateMBR();
    int size() const;

};

#endif