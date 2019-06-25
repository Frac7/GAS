#include "node.h"

/**
 * @brief Creates a node without children and initializes the data field with the input parameter
 * @param[in] data: the index of the triangle in the array of triangulation
*/
Node::Node(const unsigned int data)
    : c1(noChild), c2(noChild), c3(noChild), data(data) {}
/**
 * @brief Adds a child to a existing node
 * @param[in] value: the index of the child in the dag
*/
void Node::addChild(const int value)
{
    if(c1 == noChild)
    {
        c1 = value;
    }
    else if(c2 == noChild)
    {
        c2 = value;
    }
    else
    {
        c3 = value;
    }
}

/**
 * @brief Returns child 1
 * @return c1: child 1
*/
int Node::getC1() const
{
    return c1;
}

/**
 * @brief Returns child 2
 * @return c2: child 2
*/
int Node::getC2() const
{
    return c2;
}

/**
 * @brief Returns child 3
 * @return c3: child 3
*/
int Node::getC3() const
{
    return c3;
}

/**
 * @brief Returns the node data
 * @return data: index of the triangle in the triangulation
*/
int Node::getData() const
{
    return data;
}

/**
 * @brief Returns true or false if the node is or isn't a leaf
 *
 * If a node is a leaf, its children are all equal to -1, that is the const "noChild".
 *
 * @return flag: the node has or hasn't children
*/
bool Node::isLeaf() const
{
    return c1 == noChild && c2 == noChild && c3 == noChild;
}

/**
 * @brief Sets child 1
 * @param[in] value: the index of the child
*/
void Node::setC1(int value)
{
    c1 = value;
}

/**
 * @brief Sets child 2
 * @param[in] value: the index of the child
*/
void Node::setC2(int value)
{
    c2 = value;
}

/**
 * @brief Sets child 3
 * @param[in] value: the index of the child
*/
void Node::setC3(int value)
{
    c3 = value;
}
