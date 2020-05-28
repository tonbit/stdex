#ifndef STDEX_XMAP_H_
#define STDEX_XMAP_H_

#include "stdex/stdcc.h"

namespace stdex {

class xmap
{
public:
	xmap()
	{
	}

	xmap(const xmap &other)
	{
		_map = other._map;
	}

	xmap(xmap &&other)
	{
		_map = std::move(other._map);
	}

	xmap(const std::map<string,string> &map)
	{
		_map = map;
	}

	xmap(std::map<string,string> &&map)
	{
		_map = std::move(map);
	}

	xmap(const string &str)
	{
		parse(str);
	}

	xmap &operator=(const xmap &other)
    {
		_map = other._map;
        return *this;
    }

	xmap &operator=(xmap &&other)
    {
		_map = std::move(other._map);
        return *this;
    }

	xmap &operator=(const std::map<string,string> &map)
    {
		_map = map;
        return *this;
    }

	xmap &operator=(std::map<string,string> &&map)
    {
		_map = std::move(map);
        return *this;
    }

	xmap &operator=(const string &str)
    {
		parse(str);
        return *this;
    }

	void merge(const xmap &other)
	{
		for(auto &p: other._map)
		{
			_map.insert(p);
		}
	}

	void merge(xmap &&other)
	{
		for(auto &p: other._map)
		{
			_map.insert(std::move(p));
		}
	}

	void merge(const std::map<string,string> &map)
	{
		for(auto &p: map)
		{
			_map.insert(p);
		}
	}

	void cover(const xmap &other)
	{
		for(auto &p: other._map)
		{
			_map[p.first] = p.second;
		}
	}

	void cover(xmap &&other)
	{
		for(auto &p: other._map)
		{
			_map[std::move(p.first)] = std::move(p.second);
		}
	}

	void cover(const std::map<string,string> &map)
	{
		for(auto &p: map)
		{
			_map[p.first] = p.second;
		}
	}

	bool contain(const string &key)
	{
		if (_map.find(key) != _map.end())
			return true;
		else
			return false;
	}

	string &operator[](const string &key)
	{
		return _map[key];
	}

	string &at(const string &key)
	{
		return _map[key];
	}

	inline string to_string()
	{
		string tmp;
		tmp.append("{");

		for (auto &p : _map)
		{
			tmp.append("\"", 1);
			tmp.append(p.first);
			tmp.append("\":", 2);
			tmp.append("\"", 1);
			tmp.append(p.second);
			tmp.append("\",", 2);
		}

		if (tmp.back() == ',')
		{
			tmp.back() = '}';
		}
		else
		{
			tmp.append("}");
		}

		return std::move(tmp);
	}

	int parse(const string &str)
	{
		return parse(str.c_str());
	}

	void clear()
	{
		_map.clear();
	}

private:
	std::map<string, string> _map;

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

		if (*cur != '{')
			return -1;
		cur += 1;

		while (*cur)
		{
			string label, value;

			off = parse_ws(cur);
			cur += off;

			if (*cur == '}')
				break;

			if (*cur != '"')
				return -1;
			cur += 1;

			off = parse_str(cur);
			if (off < 0)
				return -1;

			label.assign(cur, off);
			cur += off;

			if (*cur != '"')
				return -1;
			cur += 1;

			off = parse_ws(cur);
			cur += off;

			if (*cur != ':')
				return -1;
			cur += 1;

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

			_map[label] = value;

			off = parse_ws(cur);
			cur += off;

			if (*cur != ',')
				break;
			cur += 1;
		}

		if (*cur != '}')
			return -1;
		cur += 1;

		off = parse_ws(cur);
		cur += off;
		return cur - str;
	}



};

}

using xmap_t = stdex::xmap;

#endif //STDEX_XMAP_H_
