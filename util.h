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


//! identifying shared point, does not belong to any edge
static const int32_t EID_COMMON = -1;


struct geom_id
{
    int32_t eid;
    int32_t gid;

    geom_id(int32_t _eid, int32_t _gid):
        eid(_eid),
        gid(_gid)
    {}

    inline bool operator<(const geom_id &other) const
    {
        return (eid == other.eid) ? gid < other.gid : eid < other.eid;
    }

    inline bool operator==(const geom_id &other) const
    {
        return (eid == other.eid) && (gid == other.gid);
    }

    inline bool is_internal() const
    {
        return eid != EID_COMMON;
    }

};


inline std::ostream &operator<<(std::ostream &os, const geom_id &id)
{
    if (id.is_internal())
        os << "edge:" << id.eid << " geom:" << id.gid;
    else
        os << "node:" << id.gid;
    return os;
}



}



#endif // UTIL_H
