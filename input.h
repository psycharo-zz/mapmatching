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
    Input(const std::string &fileName, bool utm = false);

    void load(const std::string &fileName);

    void loadUTM(const std::string &fileName);

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
        m_estmns(size),
        maxError(0.0)
    {}

    Output(const Input &input):
        m_estmns(input.nodes().size()),
        maxError(0.0)
    {}

    Output(const std::string &fileName)
    {
        load(fileName);
    }


    //! load output from file
    void load(const std::string &fileName);

    //! save output to file
    void save(const std::string &fileName) const;

    //! get measure according to another output
    double evaluate(const Output &according) const;

    //! set estimation value
    inline void setEstimation(int32_t id, int32_t edge, float confidence)
    {
        m_estmns[id] = Estimate(edge, confidence);
    }

    //! data accessor
    inline const std::vector<Estimate> &estimates() const { return m_estmns; }
    inline float& getMaxError() { return maxError; } // TODO : set it more private

    inline int32_t edge(int32_t id) const { return m_estmns[id].edge; }
    inline float confidence(int32_t id) const { return m_estmns[id].confidence; }
    //! get size
    inline size_t size() const { return m_estmns.size(); }

    //! result validator, i.e. check if all matched edges are connected
    bool validate(const RoadGraph& );
private:
    float maxError;
    std::vector<Estimate> m_estmns;

}; // end of class Output


double evaluate(const Output &a, const Output &b);


}

#endif // INPUT_H
