#include <iostream>
#include <exception>
#include <memory>
#include <cfloat>

using namespace std;


#include "graph.h"
#include "input.h"
#include "spatial.h"





int main()
{


    bool indexed = true;
    string filename = "alldata";
    id_type indexID = 1;

    RoadGraph g;
    g.load("../data/graph1.dat");



    bool exists = access((filename + ".dat").c_str(), F_OK) != 0;

    if (false)
    {
        auto storage = auto_ptr<IStorageManager>(StorageManager::createNewDiskStorageManager(filename, PAGE_SIZE));
        auto tree = auto_ptr<ISpatialIndex>(RTree::createNewRTree(*storage, FILL_FACTOR, CAPACITY, CAPACITY, 2, RTree::RV_RSTAR, indexID));

        cout << "index id: " << indexID << endl;


        int64_t edgeCount = 0;
        for (const Edge *e : g.index())
        {
            int64_t nodeCount = 0;
            for (const UTMNode &n: e->geometry)
            {
                MapPoint p((const double *)&n);
                tree->insertData(0, 0, p, SHAPE_ID(edgeCount, nodeCount));
                ++nodeCount;
            }
            ++edgeCount;
        }
    }
    else
    {
        auto storage = auto_ptr<IStorageManager>(StorageManager::loadDiskStorageManager(filename));
        ISpatialIndex *tree = RTree::loadRTree(*storage, indexID);

        assert(tree->isIndexValid());

        UTMNode n = mmatch::toUTM(47.2964243, -122.2462307);
        MapPoint query((const double *)&n);
        MapNeighborVisitor visitor;

        tree->nearestNeighborQuery(NN_NUMBER-2, query, visitor);

        cout << "size:" << visitor.neighbors.size() << endl;

        double minDist = DBL_MAX;

        for (id_type id: visitor.neighbors)
        {
            cout << EDGE_ID(id) << " " << GEOM_ID(id) << endl;

            const Edge *edge = g.edge(EDGE_ID(id));

            int i = GEOM_ID(id);

            if (i == edge->geometry.size()-1)
            {
                UTMNode a = edge->geometry.at(i-2);
                UTMNode b = edge->geometry.at(i-1);
            }
            else
            {
                UTMNode a = edge->geometry.at(i);
                UTMNode b = edge->geometry.at(i+1);
            }

        }

        delete tree;
        cout << mmatch::distance(47.2964243, -122.2462307, 47.2964243, -122.2462307) << endl;
        cout << mmatch::distance(47.2964243, -122.2462307, 47.2964248, -122.2460123) << endl;
    }

    return 0;
}

