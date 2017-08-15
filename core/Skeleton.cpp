//
// Created by m on 18.06.17.
//

#include "Skeleton.h"

void Node::bind(shared_ptr<Node> node) {
    this->nodes.push_back(node);
    node->nodes.push_back(shared_from_this());
}

Node::Node(Point &_point, Skeleton *_skeleton) : point(_point.x, _point.y), skeleton(_skeleton) {
}

shared_ptr<Node> Skeleton::addNode(Point &point) {
    auto node = make_shared<Node>(point, this);
    nodes.push_back(node);
    return node;
}

Skeleton::~Skeleton() {
    nodes.clear();
}
