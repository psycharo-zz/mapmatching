#include <iostream>
#include <exception>
#include <memory>
#include <cfloat>

using namespace std;


#include "graph.h"
#include "input.h"
#include "spatial.h"

#include "frechet.h"

#include "learning.h"


unique_ptr<ISpatialIndex> load(string file_prefix, int index_id)
{
    auto storage = StorageManager::loadDiskStorageManager(file_prefix);
    auto tree = unique_ptr<ISpatialIndex>(RTree::loadRTree(*storage, index_id));
    return tree;
}

shared_ptr<ISpatialIndex> construct(id_type index_id)
{
    RoadGraph graph;
    graph.loadUTM("../data/test/nodes.txt", "../data/test/edges.txt", "../data/test/edgegeometry.txt");

    auto storage = StorageManager::createNewMemoryStorageManager();
    auto tree = shared_ptr<ISpatialIndex>(RTree::createNewRTree(*storage, FILL_FACTOR, CAPACITY, CAPACITY, 2, RTree::RV_RSTAR, index_id));
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
    return tree;
}

#include <ctime>

int main()
{
    string file_prefix = "alldata";
    id_type index_id = 1;

    RoadGraph graph;

    string dataset = "02";
    graph.loadBinary("../data/graph.dat");
    Input input("../data/input/input_" + dataset + ".400.txt");
    auto tree = load(file_prefix, index_id);

    clock_t time = clock();
    Output output = mmatch::match_frechet(graph, tree.get(), input);
    cout << "finished in: " << double(clock() - time) / (CLOCKS_PER_SEC) << endl;

//    Output output = mmatch::backtracing_match(graph, tree.get(), input, 100);
    Output about("../data/output/output_" + dataset + ".400.txt");
    cout << output.evaluate(about) << endl;




    return 0;
}

