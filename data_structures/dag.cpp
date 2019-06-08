#include "dag.h"

#include <cg3lib/cg3/core/cg3/geometry/2d/utils2d.h>


void DAG::addNode(const Node& value, const unsigned int& p1, const unsigned int& p2)
{
    int nodeIndex = nodeList.size();
    nodeList.push_back(value);

    nodeList[p1].addChild(nodeIndex);
    nodeList[p2].addChild(nodeIndex);
}

void DAG::addNode(const Node& value, const unsigned int& p1)
{
    int nodeIndex = nodeList.size();
    nodeList.push_back(value);

   nodeList[p1].addChild(nodeIndex);
}

void DAG::addNode(const Node &value)
{
    nodeList.push_back(value);
}

void DAG::clearDataStructure()
{
    const std::vector<Node>::iterator nodeListIterator = nodeList.begin() + 1;
    nodeList.erase(nodeListIterator, nodeList.end());

    //reset to node as leaf
    nodeList[0].setC1(noChild);
    nodeList[0].setC2(noChild);
    nodeList[0].setC3(noChild);
}

std::vector<Node> DAG::getNodeList() const
{
    return nodeList;
}

int DAG::searchInNodes(const unsigned int& i, const unsigned int& length, const cg3::Point2Dd point, const std::vector<Triangle>& triangles) const
{
    bool flag = false;

    if(i < length)
    {
        //get triangle index from the node
        unsigned int data = nodeList[i].getData();

        //check if the point is inside this triangle
        bool flagInside = cg3::isPointLyingInTriangle(
                    triangles[data].getV1(), triangles[data].getV2(), triangles[data].getV3(), point, false);
        //check if the node is a leaf
        bool flagLeaf = nodeList[i].isLeaf();

        flag = flagInside && flagLeaf;

        //if the point is inside and the triangle is a leaf, then return the index of the node in the dag
        if(flag)
        {
            return i;
        }
        //if the flag is false, the node can be a parent or the node doesn't contain the point
        else
        {
            int result = outside;

            //in this case the node is not a leaf but it contains the point
            if(!flagLeaf && flagInside)
            {
                //TODO: evaluate if it is reasonable to create a function to avoid code repetition
                int child = noChild;

                child = nodeList[i].getC1();
                //search in children 1
                if(child != noChild)
                {
                    result = searchInNodes(child, length, point, triangles);
                    if(result != outside)
                    {
                        return result;
                    }
                }

                child = nodeList[i].getC2();
                //search in children 2
                if(child != noChild)
                {
                    result = searchInNodes(child, length, point, triangles);
                    if(result != outside)
                    {
                        return result;
                    }
                }

                child = nodeList[i].getC3();
                //search in children 3
                if(child != noChild)
                {
                    result = searchInNodes(child, length, point, triangles);
                    if(result != outside)
                    {
                        return result;
                    }
                }
            }
            else //flagLeaf && !flagInside or both flags false
            {
                return outside;
            }
            //maybe it is not necessary to call the function on siblings if the node is a leaf because the parent checks for all the children
        }
    }

    return outside;
}
