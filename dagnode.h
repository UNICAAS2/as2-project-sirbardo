#ifndef DAGNODE_H
#define DAGNODE_H

#include "triangle.h"

class DagNode
{
    public:
        DagNode();

    private:
        std::vector<Triangle> tris;
};

#endif // DAGNODE_H
