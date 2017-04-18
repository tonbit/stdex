#ifndef STDEX_TIMER_H_
#define STDEX_TIMER_H_

#include "stdex/stdcc.h"
#include "stdex/time.h"
namespace stdex {

class Chrono
{
public:
	Chrono()
	{
		_begin = get_time_tick();
	}

	uint64_t passed()
	{
		return get_time_tick() - _begin();
	}

	void reset()
	{
		_begin = get_time_tick();
	}

private:
	uint64_t _begin;
};

}
#endif //STDEX_TIMER_H_