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

#include <cstdint>
#include <cstring>
#include <cstdlib>
#include <cstdarg>
#include <cstdio>
#include <cmath>
#include <csignal>
#include <cassert>
#include <iomanip>
#include <iostream>
#include <sstream>
#include <fstream>
#include <string>

#include <array>
#include <vector>
#include <list>
#include <map>
#include <unordered_map>
#include <set>
#include <unordered_set>
#include <deque>

#ifdef _MSC_VER

#pragma warning(disable:4996)
#pragma warning(disable:4200)
#define NOMINMAX

#include <winsock2.h>
#include <Ws2tcpip.h>
#pragma comment(lib, "ws2_32.lib")

typedef struct sockaddr sockaddr_t;
typedef int socklen_t;

#else //_MSC_VER

#include <unistd.h>
#include <sys/stat.h>
#include <sys/types.h>
#include <sys/socket.h>
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <fcntl.h>

typedef struct sockaddr sockaddr_t;

#define ioctlsocket ::ioctl
#define closesocket ::close

#endif

using std::string;
using std::wstring;

typedef unsigned char byte;
typedef unsigned long ulong;
typedef unsigned short ushort;

#endif //STDEX_STDCC_H_
