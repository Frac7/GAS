#ifndef DAG_H
#define DAG_H

#include "node.h"

class DAG
{
public:
    //add node to the dag
    void addNode(const Node& node, Node& p1, Node& p2);
    /* adding a node means adding a triangle
     * if the triangle was created adding a point inside a triangle, p2 will be null
     * while if the triangle was created by edge legalization, the number of parents will be 2
     * max 2 parents and max 3 children */

    void clearDataStructure();

private:
    std::vector<Node> nodeList;
};

#endif // DAG_H
