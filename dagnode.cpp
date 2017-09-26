#include "dagnode.h"

DagNode::DagNode(int index)
{
    tIndex = index;
}

std::vector<DagNode *> DagNode::getChildren() const
{
    return children;
}

int DagNode::getTIndex() const
{
    return tIndex;
}

void DagNode::setTIndex(int value)
{
    tIndex = value;
}

void DagNode::setChildren(const std::vector<DagNode *> &value)
{
    dead = true;
    children = value;
}
