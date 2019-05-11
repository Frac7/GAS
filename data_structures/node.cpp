#include "node.h"

Node::Node(const int& c1, const int&  c2, const int& c3, const Triangle& data)
    : c1(c1), c2(c2), c3(c3), data(data) {}

void Node::addChild(const int &value)
{
    if(c1 == noChild)
    {
        c1 = value;
    } else if(c2 == noChild)
    {
        c2 = value;
    } else
    {
        c3 = value;
    }
}

int Node::getC1() const
{
    return c1;
}

int Node::getC2() const
{
    return c2;
}

int Node::getC3() const
{
    return c3;
}

Triangle Node::getData() const
{
    return data;
}

void Node::setData(const Triangle &value)
{
    data = value;
}

bool Node::isLeaf()
{
    return c1 == noChild && c2 == noChild && c3 == noChild;
}
