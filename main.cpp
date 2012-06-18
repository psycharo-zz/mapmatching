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


const char *DELIMETER = "\t & \t";




void run_experiment(ostream &out,
                    const string &type, const string &name, const Input &input, const Output &test_output,
                    const RoadGraph &graph, ISpatialIndex *index)
{
    Output output;
    double res_time = -1;
    if (type == "global")
    {
        clock_t time = clock();
        output = mmatch::match_frechet(graph, index, input);
        res_time = double(clock() - time) / (CLOCKS_PER_SEC);
    }
    else if (type == "smartglobal")
    {
        clock_t time = clock();
        output = mmatch::match_frechet_smart(graph, index, input);
        res_time = double(clock() - time) / (CLOCKS_PER_SEC);
    }
    else if (type == "incr")
    {
        clock_t time = clock();
        output = mmatch::backtracing_match(graph, index, input, MAX_ERROR_LOCAL);
        res_time = double(clock() - time) / (CLOCKS_PER_SEC);
    }
    else if (type == "incrsmart")
    {
        clock_t time = clock();
        output = mmatch::backtracing_match_smart(graph, index, input);
        res_time = double(clock() - time) / (CLOCKS_PER_SEC);
    }
    else
    {
        cout << "unknown matching algorithm type" << endl;
        return;
    }
    out << name << DELIMETER
        << type << DELIMETER
        << res_time << DELIMETER
        << output.evaluate(test_output) << DELIMETER
        << output.maxError()  << "\\\\" << endl;
}



int main()
{
    string file_prefix = "alldata";
    id_type index_id = 1;

//    size_t i = 8;
//    string dataset = (i < 10 ? "0" : "") + to_string(i) + ".txt";
//    Input input("../data/input/input_" + dataset);
//    Output test_output("../data/output/output_" + dataset);

//    Output smart_result = mmatch::match_frechet_smart(graph, tree.get(), input);
//    cout << smart_result.evaluate(test_output) << endl;

//    Output result = mmatch::match_frechet(graph, tree.get(), input);
//    cout << result.evaluate(test_output) << endl;

    for (size_t i = 1; i <= 10; i++)
    {
//        if (i == 4)
//            continue;
        RoadGraph graph;
        unique_ptr<ISpatialIndex> tree = load(file_prefix, index_id);
        graph.loadBinary("../data/graph.dat");



        string dataset = (i < 10 ? "0" : "") + to_string(i) + ".txt";

        Input input("../data/input/input_" + dataset);
        Output test_output("../data/output/output_" + dataset);

        run_experiment(cout, "global", dataset, input, test_output, graph, tree.get());

//        run_experiment(cout, "incrsmart", dataset, input, test_output, graph, tree.get());
    }


//    cout << "frechet" << endl;
//    time = clock();
//    Output output_frechet = mmatch::match_frechet(graph, tree.get(), input);
//    cout << output_frechet.evaluate(test_output) << endl;
//    cout << "finished in: " << double(clock() - time) / (CLOCKS_PER_SEC) << endl;

//    cout << "incremental" << endl;
//    time = clock();
//    Output output_incr = mmatch::backtracing_match(graph, tree.get(), input, 100);
//    cout << output_incr.evaluate(test_output) << endl;
//    cout << "finished in: " << double(clock() - time) / (CLOCKS_PER_SEC) << endl;

    return 0;
}

