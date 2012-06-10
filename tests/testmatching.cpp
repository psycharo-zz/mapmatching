#include "testmatching.h"


#include <learning.h>

#include <memory>
using namespace std;


void TestMatching::match_frechet()
{
    RoadGraph graph;
    cout << "loading road graph" << endl;
    graph.loadBinary("/home/shakya/study/2IL55/project/data/graph.dat");
    cout << "loaded" << endl;

    id_type indexID = 1;
    std::string filePrefix = "/home/shakya/study/2IL55/project/data/alldata";
    Input input("/home/shakya/study/2IL55/project/data/GisContestTrainingData/input/input_03.txt");

    auto storage = unique_ptr<IStorageManager>(StorageManager::loadDiskStorageManager(filePrefix));
    auto tree = unique_ptr<ISpatialIndex>(RTree::loadRTree(*storage, indexID));

    Output output = mmatch::match_frechet(graph, tree.get(), input);

}
