#include "testmatching.h"


#include <learning.h>

#include <memory>
using namespace std;


void TestMatching::match_frechet()
{

//    cout << "loading road graph" << endl;
//    graph.loadBinary("/home/shakya/study/2IL55/project/data/graph.dat");
//    cout << "loaded" << endl;

//    id_type indexID = 1;
//    std::string filePrefix = "/home/shakya/study/2IL55/project/data/alldata";
//    Input input("/home/shakya/study/2IL55/project/data/GisContestTrainingData/input/input_03.txt");

//    auto storage = unique_ptr<IStorageManager>(StorageManager::loadDiskStorageManager(filePrefix));
//    auto tree = unique_ptr<ISpatialIndex>(RTree::loadRTree(*storage, indexID));

//    Output output = mmatch::match_frechet(graph, tree.get(), input);

    RoadGraph graph;
    string path = "/home/shakya/study/2IL55/project/data/test/";
    graph.loadUTM((path + "nodes.txt").c_str(), (path + "edges.txt").c_str(), (path + "edgegeometry.txt").c_str());

    Input input((path + "input.txt").c_str(), true);

    mmatch::match_frechet(graph, NULL, input);


}
