#ifndef INPUT_H
#define INPUT_H


#include "graph.h"


namespace mmatch {



/**
 * @brief The Input class incapsulates vehicle trajectories
 */
class Input
{
public:
    Input(const char *fileName);

    void load(const char *fileName);

    inline const UTMNode& operator[](size_t i) const { return m_nodes[i]; }

    inline const size_t size() const { return m_nodes.size(); }

    inline const std::vector<UTMNode> &nodes() const { return m_nodes;}
private:
    std::vector<UTMNode> m_nodes;
};


/**
 * @brief The Output class is used for storing/loading output in the competition format
 */
class Output
{
public:
    class Estimate
    {
    public:
        //! the edge the point belongs to
        int32_t edge;
        //! the confidence
        float confidence;

        Estimate():
            edge(-1),
            confidence(-1)
        {}

        Estimate(int32_t _edge, float _conf):
            edge(_edge),
            confidence(_conf)
        {}
    };

    Output(size_t size = 0):
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
    inline void setEstimation(int32_t id, int32_t edge, float confidence)
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
