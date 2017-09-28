#include "dagnode.h"

DagNode::DagNode(int index)
{
    tIndex = index;
}

std::vector<DagNode *>& DagNode::getChildren()
{
    return children;
}

bool DagNode::isLeaf()
{
    if (children.size() == 0) return true;
    else return false;
}

void DagNode::addChild(DagNode *child)
{
    children.push_back(child);
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
    children = value;
}

