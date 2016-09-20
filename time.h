#ifndef STDEX_TIME_H_
#define STDEX_TIME_H_

#include "stdex/stdcc.h"
namespace stdex {

inline u64 get_time_tick()
{
#ifdef _MSC_VER
    struct _timeb tb;
    _ftime(&tb);
    return (u64)tb.time*1000 + (u64)tb.millitm;
#else
    struct timeb tb;
    ftime(&tb);
    return (u64)tb.time*1000 + (u64)tb.millitm;
#endif
}

inline string get_time_compact()
{
	char buf[32];
	time_t t = time(NULL);
	strftime(buf, sizeof(buf), "%Y%m%d%H%M%S", localtime(&t));
	return buf;
}

inline string get_time_iso()
{
	char buf[32];
	time_t t = time(NULL);
	strftime(buf, sizeof(buf), "%F %T", localtime(&t));
	return buf;
}

}
#endif //STDEX_TIME_H_