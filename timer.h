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
		_passed = 0;
	}

	void stop()
	{
		if (_passed == 0)
			_passed = get_time_tick() - _begin;
	}

	uint64_t passed()
	{
		if (_passed == 0)
			return get_time_tick() - _begin;
		else
			return _passed;
	}

	void reset()
	{
		_begin = get_time_tick();
		_passed = 0;
	}

private:
	uint64_t _begin;
	uint64_t _passed;
};

}
#endif //STDEX_TIMER_H_