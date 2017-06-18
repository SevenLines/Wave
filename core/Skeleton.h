//
// Created by m on 18.06.17.
//

#ifndef TESTPROJECT_SKELETON_H
#define TESTPROJECT_SKELETON_H

#include "Point.h"
#include <vector>

using namespace std;

struct Node;

class Skeleton {
public:
    vector<Node *> nodes;

    Node *addNode(Point &point);

    ~Skeleton();
};

class Node {
private:
    Skeleton *skeleton;
public:
    Point point;
    vector<Node *> nodes;

    void bind(Node *node);

    Node(Point &point, Skeleton *skeleton);
};

#endif //TESTPROJECT_SKELETON_H
