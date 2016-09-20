/*
 * Licensed to the Apache Software Foundation (ASF) under one
 * or more contributor license agreements. See the NOTICE file
 * distributed with this work for additional information
 * regarding copyright ownership. The ASF licenses this file
 * to you under the Apache License, Version 2.0 (the
 * "License"); you may not use this file except in compliance
 * with the License. You may obtain a copy of the License at
 *
 *   http://www.apache.org/licenses/LICENSE-2.0
 *
 * Unless required by applicable law or agreed to in writing,
 * software distributed under the License is distributed on an
 * "AS IS" BASIS, WITHOUT WARRANTIES OR CONDITIONS OF ANY
 * KIND, either express or implied. See the License for the
 * specific language governing permissions and limitations
 * under the License.
 */

#ifndef STDEX_STDCC_H_
#define STDEX_STDCC_H_

#include <limits.h>
#include <assert.h>
#include <signal.h>
#include <errno.h>
#include <stddef.h>
#include <stdarg.h>
#include <stdlib.h>
#include <stdio.h>
#include <stdint.h>
#include <float.h>
#include <string.h>
#include <time.h>
#include <math.h>
#include <ctype.h>
#include <wctype.h>
#include <locale.h>
#include <wchar.h>

#ifdef _MSC_VER

#pragma warning(disable:4996)
#pragma warning(disable:4200)

#define NOMINMAX
#include <sys/types.h>
#include <sys/timeb.h>
#include <winsock2.h>
#include <Ws2ipdef.h>
#include <Ws2tcpip.h>

typedef struct sockaddr sockaddr_t;
typedef int socklen_t;

#define strdup _strdup
#define snprintf _snprintf
#define atoll _atoi64
#define inet_ntop InetNtop

#pragma comment(lib, "ws2_32.lib")

#else //_MSC_VER

#include <unistd.h>
#include <net/if.h>
#include <sys/types.h>
#include <sys/param.h>
#include <sys/time.h>
#include <sys/timeb.h>
#include <sys/stat.h>
#include <sys/prctl.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>
#include <syslog.h>

typedef struct sockaddr sockaddr_t;

#define ioctlsocket ::ioctl
#define closesocket ::close

#endif

#define OUT
#define INOUT

typedef uint64_t u64;
typedef uint32_t u32;
typedef uint16_t u16;
typedef uint8_t  u8;

typedef int64_t  i64;
typedef int32_t  i32;
typedef int16_t  i16;
typedef int8_t   i8;

typedef float    f32;
typedef double   f64;

typedef unsigned long int ulong;
typedef unsigned short int ushort;

#include <iostream>
#include <iomanip>
#include <sstream>
#include <istream>
#include <ostream>
#include <string>
using std::string;
using std::wstring;

#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>

typedef std::map<string, string> DataMap;

#endif //STDEX_STDCC_H_
