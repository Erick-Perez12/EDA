#include "RTree.h"
#include <algorithm>
#include <queue>

RTree::RTree(int maxEntries)
{
    this->maxEntries = maxEntries;
    this->minEntries = std::max(2, maxEntries / 2);
    root = new Node(true);
}

RTree::~RTree()
{
    if (root == nullptr)
        return;

    std::queue<Node*> q;
    q.push(root);

    while (!q.empty())
    {
        Node* current = q.front();
        q.pop();

        if (!current->isLeaf)
        {
            for (Node* child : current->children)
                q.push(child);
        }

        delete current;
    }
}

Node* RTree::getRoot() const{
    return root;
}

int RTree::height() const{
    if (root == nullptr)
        return 0;

    int h = 1;

    Node* current = root;

    while (!current->isLeaf)
    {
        current = current->children[0];
        h++;
    }

    return h;
}

Node* RTree::chooseLeaf(Node* current, const Rect& rect){
    if (current->isLeaf)
        return current;

    double bestIncrease = 1e100;
    double bestArea = 1e100;

    Node* bestChild = nullptr;

    for (Node* child : current->children){
        double increase =
            child->mbr.enlargement(rect);

        double area =
            child->mbr.area();

        if (increase < bestIncrease){
            bestIncrease = increase;
            bestArea = area;
            bestChild = child;
        }
        else if (increase == bestIncrease)
        {
            if (area < bestArea){
                bestArea = area;
                bestChild = child;
            }
        }
    }

    return chooseLeaf(bestChild, rect);
}

void RTree::adjustTree(Node* node){
    while (node != nullptr)
    {
        node->updateMBR();

        node = node->parent;
    }
}

void RTree::pickSeeds(const std::vector<Rect>& rects,int& seed1,int& seed2){
    int n = rects.size();
    seed1 = 0;
    seed2 = 1;
    double maxWaste = -1.0;
    for (int i = 0; i < n; i++){
        for (int j = i + 1; j < n; j++){
            Rect combined = Rect::combine(rects[i], rects[j]);
            double waste =
                combined.area() -
                rects[i].area() -
                rects[j].area();

            if (waste > maxWaste){
                maxWaste = waste;
                seed1 = i;
                seed2 = j;
            }
        }
    }
}
void RTree::insert(double x, double y){
    insert(Rect(x, y, x, y));
}

void RTree::insert(const Rect& rect){
    Node* leaf = chooseLeaf(root, rect);
    leaf->entries.push_back(rect);
    leaf->updateMBR();
    Node* current = leaf;
    while (current != nullptr){
        if (current->size() > maxEntries){
            Node* sibling = split(current);
            if(current->parent == nullptr){
                Node* newRoot = new Node(false);

                newRoot->children.push_back(current);
                newRoot->children.push_back(sibling);

                current->parent = newRoot;
                sibling->parent = newRoot;

                root = newRoot;

                newRoot->updateMBR();
                break;
            }
            else{
                Node* parent = current->parent;
                parent->children.push_back(sibling);
                sibling->parent = parent;
                parent->updateMBR();
                current = parent;
            }

        }
        else{
            current->updateMBR();
            current = current->parent;
        }
    }
}
Node* RTree::split(Node* node){
    int n = node->size();

    struct ItemInfo {
        int index;
        Rect mbr;
    };

    std::vector<ItemInfo> items(n);
    Rect totalMBR;

    if (node->isLeaf){
        for (int i = 0; i < n; i++)
        {
            items[i] = {i, node->entries[i]};
            totalMBR = (i == 0) ? node->entries[i] : Rect::combine(totalMBR, node->entries[i]);
        }
    }
    else{
        for (int i = 0; i < n; i++){
            items[i] = {i, node->children[i]->mbr};
            totalMBR = (i == 0) ? node->children[i]->mbr : Rect::combine(totalMBR, node->children[i]->mbr);
        }
    }

    int seed1 = 0, seed2 = 1;
    int minXMaxIdx = 0, maxXMinIdx = 0;
    double minXMax = items[0].mbr.xmax;
    double maxXMin = items[0].mbr.xmin;

    for (int i = 1; i < n; i++){
        if (items[i].mbr.xmax < minXMax) { minXMax = items[i].mbr.xmax; minXMaxIdx = i; }
        if (items[i].mbr.xmin > maxXMin) { maxXMin = items[i].mbr.xmin; maxXMinIdx = i; }
    }
    double widthX = totalMBR.xmax - totalMBR.xmin;
    double sepX = (widthX > 0) ? (maxXMin - minXMax) / widthX : 0;

    int minYMaxIdx = 0, maxYMinIdx = 0;
    double minYMax = items[0].mbr.ymax;
    double maxYMin = items[0].mbr.ymin;

    for (int i = 1; i < n; i++){
        if (items[i].mbr.ymax < minYMax) { minYMax = items[i].mbr.ymax; minYMaxIdx = i; }
        if (items[i].mbr.ymin > maxYMin) { maxYMin = items[i].mbr.ymin; maxYMinIdx = i; }
    }
    double heightY = totalMBR.ymax - totalMBR.ymin;
    double sepY = (heightY > 0) ? (maxYMin - minYMax) / heightY : 0;

    if (sepX >= sepY){
        seed1 = minXMaxIdx;
        seed2 = maxXMinIdx;
    }
    else{
        seed1 = minYMaxIdx;
        seed2 = maxYMinIdx;
    }

    if (seed1 == seed2) seed2 = (seed1 + 1) % n;

    Node* newNode = new Node(node->isLeaf);
    newNode->parent = node->parent;

    std::vector<bool> assigned(n, false);
    assigned[seed1] = true;
    assigned[seed2] = true;

    Rect mbr1 = items[seed1].mbr;
    Rect mbr2 = items[seed2].mbr;

    std::vector<int> group1Indices = {seed1};
    std::vector<int> group2Indices = {seed2};

    for (int i = 0; i < n; i++){
        if (assigned[i]) continue;

        int assignedCount = 0;
        for (bool a : assigned) if (a) assignedCount++;
        int unassignedCount = n - assignedCount;
        if (group1Indices.size() + unassignedCount == minEntries){
            group1Indices.push_back(i);
            mbr1 = Rect::combine(mbr1, items[i].mbr);
            assigned[i] = true;
            continue;
        }
        if (group2Indices.size() + unassignedCount == minEntries){
            group2Indices.push_back(i);
            mbr2 = Rect::combine(mbr2, items[i].mbr);
            assigned[i] = true;
            continue;
        }

        double d1 = Rect::combine(mbr1, items[i].mbr).area() - mbr1.area();
        double d2 = Rect::combine(mbr2, items[i].mbr).area() - mbr2.area();

        if (d1 < d2 || (d1 == d2 && mbr1.area() < mbr2.area())){
            group1Indices.push_back(i);
            mbr1 = Rect::combine(mbr1, items[i].mbr);
        }
        else{
            group2Indices.push_back(i);
            mbr2 = Rect::combine(mbr2, items[i].mbr);
        }
        assigned[i] = true;
    }

    if (node->isLeaf){
        std::vector<Rect> oldEntries = node->entries;
        node->entries.clear();
        newNode->entries.clear();

        for (int idx : group1Indices) node->entries.push_back(oldEntries[idx]);
        for (int idx : group2Indices) newNode->entries.push_back(oldEntries[idx]);
    }
    else{
        std::vector<Node*> oldChildren = node->children;
        node->children.clear();
        newNode->children.clear();

        for (int idx : group1Indices){
            node->children.push_back(oldChildren[idx]);
            oldChildren[idx]->parent = node;
        }
        for (int idx : group2Indices){
            newNode->children.push_back(oldChildren[idx]);
            oldChildren[idx]->parent = newNode;
        }
    }

    node->updateMBR();
    newNode->updateMBR();
    return newNode;
}
std::vector<Rect> RTree::search(const Rect& query, int& visitedNodes){
    visitedNodes = 0;
    std::vector<Rect> result;
    if (root != nullptr)
    {
        searchRecursive(root, query, result, visitedNodes);
    }
    return result;
}

void RTree::searchRecursive(Node* node, const Rect& query, std::vector<Rect>& result, int& visitedNodes){
    visitedNodes++;
    if (node->isLeaf){
        for (const Rect& entry : node->entries){
            if (query.intersects(entry)){
                result.push_back(entry);
            }
        }
    }
    else{
        for (Node* child : node->children){
            if (query.intersects(child->mbr)){
                searchRecursive(child, query, result, visitedNodes);
            }
        }
    }
}