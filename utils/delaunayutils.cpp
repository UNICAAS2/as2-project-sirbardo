#include "delaunayutils.h"
#include "voronoi.h"


void DelaunayUtils::createVoronoi(Triangulation *t, Voronoi *voronoi)
{
    std::unordered_map<uint, uint> t_to_c;
    std::vector<uint> activeTris;
    std::vector<Point2Dd> vertices;
    std::vector<uint> edges;

    for (int i = 0; i < t->getActiveList().size(); i++)
    {
        if (t->getActiveList()[i] == true)
        {
            activeTris.push_back(i*3);
            t_to_c.insert({i*3, activeTris.size()-1}); // mapping triangle index in triangulation to circumcenter index in voronoi
        }
    }

    for ( unsigned i = 0; i < t_to_c.bucket_count(); ++i) {
      std::cout << "bucket #" << i << " contains:";
      for ( auto local_it = t_to_c.begin(i); local_it!= t_to_c.end(i); ++local_it )
        std::cout << " " << local_it->first << ":" << local_it->second;
      std::cout << std::endl;
    }


    for (int i = 0; i<activeTris.size(); i++)
    {
        vertices.push_back(geomUtils::getCircumcenter(t->getVertices()[t->getTris()[activeTris[i]]], t->getVertices()[t->getTris()[activeTris[i]+1]], t->getVertices()[t->getTris()[activeTris[i]+2]]));

        edges.push_back(t_to_c[t->getAdj()[activeTris[i]]->getTIndex()]);
        edges.push_back(t_to_c[t->getAdj()[activeTris[i]+1]->getTIndex()]);
        edges.push_back(t_to_c[t->getAdj()[activeTris[i]+2]->getTIndex()]);

    }

    voronoi->addDiagram(vertices, edges);

}
