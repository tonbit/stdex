#ifndef STDEX_NUMBER_H_
#define STDEX_NUMBER_H_

#include "stdex/stdcc.h"
namespace stdex {

inline size_t align(size_t num, size_t x=sizeof(void *))
{
    return (num + x - 1) & ~(x - 1);
}

inline uint32_t lower32(uint64_t num)
{
    return (uint32_t)(num & 0xffffffff);
}

inline uint32_t high32(uint64_t num)
{
    return (uint32_t)(num >> 32);
}

inline uint64_t make_u64(uint32_t hi, uint32_t lw)
{
    return (((uint64_t)hi)<<32) + (uint64_t)lw;
}

}
#endif //STDEX_NUMBER_H_