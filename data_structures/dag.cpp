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
 * @param[in] point: last point inserted
 * @param[in] triangles: triangles of triangulation
*/
int DAG::searchInNodes(const cg3::Point2Dd& point, const std::vector<Triangle> &triangles) const
{
    //flags to mark children as visited
    bool firstChildVisited = false;
    bool secondChildVisited = false;
    bool thirdChildVisited = false;

    //indices to perform the search
    unsigned int currentNode = 0;
    int lastParentVisited = -1; //this index allows to "go back" to the parent
    int resultNode = -1;
    int lastChildrenVisited = -1; //this index allows to "go down" in the children subtree

    unsigned int length = unsigned(nodeList.size());

    while(resultNode == -1 && currentNode < length)
    {
        //get triangle index from the node
        unsigned int data = nodeList[unsigned(currentNode)].getData();

        //check if the point is inside this triangle
        bool flagInside = cg3::isPointLyingInTriangle(
                    triangles[unsigned(data)].getV1(), triangles[unsigned(data)].getV2(), triangles[unsigned(data)].getV3(), point, true);
        //check if the node is a leaf
        bool flagLeaf = nodeList[unsigned(currentNode)].isLeaf();

        //if the point is inside and the triangle is a leaf, then return the index of the node in the dag
        if(flagInside && flagLeaf)
        {
            resultNode = int(currentNode);
        }
        //if the flag is false, the node can be a parent or the node doesn't contain the point
        else
        {
            //in this case the node is not a leaf but it contains the point
            if(!flagLeaf && flagInside)
            {
                //if I deside to go down to a child, then these flags must be reset
                if(lastChildrenVisited == int(currentNode))
                {
                    firstChildVisited = false;
                    secondChildVisited = false;
                    thirdChildVisited = false;
                }

                int child1 = nodeList[unsigned(currentNode)].getC1();
                int child2 = nodeList[unsigned(currentNode)].getC2();
                int child3 = nodeList[unsigned(currentNode)].getC3();

                //search in children 1
                if(child1 != noChild && !firstChildVisited)
                {
                    lastParentVisited = int(currentNode);
                    currentNode = unsigned(child1);
                    firstChildVisited = true;
                    lastChildrenVisited = child1;
                }
                //search in children 2
                else if(child2 != noChild && !secondChildVisited)
                {
                    lastParentVisited = int(currentNode);
                    currentNode = unsigned(child2);
                    secondChildVisited = true;
                    lastChildrenVisited = child2;
                }
                //search in children 3
                else if(child3 != noChild && !thirdChildVisited)
                {
                    lastParentVisited = int(currentNode);
                    currentNode = unsigned(child3);
                    thirdChildVisited = true;
                    lastChildrenVisited = child3;
                }
            }
            //flagLeaf && !flagInside or both flags false, then go up to the parent
            else if(!flagInside)
            //it is not necessary to call the function on siblings if the node is a leaf because the parent checks for all the children
            {
                currentNode = unsigned(lastParentVisited);
            }

        }
    }
    return resultNode;
}
