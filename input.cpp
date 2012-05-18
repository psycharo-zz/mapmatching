#include "input.h"

#include <iostream>
#include <fstream>
#include <cstdlib>
#include <iomanip>

// DEBUG
#include <iostream>




void Input::load(const char *fileName)
{
    std::ifstream ifinput(fileName);

    if (!ifinput.is_open())
        throw GraphException("can't open input file");

    Node node(0, 0);
    std::string s;
    while (getline(ifinput, s))
    {
        size_t posL;
        size_t posR = s.find(IO_DELIM);
        // ignoring id

        posL = posR+1;
        posR = s.find(IO_DELIM, posL);
        node.lat = atof(s.substr(posL, posR-posL).c_str());

        posL = posR+1;
        node.lon = atof(s.substr(posL, s.length()-posL).c_str());

        m_nodes.push_back(node);
    }
}




void Output::load(const char *fileName)
{
    std::ifstream ifoutput(fileName);

    if (!ifoutput.is_open())
        throw GraphException("can't open input file");

    m_estmns.clear();
    Estimate estmn;
    std::string s;
    while (getline(ifoutput, s))
    {
        size_t posL;
        size_t posR = s.find(IO_DELIM);
        // ignoring id

        posL = posR+1;
        posR = s.find(IO_DELIM, posL);
        estmn.edge = atoi(s.substr(posL, posR-posL).c_str());

        posL = posR+1;
        estmn.confidence = atof(s.substr(posL, s.length()-posL).c_str());

        m_estmns.push_back(estmn);
    }
}


void Output::save(const char *fileName) const
{
    std::ofstream ofoutput(fileName);

    if (!ofoutput.is_open())
        throw GraphException("can't open file for output");

    ofoutput.setf(std::ios_base::floatfield, std::ios_base::fixed);
    ofoutput.precision(2);
    for (int i = 0; i < m_estmns.size(); i++)
        ofoutput << i << IO_DELIM << m_estmns[i].edge << IO_DELIM << m_estmns[i].confidence << '\n';
}

