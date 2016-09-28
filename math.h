#ifndef STDEX_MATH_H_
#define STDEX_MATH_H_

#include "stdex/stdcc.h"
namespace stdex {

inline long lceil(float arg)
{
	return static_cast<long>(std::ceil(arg));
}

inline long lceil(double arg)
{
	return static_cast<long>(std::ceil(arg));
}

inline long lfloor(float arg)
{
	return static_cast<long>(std::floor(arg));
}

inline long lfloor(double arg)
{
	return static_cast<long>(std::floor(arg));
}

}
#endif //STDEX_MATH_H_
