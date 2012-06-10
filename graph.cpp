#include "graph.h"

#include <cstdlib>
#include <limits>
#include <fstream>

// DEBUG
#include <iostream>


using namespace std;



// TODO: split?
void Edge::fillGeometry(const string &s)
{
    geometry.clear();

    size_t posL = 0,
           posR = 0;

    // position of the first ^
    posR = s.find(DELIM, posL);
    // 0..posL - id

    posL = posR+1;
    posR = s.find(DELIM, posL);
    s.copy(name, posR-posL, posL);
//    name = s.substr(posL, posR-posL);

    posL = posR+1;
    posR = s.find(DELIM, posL);
    s.copy(type, posR-posL, posL);
//    type = s.substr(posL, posR-posL);

    posL = posR+1;
    posR = s.find(DELIM, posL);
    length = atof(s.substr(posL, posR-posL).c_str());

    double lat, lon;
    while (posR != string::npos)
    {
        posL = posR+1;
        posR = s.find(DELIM, posL);
        lat = atof(s.substr(posL, posR-posL).c_str());

        posL = posR+1;
        posR = s.find(DELIM, posL);
        lon = atof(s.substr(posL, posR-posL).c_str());

        geometry.push_back(mmatch::toUTM(lat, lon));
    }
}



RoadGraph::~RoadGraph()
{
    for (const Edge *edge: m_edgeIndex)
        delete edge;
}

void RoadGraph::load(const char *fileNodes, const char *fileEdges, const char *fileGeometry)
{
    ifstream ifnodes(fileNodes);

    if (!ifnodes.is_open())
        throw Exception("can't open node data");

    double lat, lon;
    int id;
    while (ifnodes >> id >> lat >> lon)
    {
        m_nodes.push_back(mmatch::toUTM(lat, lon));
        ifnodes.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    ifnodes.close();


    ifstream ifedges(fileEdges);
    ifstream ifgeom(fileGeometry);

    if (!ifedges.is_open() || !ifgeom.is_open())
        throw Exception("can't open edge data");

    m_edges.resize(m_nodes.size());

    string geomStr;
    Edge edge;
    while (ifedges >> edge.id >> edge.from >> edge.to >> edge.cost &&
           getline(ifgeom, geomStr))
    {
        Edge *e = new Edge(edge);
        e->fillGeometry(geomStr);

        // adding to adjacency list
        m_edges[e->from].push_back(e);

        // adding pointer to edge index
        // NOTE!!!: supposing that id read corresponds to the str number FIXME?
        m_edgeIndex.push_back(e);
        ifedges.ignore(numeric_limits<streamsize>::max(), '\n');
    }
    ifedges.close();
    ifgeom.close();
}



void RoadGraph::loadBinary(const char *inputFile)
{
    ifstream input(inputFile, ios_base::binary);

    if (!input.is_open())
        throw Exception("RoadGraph::loadBinary(): can't open the file");

    size_t size = 0;

    m_nodes.clear();
    m_edges.clear();
    // TODO: separate deletion function?
    m_edgeIndex.clear();
    for (const Edge *edge: m_edgeIndex)
        delete edge;

    input.read((char *)&size, sizeof(size_t));
    m_nodes.resize(size);
    m_edges.resize(size);
    input.read((char *)&m_nodes.front(), size * sizeof(UTMNode));

    input.read((char *)&size, sizeof(size_t));
    m_edgeIndex.resize(size);

    for (size_t i = 0; i < m_edgeIndex.size(); ++i)
    {
        Edge *e = new Edge;
        input.read((char *)&e->id, (sizeof(int)<<2) + sizeof(double) + (Edge::MAX_NAME_LENGTH<<1));
        input.read((char *)&size, sizeof(size_t));
        e->geometry.resize(size);
        input.read((char *)&e->geometry.front(), size * sizeof(UTMNode));

        m_edges[e->from].push_back(e);
        m_edgeIndex[i] = e;
    }

}





void RoadGraph::saveBinary(const char *dst)
{
    ofstream output(dst, ios_base::binary);

    size_t size = m_nodes.size();
    output.write((const char *)&size, sizeof(size_t));
    output.write((const char *)&m_nodes.front(), size * sizeof(UTMNode));

    size = m_edgeIndex.size();
    output.write((const char *)&size, sizeof(size_t));
    for (const Edge *e: m_edgeIndex)
    {
        output.write((const char *)&e->id, (sizeof(int)<<2) + sizeof(double) + (Edge::MAX_NAME_LENGTH<<1));
        size = e->geometry.size();
        output.write((const char *)&size, sizeof(size_t));
        output.write((const char *)&e->geometry.front(), size * sizeof(UTMNode));
    }
}









