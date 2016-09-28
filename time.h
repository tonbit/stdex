#ifndef STDEX_TIME_H_
#define STDEX_TIME_H_

#include "stdex/stdcc.h"
#include <sys/timeb.h>
namespace stdex {

inline uint64_t get_time_tick()
{
#ifdef _MSC_VER
    struct _timeb tb;
    _ftime(&tb);
    return (uint64_t)tb.time*1000 + (uint64_t)tb.millitm;
#else
    struct timeb tb;
    ftime(&tb);
    return (uint64_t)tb.time*1000 + (uint64_t)tb.millitm;
#endif
}

inline string to_time_compact(std::time_t t)
{
	std::stringstream ss;
	ss << std::put_time(localtime(&t), "%Y%m%d%H%M%S");
	return ss.str();
}

//ISO 8601
inline string to_time_iso(std::time_t t)
{
	std::stringstream ss;
	ss << std::put_time(localtime(&t), "%Y-%m-%d %H:%M:%S");
	return ss.str();
}

//ISO 8601
inline time_t from_time_iso(const string &in)
{
	std::tm tm;
	std::stringstream ss(in);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	return mktime(&tm);
}

}
#endif //STDEX_TIME_H_
