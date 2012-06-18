#include "input.h"

#include <fstream>
#include <cstdlib>
//#include <iomanip>

using namespace std;


Input::Input(const std::string &fileName, bool utm)
{
    if (utm)
        loadUTM(fileName);
    else
        load(fileName);
}


void Input::load(const std::string &fileName)
{
    m_path = fileName;
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


void Input::loadUTM(const std::string &fileName)
{
    m_path = fileName;
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

        m_nodes.push_back(UTMNode(lat, lon));
    }
}


vector<Input> Input::split(size_t parts) const
{
    vector<Input> result;

    size_t step = m_nodes.size() / (parts-1);
    size_t i = 0;
    while (i + step < m_nodes.size())
    {
        result.push_back(Input(vector<UTMNode>(m_nodes.begin() + i, m_nodes.begin() + i + step)));
        i += step;
    }
    if (i + step > m_nodes.size())
    {
        result.push_back(Input(vector<UTMNode>(begin(m_nodes) + i, end(m_nodes))));
    }

    return result;
}

Input Input::merge(const vector<Input> &result) const
{
    vector<UTMNode> res;
    for (const Input &input : result)
        res.insert(res.end(), input.m_nodes.begin(), input.m_nodes.end());
    return Input(res);
}


Input::Input(const std::vector<UTMNode> &nodes):
    m_nodes(nodes)
{
}






void Output::load(const std::string &fileName)
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


void Output::save(const std::string &fileName) const
{
    std::ofstream ofoutput(fileName);

    if (!ofoutput.is_open())
        throw Exception("can't open file for output");

    ofoutput.setf(std::ios_base::floatfield, std::ios_base::fixed);
    ofoutput.precision(2);
    for (int i = 0; i < m_estmns.size(); i++)
        ofoutput << i << IO_DELIM << m_estmns[i].edge << IO_DELIM << m_estmns[i].confidence << '\n';
}



double Output::evaluate(const Output &according) const
{
    if (estimates().size() != according.estimates().size())
        throw Exception("sizes must be equal to evaluate");
    double result = 0;
    for (size_t i = 0; i < estimates().size(); ++i)
    {
        if (edge(i) == according.edge(i))
            result += confidence(i);
    }
    return result / estimates().size();
}


Output Output::merge(const vector<Output> &result) const
{
    vector<Estimate> res;
    for (const Output &output : result)
        res.insert(res.end(), output.m_estmns.begin(), output.m_estmns.end());
    return Output(res);
}

Output::Output(std::vector<Estimate> &estimates):
    m_estmns(estimates)
{

}

