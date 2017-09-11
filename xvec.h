#ifndef STDEX_XVEC_H_
#define STDEX_XVEC_H_

#include "stdex/stdcc.h"

namespace stdex {

class xvec
{
public:
	xvec()
	{
	}

	xvec(const xvec &other)
	{
		_vec = other._vec;
	}

	xvec(xvec &&other)
	{
		_vec = std::move(other._vec);
	}

	xvec(const std::vector<string> &vec)
	{
		_vec = vec;
	}

	xvec(std::vector<string> &&vec)
	{
		_vec = std::move(vec);
	}

	xvec(const string &str)
	{
		parse(str);
	}

	xvec &operator=(const xvec &other)
    {
		_vec = other._vec;
        return *this;
    }

	xvec &operator=(xvec &&other)
    {
		_vec = std::move(other._vec);
        return *this;
    }

	xvec &operator=(const std::vector<string> &vec)
    {
		_vec = vec;
        return *this;
    }

	xvec &operator=(std::vector<string> &&vec)
    {
		_vec = std::move(vec);
        return *this;
    }

	xvec &operator=(const string &str)
    {
		parse(str);
        return *this;
    }

	void append(const xvec &other)
	{
		_vec.insert(_vec.end(), other._vec.begin(), other._vec.end());
	}

	void append(const std::vector<string> &vec)
	{
		_vec.insert(_vec.end(), vec.begin(), vec.end());
	}

	string &operator[](size_t i)
	{
		return _vec[i];
	}

	inline string to_string()
	{
		string tmp;
		tmp.append("[");

		for (auto &s : _vec)
		{
			tmp.append("\"", 1);
			tmp.append(s);
			tmp.append("\",", 2);
		}

		if (tmp.back() == ',')
		{
			tmp.back() = ']';
		}
		else
		{
			tmp.append("]");
		}

		return std::move(tmp);
	}

	int parse(const string &str)
	{
		return parse(str.c_str());
	}

private:
	std::vector<string> _vec;

	int parse_ws(const char *str)
	{
		const char *cur = str;
		char c;

		while ((c = *cur) != '\0')
		{
			if (!isspace(c))
				break;

			++cur;
		}

		return cur - str;
	}

	int parse_str(const char *str)
	{
		const char *cur = str;
		char c;

		while ((c = *cur) != '\0')
		{
			if (c == '"')
				break;

			if (c == '\\')
				++cur;

			++cur;
		}

		return cur - str;
	}

	int parse_num(const char *str)
	{
		char *end;
		strtod(str, &end);
		return end - str;
	}

	int parse(const char *str)
	{
		const char *cur = str;
		int off;

		off = parse_ws(cur);
		cur += off;

		if (*cur == '\0')
			return cur - str;

		if (*cur != '[')
			return -1;
		cur += 1;

		while (*cur)
		{
			string label, value;

			off = parse_ws(cur);
			cur += off;

			if (*cur == 'n')
			{
				if (memcmp("null", cur, 4) != 0)
					return -1;

				value.assign("");
				cur += 4;
			}
			else if (*cur == 't')
			{
				if (memcmp("true", cur, 4) != 0)
					return -1;

				value.assign("true");
				cur += 4;
			}
			else if (*cur == 'f')
			{
				if (memcmp("false", cur, 5) != 0)
					return -1;

				value.assign("false");
				cur += 5;
			}
			else if (*cur == '"')
			{
				cur += 1;

				off = parse_str(cur);
				if (off < 0)
					return -1;

				value.assign(cur, off);
				cur += off;

				if (*cur != '"')
					return -1;
				cur += 1;
			}
			else
			{
				off = parse_num(cur);
				if (off < 0)
					return -1;

				value.assign(cur, off);
				cur += off;
			}

			_vec.push_back(value);

			off = parse_ws(cur);
			cur += off;

			if (*cur != ',')
				break;
			cur += 1;
		}

		if (*cur != ']')
			return -1;
		cur += 1;

		off = parse_ws(cur);
		cur += off;
		return cur - str;
	}

};

}

using xvec_t = stdex::xvec;

#endif //STDEX_XVEC_H_
