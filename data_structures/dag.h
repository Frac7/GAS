#ifndef DAG_H
#define DAG_H

#include "node.h"

/**
 * @brief DAG: search data structure
 *
 * the DAG is implemented using a vector of nodes. In this implementation, triangulation data structure stores all the triangles seen in the execution, and when a triangle is added to the triangulation, the corresponding node is added to the DAG: this means that the DAG and the vectors containing adjacencies and triangles in the triangulation are parallel with it.
 * In this class, there are 3 overloads for adding nodes: the first method allows to add only the data of the triangle contained in the node; the second allows to add a node and update its parent to set it as child, this is the case when the point is inserted inside the triangle; the third allows to add a node and update its two parents: this is the case of the edge flip.
 * There are also a method for clearing the data structur without removing the root and a method for search in the DAG: this method pick a point and the vector of triangle of the triangulation and, starting from the root, perform an iterative search updating the index according to parents and children.

 */
class DAG
{
public:

    //add node to the dag
    void addNode(const Node& node, unsigned int p1, unsigned int p2);
    void addNode(const Node& node, unsigned int p1);
    void addNode(const Node& node);

    /* adding a node means adding a triangle
     * if the triangle was created adding a point inside a triangle, p2 will be null
     * while if the triangle was created by edge legalization, the number of parents will be 2
     * max 2 parents and max 3 children */

    void clearDataStructure();

    std::vector<Node>& getNodeList();

    int searchInNodes(const unsigned int i, const unsigned int length, const cg3::Point2Dd& point, const std::vector<Triangle>& triangles) const;

private:
    std::vector<Node> nodeList;
};

#endif // DAG_H
