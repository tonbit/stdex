#ifndef STDEX_OPTIONAL_H_
#define STDEX_OPTIONAL_H_

#include "stdex/stdcc.h"
namespace stdex {

template<typename Type>
class optional
{
public:
	typedef Type value_type;

	optional()
	{
		_owned = false;
	}

	optional(const optional &other)
	{
		_owned = other._owned;
		if (_owned)
    		_value = other._value;
	}

	optional(optional &&other)
	{
		_owned = other._owned;
		if (_owned)
    		_value = std::move(other._value);
	}

	optional(const Type &obj)
	{
		_owned = true;
		_value = obj;
	}

	optional(Type &&obj)
	{
		_owned = true;
		_value = std::move(obj);
	}

	optional &operator=(const optional &other)
	{
		_owned = other._owned;
		if (_owned)
    		_value = other._value;
		return *this;
	}

	optional &operator=(optional &&other)
	{
		_owned = other._owned;
		if (_owned)
    		_value = std::move(other._value);
		return *this;
	}

	optional &operator=(const Type &obj)
	{
		_owned = true;
		_value = obj;
		return *this;
	}

	optional &operator=(Type &&obj)
	{
		_owned = true;
		_value = std::move(obj);
		return *this;
	}

	explicit operator bool() const
	{
		return _owned;
	}

	bool has_value() const
	{
		return _owned;
	}

	Type &value() &
	{
		return _value;
	}

	const Type &value() const&
	{
		return _value;
	}

	Type &&value() &&
	{
		return std::move(_value);
	}

	const Type &&value() const&&
	{
		return std::move(_value);
	}

	Type value_or(Type &&def_value) const&
	{
		if (_owned)
			return _value;
		else
			return std::move(def_value);
	}

	Type value_or(Type &&def_value) &&
	{
		if (_owned)
			return std::move(_value);
		else
			return std::move(def_value);
	}

	void swap(optional &other)
	{
		atuo tmp = std::move(other);
		other = std::move(*this);
		*this = std::move(tmp);
	}

	void reset()
	{
		_owned = false;
	}

	const Type* operator->() const
	{
		return &_value;
	}

	Type* operator->()
	{
		return &_value;
	}

	const Type& operator*() const&
	{
		return _value;
	}

	Type& operator*() &
	{
		return _value;
	}

	const Type&& operator*() const&&
	{
		return _value;
	}

	Type&& operator*() &&
	{
		return std::move(_value);
	}

private:
	bool _owned;
	Type _value;
};

}
#endif //STDEX_OPTIONAL_H_
