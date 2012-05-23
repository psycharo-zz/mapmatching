#include "input.h"

#include <fstream>
#include <cstdlib>
//#include <iomanip>

using namespace std;


Input::Input(const char *fileName)
{
    load(fileName);
}


void Input::load(const char *fileName)
{
    std::ifstream ifinput(fileName);

    if (!ifinput.is_open())
        throw Exception("can't open input file");

    double lat, lon;
    std::string s;
    while (getline(ifinput, s))
    {
        size_t posL;
        size_t posR = s.find(IO_DELIM);
        // ignoring id

        posL = posR+1;
        posR = s.find(IO_DELIM, posL);
        lat = atof(s.substr(posL, posR-posL).c_str());

        posL = posR+1;
        lon = atof(s.substr(posL, s.length()-posL).c_str());

        m_nodes.push_back(mmatch::toUTM(lat, lon));
    }
}




void Output::load(const char *fileName)
{
    std::ifstream ifoutput(fileName);

    if (!ifoutput.is_open())
        throw Exception("can't open input file");

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
        throw Exception("can't open file for output");

    ofoutput.setf(std::ios_base::floatfield, std::ios_base::fixed);
    ofoutput.precision(2);
    for (int i = 0; i < m_estmns.size(); i++)
        ofoutput << i << IO_DELIM << m_estmns[i].edge << IO_DELIM << m_estmns[i].confidence << '\n';
}

