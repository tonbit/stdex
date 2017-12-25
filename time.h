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

inline string to_time_spec(std::time_t t, const string &format)
{
#ifndef _MSC_VER
	char buf[128];
	strftime(buf, sizeof(buf), format.c_str(), localtime(&t));
	return buf;
#else
	std::stringstream ss;
	ss << std::put_time(localtime(&t), format.c_str());
	return ss.str();
#endif
}

inline string to_time_compact(std::time_t t)
{
#ifndef _MSC_VER
	char buf[128];
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", localtime(&t));
	return buf;
#else
	std::stringstream ss;
	ss << std::put_time(localtime(&t), "%Y%m%d%H%M%S");
	return ss.str();
#endif
}

//ISO 8601
inline string to_time_iso(std::time_t t)
{
#ifndef _MSC_VER
	char buf[128];
	strftime(buf, sizeof(buf), "%Y-%m-%d %H:%M:%S", localtime(&t));
	return buf;
#else
	std::stringstream ss;
	ss << std::put_time(localtime(&t), "%Y-%m-%d %H:%M:%S");
	return ss.str();
#endif
}

//ISO 8601
inline time_t from_time_iso(const string &in)
{
	if (in.empty())
		return 0;

#ifndef _MSC_VER
	std::tm tm;
	strptime(in.c_str(), "%Y-%m-%d %H:%M:%S", &tm);
	return mktime(&tm);
#else
	std::tm tm;
	std::stringstream ss(in);
	ss >> std::get_time(&tm, "%Y-%m-%d %H:%M:%S");
	return mktime(&tm);
#endif
}

inline string to_time_gmt(std::time_t t)
{
#ifndef _MSC_VER
	char buf[128];
	strftime(buf, sizeof(buf), "%a, %d %b %Y %H:%M:%S GMT", localtime(&t));
	return buf;
#else
	std::stringstream ss;
	ss << std::put_time(localtime(&t), "%a, %d %b %Y %H:%M:%S GMT");
	return ss.str();
#endif
}

}
#endif //STDEX_TIME_H_
