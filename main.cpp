#include <iostream>
#include <exception>
#include <memory>
#include <cfloat>

using namespace std;


#include "graph.h"
#include "input.h"
#include "spatial.h"


#include "learning.h"




int main()
{


    string filePrefix = "alldata";
    id_type indexID = 1;



    RoadGraph graph;

    cout << "loading road graph" << endl;
    graph.loadBinary("../data/graph.dat");


    //graph.load("../data/WA_Nodes.txt", "../data/WA_Edges.txt", "../data/WA_EdgeGeometry.txt");

    //cout << "saving road graph" << endl;
    //graph.saveBinary("../data/graph.dat");

    if (!exists(filePrefix + ".dat"))
    {
        cout << "creating spatial index" << endl;


        auto storage = auto_ptr<IStorageManager>(StorageManager::createNewDiskStorageManager(filePrefix, PAGE_SIZE));
        auto tree = auto_ptr<ISpatialIndex>(RTree::createNewRTree(*storage, FILL_FACTOR, CAPACITY, CAPACITY, 2, RTree::RV_RSTAR, indexID));

        cout << "index id: " << indexID << endl;


        int64_t edgeCount = 0;
        for (const Edge *e : graph.index())
        {
            int64_t nodeCount = 0;
            for (const UTMNode &n: e->geometry)
            {
                MapPoint p(n);
                tree->insertData(0, 0, p, SHAPE_ID(edgeCount, nodeCount));
                ++nodeCount;
            }
            ++edgeCount;
        }
    }
    else
    {
        cout << "loading spatial index" << endl;

        Input input("../data/GisContestTrainingData/input/input_01.txt");

        auto storage = auto_ptr<IStorageManager>(StorageManager::loadDiskStorageManager(filePrefix));
        auto tree = auto_ptr<ISpatialIndex>(RTree::loadRTree(*storage, indexID));

        Output output = mmatch::match(graph, tree.get(), input);

        cout << mmatch::distance(47.2964243, -122.2462307, 47.2964243, -122.2462307) << endl;
        cout << mmatch::distance(47.2964243, -122.2462307, 47.2964248, -122.2460123) << endl;
    }

    return 0;
}

