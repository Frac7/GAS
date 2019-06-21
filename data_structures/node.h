#ifndef NODE_H
#define NODE_H

#include "triangle.h"

#include <cg3/core/cg3/geometry/2d/point2d.h>

class Node
{
public:
    //create node from children and data
    Node(const int data);

    void addChild(const int value);

    //getters and setters
    int getC1() const;

    int getC2() const;

    int getC3() const;

    int getData() const;
    void setData(const int value);

    bool isLeaf() const;

    void setC1(int value);

    void setC2(int value);

    void setC3(int value);

private:
    //first child
    int c1;
    //second child
    int c2;
    //third child
    int c3;
    //data contained in the node
    int data;
};

const int noChild = -1;

#endif // NODE_H
