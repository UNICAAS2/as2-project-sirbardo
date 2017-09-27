#include "dagnode.h"

DagNode::DagNode(int index)
{
    tIndex = index;
}

std::vector<DagNode *> DagNode::getChildren()
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
    dead=true;
}

int DagNode::getTIndex() const
{
    return tIndex;
}

void DagNode::setTIndex(int value)
{
    tIndex = value;
}

bool DagNode::isAlive()
{
    return dead ? false : true;
}

void DagNode::setChildren(const std::vector<DagNode *> &value)
{
    dead = true;
    children = value;
}

