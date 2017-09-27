#ifndef DAGNODE_H
#define DAGNODE_H

#include "triangle.h"

class DagNode
{
    public:
        DagNode(int index);

        void addChild(DagNode *child);
        std::vector<DagNode *> getChildren();
        bool isLeaf();
        int getTIndex() const;
        void setTIndex(int value);

        void setChildren(const std::vector<DagNode *> &value);
        bool isAlive();
private:
        int tIndex;
        std::vector<DagNode*> children;
        bool dead = false;
};
#endif // DAGNODE_H
