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

inline string get_local_addr()
{
    int sock_fd = socket(AF_INET, SOCK_STREAM, 0);
    struct sockaddr_in remote_addr;
    struct sockaddr_in local_addr;
    char *local_ip = NULL;
    socklen_t len = 0;

    remote_addr.sin_family = AF_INET;
    remote_addr.sin_port = htons(80);
    remote_addr.sin_addr.s_addr = inet_addr("116.62.90.150");

    len =  sizeof(struct sockaddr_in);
    int status = connect(sock_fd, (struct sockaddr*)&remote_addr, len);
    if(status != 0 ){
        printf("connect err: %d\n", status);
        return string();
    }

    getsockname(sock_fd, (struct sockaddr*)&local_addr, &len);

    local_ip = inet_ntoa(local_addr.sin_addr);
    if(local_ip == nullptr)
    {
        printf("inet_ntoa err: %d\n", errno);
        return string();
    }

    return local_ip;
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

inline int get_cpu_num()
{
#ifdef _MSC_VER

	SYSTEM_INFO info;
	GetSystemInfo(&info);
	return info.dwNumberOfProcessors;

#else

	return sysconf(_SC_NPROCESSORS_ONLN);

#endif
}


}
#endif //STDEX_MISC_H_
