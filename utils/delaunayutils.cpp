#include "delaunayutils.h"
#include "voronoi.h"

/**
 * @brief Creates the Voronoi dual graph of a Delaunay triangulation.
 * This method takes a pointer to a Triangulation and a pointer to a voronoi as input,
 * and fills the voronoi object pointed by the pointer with the Voronoi Diagram
 * of the Triangulation
 *
 *
*/
void DelaunayUtils::createVoronoi(Triangulation *t, Voronoi *voronoi)
{
    std::unordered_map<uint, uint> t_to_c; //triangle index to circumcenter index
    std::vector<uint> activeTris;
    std::vector<Point2Dd> vertices;
    std::vector<uint> edges;

    for (int i = 0; i < t->getActiveList().size(); i++) //of each triangle that ever existed of the triangulation,
    {
        if (    (t->getActiveList()[i]) == true && //only the ones that are active and share no vertices with the BT are picked.
                (t->getTris()[i*3]) != 0 &&
                (t->getTris()[i*3]) != 1 &&
                (t->getTris()[i*3]) != 2 &&
                (t->getTris()[i*3+1]) != 0 &&
                (t->getTris()[i*3+1]) != 1 &&
                (t->getTris()[i*3+1]) != 2 &&
                (t->getTris()[i*3+2]) != 0 &&
                (t->getTris()[i*3+2]) != 1 &&
                (t->getTris()[i*3+2]) != 2
           )
        {
            activeTris.push_back(i*3); // we store the index that is used to find the triangle in the triangulation
            t_to_c.insert({i*3, activeTris.size()-1}); // mapping triangle index in triangulation to index of activeTris, which will also be index of circumcenters.
        }
    }

    for (int i = 0; i<activeTris.size(); i++) //Core of the function.
    {
        vertices.push_back( //for each visible triangle in the triangulation, we compute its circumcenter
        geomUtils::getCircumcenter( t->getVertices()[t->getTris()[activeTris[i]+2]],
                                    t->getVertices()[t->getTris()[activeTris[i]]],
                                    t->getVertices()[t->getTris()[activeTris[i]+1]]
                                    ) );

        /*
         * The following conditional statements are needed to avoid trying to join a circumcenter of a triangle that is incident
         * to an edge of the bounding triangle with its adjacent triangle on that side: that would make no sense since there
         * is no triangle outside of the bounding triangle.
         *
         * Here, the circumcenter of this loop is linked to its adjacent circumcenters by looking up what its adjacent triangle's
         * index will be in the circumcenter vector (vertices), by exploiting the omeomorphism of the indices of vertices and
         * activeTris.
         */

        if (t_to_c[t->getAdj()[activeTris[i]]->getTIndex()] != 0)
        {
            edges.push_back(i);
            edges.push_back(t_to_c[t->getAdj()[activeTris[i]]->getTIndex()]);
        }
        if (t_to_c[t->getAdj()[activeTris[i]+1]->getTIndex()] != 0)
        {
            edges.push_back(i);
            edges.push_back(t_to_c[t->getAdj()[activeTris[i]+1]->getTIndex()]);
        }
        if (t_to_c[t->getAdj()[activeTris[i]+2]->getTIndex()] != 0)
        {
            edges.push_back(i);
            edges.push_back(t_to_c[t->getAdj()[activeTris[i]+2]->getTIndex()]);
        }

    }

    voronoi->addDiagram(vertices, edges);

}

