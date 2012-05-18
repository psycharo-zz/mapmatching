#include <iostream>
#include <exception>

using namespace std;


#include "graph.h"
#include "input.h"

//#include <spatialindex/RTree.h>


#include <typeinfo>
int main()
{
//    Edge e;

    RoadGraph g;

    try
    {

//        input.load("data/input/input_01.txt");
//        output.load("data/output/output_01.txt");
        g.load("../data/graph1.dat");
//        g.save("../data/graph.dat");


//        std::cout << "max distance:" << maxDistance(g, input, output) << std::endl;

    }
    catch (std::exception &ex)
    {
        std::cout << "ERROR:" << ex.what() << std::endl;
    }


    return 0;
}

