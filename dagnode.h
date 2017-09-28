#ifndef DAGNODE_H
#define DAGNODE_H

#include "triangle.h"

class DagNode
{
    public:
        DagNode(int index);

        void addChild(DagNode *child);
        std::vector<DagNode *> &getChildren();
        bool isLeaf();
        int getTIndex();
        void setTIndex(int value);

        void setChildren(const std::vector<DagNode *> &value);
        ~DagNode();
private:
        int tIndex;
        std::vector<DagNode*> children;
};
#endif // DAGNODE_H
