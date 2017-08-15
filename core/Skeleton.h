//
// Created by m on 18.06.17.
//

#ifndef TESTPROJECT_SKELETON_H
#define TESTPROJECT_SKELETON_H

#include "Point.h"
#include <vector>
#include <memory>

using namespace std;

struct Node;

class Skeleton {
public:
    vector<shared_ptr<Node>> nodes;

    shared_ptr<Node> addNode(Point &point);

    ~Skeleton();
};

class Node : public enable_shared_from_this<Node> {
private:
    Skeleton *skeleton;
public:
    Point point;
    vector<shared_ptr<Node>> nodes;

    void bind(shared_ptr<Node> node);

    Node(Point &point, Skeleton *skeleton);
};

#endif //TESTPROJECT_SKELETON_H
