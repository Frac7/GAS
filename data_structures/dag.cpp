#include "dag.h"

#include <cg3lib/cg3/core/cg3/geometry/2d/utils2d.h>

/**
 * @brief Adds node the dag and sets it as children of nodes p1 and p2
 * @param[in] value: the node to add
 * @param[in] p1: first parent index in the dag
 * @param[in] p2: second parent index in the dag
*/
void DAG::addNode(const Node& value, unsigned int p1, unsigned int p2)
{
    unsigned int nodeIndex = unsigned(nodeList.size());
    nodeList.push_back(value);

    //updating parents
    nodeList[p1].addChild(int(nodeIndex));
    nodeList[p2].addChild(int(nodeIndex));
}

/**
 * @brief Adds node the dag and sets it as children of node p1
 * @param[in] value: the node to add
 * @param[in] p1: first parent index in the dag
*/
void DAG::addNode(const Node& value, unsigned int p1)
{
    unsigned int nodeIndex = unsigned(nodeList.size());
    nodeList.push_back(value);

    //updating parent
   nodeList[p1].addChild(int(nodeIndex));
}

/**
 * @brief Adds node the dag
 * @param[in] value: the node to add
*/
void DAG::addNode(const Node &value)
{
    nodeList.push_back(value);
}

/**
 * @brief Removes each node from the dag but not the root
*/
void DAG::clearDataStructure()
{
    const std::vector<Node>::iterator nodeListIterator = nodeList.begin() + 1;
    nodeList.erase(nodeListIterator, nodeList.end());

    //reset node as leaf
    nodeList[0].setC1(noChild);
    nodeList[0].setC2(noChild);
    nodeList[0].setC3(noChild);
}

/**
 * @brief Returns the list of nodes
 * @return nodeList: the list of nodes in the dag
*/
std::vector<Node> &DAG::getNodeList()
{
    return nodeList;
}

/**
 * @brief Searches the triangle containing the point using the dag
 * @param[in] i: the current node
 * @param[in] length: total nodes, used as base condition for recursion
 * @param[in] point: last point inserted
 * @param[in] triangles: triangles of triangulation
*/
int DAG::searchInNodes(const unsigned int i, const unsigned int length, const cg3::Point2Dd& point, const std::vector<Triangle>& triangles) const
{
    if(i < length)
    {
        //get triangle index from the node
        unsigned int data = nodeList[i].getData();

        //check if the point is inside this triangle
        bool flagInside = cg3::isPointLyingInTriangle(
                    triangles[data].getV1(), triangles[data].getV2(), triangles[data].getV3(), point, true);
        //check if the node is a leaf
        bool flagLeaf = nodeList[i].isLeaf();

        //if the point is inside and the triangle is a leaf, then return the index of the node in the dag
        if(flagInside && flagLeaf)
        {
            return int(i);
        }
        //if the flag is false, the node can be a parent or the node doesn't contain the point
        else
        {
            int result = -1;

            //in this case the node is not a leaf but it contains the point
            if(!flagLeaf && flagInside)
            {
                int child = noChild;

                child = nodeList[i].getC1();
                //search in children 1
                if(child != noChild)
                {
                    result = searchInNodes(unsigned(child), length, point, triangles);
                    if(result != -1)
                    {
                        return result;
                    }
                }

                child = nodeList[i].getC2();
                //search in children 2
                if(child != noChild)
                {
                    result = searchInNodes(unsigned(child), length, point, triangles);
                    if(result != -1)
                    {
                        return result;
                    }
                }

                child = nodeList[i].getC3();
                //search in children 3
                if(child != noChild)
                {
                    result = searchInNodes(unsigned(child), length, point, triangles);
                    if(result != -1)
                    {
                        return result;
                    }
                }
            }
        }
    }
    //flagLeaf && !flagInside or both flags false
    return -1;
    //it is not necessary to call the function on siblings if the node is a leaf because the parent checks for all the children
}
