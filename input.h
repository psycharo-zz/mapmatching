#ifndef INPUT_H
#define INPUT_H


#include "graph.h"


namespace mmatch {

static const char IO_DELIM = ',';

class Input
{
public:
    Input() {}
    void load(const char *fileName);

    inline const std::vector<WGS84Node> &nodes() const { return m_nodes;}
private:
    std::vector<WGS84Node> m_nodes;
};



class Output
{
public:
    class Estimate
    {
    public:
        //! the edge the point belongs to
        int edge;
        //! the confidence
        float confidence;

        Estimate():
            edge(-1),
            confidence(-1)
        {}

        Estimate(int _edge, float _conf):
            edge(_edge),
            confidence(_conf)
        {}
    };

    Output(int size = 0):
        m_estmns(size)
    {}

    Output(const Input &input):
        m_estmns(input.nodes().size())
    {}


    //! load output from file
    void load(const char *fileName);

    //! save output to file
    void save(const char *fileName) const;

    //! set estimation value
    inline void setEstimation(int id, int edge, float confidence)
    {
        m_estmns[id] = Estimate(edge, confidence);
    }

    //! data accessor
    inline const std::vector<Estimate> &estimates() const { return m_estmns; }

private:
    std::vector<Estimate> m_estmns;

};


class Evaluation
{
public:
    Evaluation(const Output &outputA, const Output &outputB);

private:
    // some evaluation results
    // TODO: to string? which kinds of results
};


}

#endif // INPUT_H
