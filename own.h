#ifndef STDEX_OWN_H_
#define STDEX_OWN_H_

#include "stdex/stdcc.h"
namespace stdex {

template<class TYPE>
class own
{
public:
	own()
	{
		_owned = false;
	}

	own(const own &other)
	{
		_owned = other._owned;
		if (_owned)
    		_object = other._object;
	}

	own(own &&other)
	{
		_owned = other.owned;
		if (_owned)
    		_object = std::move(other.object);
	}

	own(const TYPE &obj)
	{
		_owned = true;
		_object = obj;
	}

	own(TYPE &&obj)
	{
		_owned = true;
		_object = std::move(obj);
	}

	own &operator=(const own &other)
	{
		_owned = other._owned;
		if (_owned)
    		_object = other._object;
		return *this;
	}

	own &operator=(own &&other)
	{
		_owned = other.owned;
		if (_owned)
    		_object = std::move(other.object);
		return *this;
	}

	own &operator=(const TYPE &obj)
	{
		_owned = true;
		_object = obj;
		return *this;
	}

	own &operator=(TYPE &&obj)
	{
		_owned = true;
		_object = std::move(obj);
		return *this;
	}

	bool is_owned() const
	{
		return _owned;
	}

	const TYPE &get_refer() const
	{
		return _object;
	}

private:
	TYPE _object;
	bool _owned;
};

}
#endif //STDEX_OWN_H_
