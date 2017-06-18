//
// Created by m on 18.06.17.
//

#include "Skeleton.h"

void Node::bind(Node *node) {
    this->nodes.push_back(node);
    node->nodes.push_back(this);
}


Node::Node(Point &_point, Skeleton *_skeleton) : point(_point.x, _point.y), skeleton(_skeleton) {
}

Node *Skeleton::addNode(Point &point) {
    auto node = new Node(point, this);
    nodes.push_back(node);
    return node;
}

Skeleton::~Skeleton() {
    for (auto node : nodes) {
        delete node;
    }
    nodes.clear();
}
