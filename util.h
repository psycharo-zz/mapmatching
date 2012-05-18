#ifndef UTIL_H
#define UTIL_H


// TODO: decent exceptions
class GraphException : public std::exception
{
public:
    GraphException(const char *_msg):
        m_msg(_msg)
    {}

    virtual ~GraphException() throw() {}

    const char *what() const throw()
    {
        return m_msg.data();
    }

private:
    std::string m_msg;
};


#endif // UTIL_H
