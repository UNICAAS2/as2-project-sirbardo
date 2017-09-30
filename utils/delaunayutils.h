#ifndef DELAUNAYUTILS_H
#define DELAUNAYUTILS_H


#include "triangle.h"
#include "delaunaytriangulation.h"
#include <common/point2d.h>
#include "geomutils.h"
#include <unordered_map>
#include "dagdelaunay.h"


class Voronoi;

/**
 * Algorithms related to the Delaunay Triangulation
 *
 */

namespace DelaunayUtils
{

    void createVoronoi(Triangulation *t, Voronoi *voronoi);
}

#endif // DelaunayUtils
