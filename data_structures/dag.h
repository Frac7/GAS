#ifndef DAG_H
#define DAG_H

#include "node.h"

class DAG
{
public:

    //add node to the dag
    void addNode(const Node& node, const unsigned int p1, const unsigned int p2);
    void addNode(const Node& node, const unsigned int p1);
    void addNode(const Node& node);

    /* adding a node means adding a triangle
     * if the triangle was created adding a point inside a triangle, p2 will be null
     * while if the triangle was created by edge legalization, the number of parents will be 2
     * max 2 parents and max 3 children */

    void clearDataStructure();

    int searchInNodes(const cg3::Point2Dd& point, const std::vector<Triangle> &triangles) const;

    std::vector<Node>& getNodeList();

private:
    std::vector<Node> nodeList;
};

#endif // DAG_H
