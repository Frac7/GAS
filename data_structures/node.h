#ifndef NODE_H
#define NODE_H

#include "triangle.h"

#include <cg3/core/cg3/geometry/2d/point2d.h>

//"empty" child
const int noChild = -1;

/**
 * @brief Node: node of the DAG
 *
 * Node: the node belongs to the DAG, it is composed by three children
 * (the graph is directed, so the only way to traverse this data structure is from the root to the children; this is why the (max) 2 parents were not stored in the node)
 * that are integer indices referring to the vector contained in the DAG, and the index of the triangle in the triangulation that is contained in the node.
 * This class implements getters and setters and a method that returns true if the node is a leaf, that means that each of its children is set to -1.
 */
class Node
{
public:
    //create node from children and data
    Node(unsigned int data);

    void addChild(int value);

    //getters and setters
    int getC1() const;
    int getC2() const;
    int getC3() const;
    unsigned int getData() const;


    void setC1(int value);
    void setC2(int value);
    void setC3(int value);

    bool isLeaf() const;

private:
    //first child
    int c1;
    //second child
    int c2;
    //third child
    int c3;
    //data contained in the node
    unsigned int data;
};

#endif // NODE_H
