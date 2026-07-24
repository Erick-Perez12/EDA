#include "Node.h"

Node::Node(bool leaf){
    isLeaf = leaf;
    parent = nullptr;
}

int Node::size() const{
    if (isLeaf)
        return entries.size();

    return children.size();
}

void Node::updateMBR(){
    if (isLeaf){
        if (entries.empty())
            return;
        Rect r = entries[0];
        for (size_t i = 1; i < entries.size(); i++){
            r = Rect::combine(r, entries[i]);
        }

        mbr = r;
    }
    else{
        if (children.empty())
            return;

        Rect r = children[0]->mbr;
        for (size_t i = 1; i < children.size(); i++){
            r = Rect::combine(r, children[i]->mbr);
        }
        mbr = r;
    }
}