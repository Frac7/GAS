#include "dag.h"


void DAG::addNode(const Node& value, Node& p1, Node& p2)
{
    int nodeIndex = nodeList.size();
    nodeList.push_back(value);

    //TODO: check if p2 exists
    p1.addChild(nodeIndex);
    p2.addChild(nodeIndex);
}
