#ifndef NODE_H
#define NODE_H

#include "triangle.h"

#include <cg3/core/cg3/geometry/2d/point2d.h>

class Node
{
public:
    //create node from children and data
    Node(const int& c1, const int&  c2, const int& c3, const int& data);

    void addChild(const int& value);

    //getters and setters
    int getC1() const;

    int getC2() const;

    int getC3() const;

    int getData() const;
    void setData(const int& value);

    bool isLeaf();

    //methods for managing flag deletion
    bool isDeleted();
    void setDeleted();
    //TODO: evaluate the best position to place these methods...

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

//when a node must be deleted, the data field contains this value
const int deleted = -1;

#endif // NODE_H
