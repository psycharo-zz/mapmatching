#include "testmatching.h"


#include <learning.h>
#include <frechet.h>

#include <memory>
using namespace std;


void TestMatching::matchFrechet()
{
    RoadGraph graph;
//    cout << "loading road graph" << endl;
//    graph.loadBinary("/home/shakya/study/2IL55/project/data/graph.dat");
//    Input input("/home/shakya/study/2IL55/project/data/GisContestTrainingData/input/input_03.txt");


//    id_type indexID = 1;
//    std::string filePrefix = "/home/shakya/study/2IL55/project/data/alldata";


//    auto storage = unique_ptr<IStorageManager>(StorageManager::loadDiskStorageManager(filePrefix));
//    auto tree = unique_ptr<ISpatialIndex>(RTree::loadRTree(*storage, indexID));

//    Output output = mmatch::match_frechet(graph, tree.get(), input);


    string path = "/home/shakya/study/2IL55/project/data/test/";
    graph.loadUTM((path + "nodes.txt").c_str(), (path + "edges.txt").c_str(), (path + "edgegeometry.txt").c_str());
    Input input((path + "input.txt").c_str(), true);

    mmatch::match_frechet(graph, NULL, input);


}
