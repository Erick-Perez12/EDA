#include "RStarTree.h"
#include <algorithm>
#include <cmath>

RStarTree::RStarTree(int maxEntries) : RTree(maxEntries){
}

void RStarTree::insert(double x, double y){
    insert(Rect(x, y, x, y));
}

void RStarTree::insert(const Rect& rect){
    reinsertedLevels.clear();
    insertInternal(rect, 0); 
}

void RStarTree::insertInternal(const Rect& rect, int level, Node* childNode){
    Node* targetNode = chooseSubtree(root, rect, height() - 1, level);
    if (targetNode->isLeaf){
        targetNode->entries.push_back(rect);
    }
    else{
        if (childNode != nullptr){
            childNode->parent = targetNode;
            targetNode->children.push_back(childNode);
        }
    }

    targetNode->updateMBR();
    if (targetNode->size() > maxEntries){
        if (targetNode != root && reinsertedLevels.find(level) == reinsertedLevels.end()){
            reinsertedLevels.insert(level);
            forcedReinsert(targetNode, level);
        }
        else{
            splitRStar(targetNode);
        }
    }
    else{
        adjustTree(targetNode);
    }
}

Node* RStarTree::chooseSubtree(Node* current, const Rect& rect, int currentHeight, int targetLevel){
    if (currentHeight == targetLevel || current->isLeaf)
        return current;

    Node* bestChild = nullptr;
    if (current->children[0]->isLeaf){
        double minOverlapInc = 1e100;
        double minAreaInc = 1e100;

        for (Node* child : current->children)
        {
            double currentOverlap = 0.0;
            double newOverlap = 0.0;

            for (Node* other : current->children)
            {
                if (child == other) continue;
                if (child->mbr.intersects(other->mbr))
                {
                    Rect inter(
                        std::max(child->mbr.xmin, other->mbr.xmin),
                        std::max(child->mbr.ymin, other->mbr.ymin),
                        std::min(child->mbr.xmax, other->mbr.xmax),
                        std::min(child->mbr.ymax, other->mbr.ymax)
                    );
                    currentOverlap += inter.area();
                }

                Rect expanded = Rect::combine(child->mbr, rect);
                if (expanded.intersects(other->mbr))
                {
                    Rect inter(
                        std::max(expanded.xmin, other->mbr.xmin),
                        std::max(expanded.ymin, other->mbr.ymin),
                        std::min(expanded.xmax, other->mbr.xmax),
                        std::min(expanded.ymax, other->mbr.ymax)
                    );
                    newOverlap += inter.area();
                }
            }

            double overlapInc = newOverlap - currentOverlap;
            double areaInc = child->mbr.enlargement(rect);

            if (overlapInc < minOverlapInc)
            {
                minOverlapInc = overlapInc;
                minAreaInc = areaInc;
                bestChild = child;
            }
            else if (overlapInc == minOverlapInc && areaInc < minAreaInc)
            {
                minAreaInc = areaInc;
                bestChild = child;
            }
        }
    }
    else{
        double minAreaInc = 1e100;
        double minArea = 1e100;

        for (Node* child : current->children){
            double areaInc = child->mbr.enlargement(rect);
            double area = child->mbr.area();

            if (areaInc < minAreaInc){
                minAreaInc = areaInc;
                minArea = area;
                bestChild = child;
            }
            else if (areaInc == minAreaInc && area < minArea){
                minArea = area;
                bestChild = child;
            }
        }
    }

    return chooseSubtree(bestChild, rect, currentHeight - 1, targetLevel);
}

void RStarTree::forcedReinsert(Node* node, int level){
    int p = static_cast<int>(node->size() * 0.30);
    if (p < 1) p = 1;

    double centerX = node->mbr.centerX();
    double centerY = node->mbr.centerY();

    if (node->isLeaf){
        std::sort(node->entries.begin(), node->entries.end(),
            [centerX, centerY](const Rect& a, const Rect& b) {
                double distA = std::pow(a.centerX() - centerX, 2) + std::pow(a.centerY() - centerY, 2);
                double distB = std::pow(b.centerX() - centerX, 2) + std::pow(b.centerY() - centerY, 2);
                return distA > distB;
            });

        std::vector<Rect> removedEntries(node->entries.begin(), node->entries.begin() + p);
        node->entries.erase(node->entries.begin(), node->entries.begin() + p);
        node->updateMBR();
        adjustTree(node);

        for (const Rect& r : removedEntries){
            insertInternal(r, level, nullptr);
        }
    }
    else{
        std::sort(node->children.begin(), node->children.end(),
            [centerX, centerY](Node* a, Node* b) {
                double distA = std::pow(a->mbr.centerX() - centerX, 2) + std::pow(a->mbr.centerY() - centerY, 2);
                double distB = std::pow(b->mbr.centerX() - centerX, 2) + std::pow(b->mbr.centerY() - centerY, 2);
                return distA > distB;
            });

        std::vector<Node*> removedChildren(node->children.begin(), node->children.begin() + p);
        node->children.erase(node->children.begin(), node->children.begin() + p);
        node->updateMBR();
        adjustTree(node);

        for (Node* child : removedChildren){
            insertInternal(child->mbr, level + 1, child);
        }
    }
}

void RStarTree::splitRStar(Node* node){
    struct Element {
        Rect mbr;
        Node* childNode;
    };

    std::vector<Element> elements;
    if (node->isLeaf) {
        for (const auto& r : node->entries) elements.push_back({r, nullptr});
    } else {
        for (auto* c : node->children) elements.push_back({c->mbr, c});
    }

    int n = elements.size();
    int bestAxis = 0; 
    double minPerimeterSum = 1e100;
    for (int axis = 0; axis < 2; ++axis){
        double axisPerimeter = 0.0;
        for (int sortCriterion = 0; sortCriterion < 2; ++sortCriterion){
            std::sort(elements.begin(), elements.end(), [axis, sortCriterion](const Element& a, const Element& b) {
                if (axis == 0) return (sortCriterion == 0) ? a.mbr.xmin < b.mbr.xmin : a.mbr.xmax < b.mbr.xmax;
                return (sortCriterion == 0) ? a.mbr.ymin < b.mbr.ymin : a.mbr.ymax < b.mbr.ymax;
            });
            for (int k = minEntries; k <= n - minEntries; ++k){
                Rect r1 = elements[0].mbr;
                for (int i = 1; i < k; ++i) r1 = Rect::combine(r1, elements[i].mbr);

                Rect r2 = elements[k].mbr;
                for (int i = k + 1; i < n; ++i) r2 = Rect::combine(r2, elements[i].mbr);

                double p1 = 2 * ((r1.xmax - r1.xmin) + (r1.ymax - r1.ymin));
                double p2 = 2 * ((r2.xmax - r2.xmin) + (r2.ymax - r2.ymin));
                axisPerimeter += (p1 + p2);
            }
        }

        if (axisPerimeter < minPerimeterSum){
            minPerimeterSum = axisPerimeter;
            bestAxis = axis;
        }
    }

    int bestK = minEntries;
    double minOverlap = 1e100;
    double minAreaSum = 1e100;

    std::sort(elements.begin(), elements.end(), [bestAxis](const Element& a, const Element& b) {
        return (bestAxis == 0) ? a.mbr.xmin < b.mbr.xmin : a.mbr.ymin < b.mbr.ymin;
    });

    for (int k = minEntries; k <= n - minEntries; ++k){
        Rect r1 = elements[0].mbr;
        for (int i = 1; i < k; ++i) r1 = Rect::combine(r1, elements[i].mbr);

        Rect r2 = elements[k].mbr;
        for (int i = k + 1; i < n; ++i) r2 = Rect::combine(r2, elements[i].mbr);

        double overlap = 0.0;
        if (r1.intersects(r2)){
            Rect inter(
                std::max(r1.xmin, r2.xmin),
                std::max(r1.ymin, r2.ymin),
                std::min(r1.xmax, r2.xmax),
                std::min(r1.ymax, r2.ymax)
            );
            overlap = inter.area();
        }

        double areaSum = r1.area() + r2.area();

        if (overlap < minOverlap){
            minOverlap = overlap;
            minAreaSum = areaSum;
            bestK = k;
        }
        else if (overlap == minOverlap && areaSum < minAreaSum){
            minAreaSum = areaSum;
            bestK = k;
        }
    }

    Node* newNode = new Node(node->isLeaf);
    newNode->parent = node->parent;

    if (node->isLeaf){
        node->entries.clear();
        newNode->entries.clear();

        for (int i = 0; i < bestK; ++i) node->entries.push_back(elements[i].mbr);
        for (int i = bestK; i < n; ++i) newNode->entries.push_back(elements[i].mbr);
    }
    else{
        node->children.clear();
        newNode->children.clear();

        for (int i = 0; i < bestK; ++i){
            node->children.push_back(elements[i].childNode);
            elements[i].childNode->parent = node;
        }
        for (int i = bestK; i < n; ++i){
            newNode->children.push_back(elements[i].childNode);
            elements[i].childNode->parent = newNode;
        }
    }

    node->updateMBR();
    newNode->updateMBR();
    adjustTreeRStar(node, newNode);
}

void RStarTree::adjustTreeRStar(Node* node, Node* newNode){
    if (node == root){
        if (newNode != nullptr){
            Node* newRoot = new Node(false);
            newRoot->children.push_back(node);
            newRoot->children.push_back(newNode);

            node->parent = newRoot;
            newNode->parent = newRoot;

            root = newRoot;
            root->updateMBR();
        }
        return;
    }

    Node* parentNode = node->parent;
    node->updateMBR();

    if (newNode != nullptr){
        newNode->parent = parentNode;
        parentNode->children.push_back(newNode);
        parentNode->updateMBR();
        if (parentNode->size() > maxEntries){
            splitRStar(parentNode);
        }
        else{
            adjustTreeRStar(parentNode, nullptr);
        }
    }
    else{
        adjustTreeRStar(parentNode, nullptr);
    }
}