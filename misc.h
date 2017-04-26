#ifndef STDEX_MISC_H_
#define STDEX_MISC_H_

#include "stdex/stdcc.h"
#include <chrono>
#include <thread>
#ifdef _MSC_VER
#include <Ws2tcpip.h>
#define inet_ntop InetNtop
#endif

namespace stdex {

inline void sleep(int ms)
{
	if (ms >= 0)
	    std::this_thread::sleep_for(std::chrono::milliseconds(ms));
}

inline int get_host_addr(const string &host, string &addr)
{
    struct hostent *he;
    char buf[256];

    he = gethostbyname(host.c_str());
    if (he == NULL)
        return 1;

    if (he->h_addrtype == AF_INET)
    {
		if (inet_ntop(AF_INET, he->h_addr, buf, sizeof(buf)) == NULL)
			return 2;
    }
    else if (he->h_addrtype == AF_INET6)
    {
		if (inet_ntop(AF_INET6, he->h_addr, buf, sizeof(buf)) == NULL)
			return 3;
    }
    else
    {
        return 4;
    }

    addr = buf;
    return 0;
}

inline string get_current_path()
{
    char buf[1024];

#ifdef _MSC_VER
    GetCurrentDirectory(sizeof(buf), buf);
    return buf;
#else
    getcwd(buf, sizeof(buf));
    return buf;
#endif
}

inline string get_path_separator()
{
#ifdef _MSC_VER
    return "\\";
#else
    return "/";
#endif
}

template<typename Map>
inline void merge(Map &m1, const Map &m2)
{
	for(auto &p: m2)
	{
		if (m1.find(p.first) == m1.end())
		{
			m1.insert(p);
		}
	}
}



}
#endif //STDEX_MISC_H_
