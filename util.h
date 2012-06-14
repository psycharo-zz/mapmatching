#ifndef UTIL_H
#define UTIL_H


#include <string>


namespace mmatch
{


static const char DELIM = '^';

static const char IO_DELIM = ',';


// TODO: decent exceptions
class Exception : public std::exception
{
public:
    Exception(const char *_msg):
        m_msg(_msg)
    {}

    virtual ~Exception() throw() {}

    const char *what() const throw()
    {
        return m_msg.data();
    }

private:
    std::string m_msg;
};




inline bool exists(const std::string &filename)
{
    return access(filename.c_str(), F_OK) == 0;
}





}



#endif // UTIL_H
