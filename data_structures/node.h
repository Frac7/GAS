#ifndef NODE_H
#define NODE_H

#include "triangle.h"

#include <cg3/core/cg3/geometry/2d/point2d.h>

class Node
{
public:
    //create node from children and data
    Node(const int& c1, const int&  c2, const int& c3, const Triangle& data);

    void addChild(const int& value);

    //getters and setters
    int getC1() const;

    int getC2() const;

    int getC3() const;

    Triangle getData() const;
    void setData(const Triangle &value);

    bool isLeaf();

private:
    //first child
    int c1;
    //second child
    int c2;
    //third child
    int c3;
    //data contained in the node
    Triangle data;
};

const int noChild = -1;

#endif // NODE_H
