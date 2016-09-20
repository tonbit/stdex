#ifndef STDEX_NUMBER_H_
#define STDEX_NUMBER_H_

#include "stdex/stdcc.h"
namespace stdex {

inline size_t align(size_t num, size_t x=sizeof(void *))
{
    return (num + x - 1) & ~(x - 1);
}

inline u32 lower32(u64 num)
{
    return (u32)(num & 0xffffffff);
}

inline u32 high32(u64 num)
{
    return (u32)(num >> 32);
}

inline u64 make_u64(u32 hi, u32 lw)
{
    return (((u64)hi)<<32) + (u64)lw;
}

}
#endif //STDEX_NUMBER_H_