#ifndef STDEX_STRING_H_
#define STDEX_STRING_H_

#include "stdex/stdcc.h"

namespace stdex {

inline bool startof(const string &str, const string &sub)
{
	if (str.size() >= sub.size())
	{
		if (memcmp(&str[0], &sub[0], sub.size()) == 0)
			return true;
	}
	return false;
}

inline bool endof(const string &str, const string &sub)
{
	if (str.size() >= sub.size())
	{
		if (memcmp(&str[0]+str.size()-sub.size(), &sub[0], sub.size()) == 0)
			return true;
	}
	return false;
}

inline string ltrim(const string &str)
{
    int i;
    int len = str.length();

    for (i=0; i<len; ++i)
    {
        if (!isspace(str[i]))
            break;
    }

    return str.substr(i, len-i);
}

inline string ltrim(string &&str)
{
    int i;
    int len = str.length();

    for (i=0; i<len; ++i)
    {
        if (!isspace(str[i]))
            break;
    }

    return str.substr(i, len-i);
}

inline string rtrim(const string &str)
{
    int i;
    int len = str.length();

    for (i=len-1; i>=0; i--)
    {
        if (!isspace(str[i]))
            break;
    }

    return str.substr(0, i+1);
}

inline string rtrim(string &&str)
{
    int i;
    int len = str.length();

    for (i=len-1; i>=0; i--)
    {
        if (!isspace(str[i]))
            break;
    }

    return str.substr(0, i+1);
}

inline string trim(const string &str)
{
    int i, j;
    int len = str.length();

    for (i=0; i<len; ++i)
    {
        if (!isspace(str[i]))
            break;
    }

    for (j=len-1; j>=i; --j)
    {
        if (!isspace(str[j]))
            break;
    }

    return str.substr(i, j-i+1);
}

inline string trim(string &&str)
{
    int i, j;
    int len = str.length();

    for (i=0; i<len; ++i)
    {
        if (!isspace(str[i]))
            break;
    }

    for (j=len-1; j>=i; --j)
    {
        if (!isspace(str[j]))
            break;
    }

    return str.substr(i, j-i+1);
}

inline std::vector<string> split(const string &str, char sep)
{
	std::vector<string> vec;

	if (str.empty())
		return vec;

    string::size_type pos1 = 0;
    string::size_type pos2 = 0;
	string::size_type len = str.length();
	while (true)
    {
        if (pos1 >= len)
        {
            vec.push_back("");
            break;
        }

        pos2 = str.find_first_of(sep, pos1);
        if (pos2 == string::npos)
        {
            vec.push_back(trim(str.substr(pos1, string::npos)));
            break;
        }
        else
        {
            vec.push_back(trim(str.substr(pos1, pos2-pos1)));
            pos1 = pos2+1;
        }
    }

    return std::move(vec);
}

inline std::vector<string> split(const string &str, char sep, int maxp)
{
	std::vector<string> vec;

	if (str.empty())
		return vec;

    string::size_type pos1 = 0;
    string::size_type pos2 = 0;
	string::size_type len = str.length();

	while (true)
    {
        if (pos1 >= len)
        {
            vec.push_back("");
            break;
        }

		if (maxp == 1)
		{
			vec.push_back(trim(str.substr(pos1, string::npos)));
            break;
		}

        pos2 = str.find_first_of(sep, pos1);
        if (pos2 == string::npos)
        {
            vec.push_back(trim(str.substr(pos1, string::npos)));
            break;
        }
        else
        {
            vec.push_back(trim(str.substr(pos1, pos2-pos1)));
            pos1 = pos2+1;
			maxp--;
        }
    }

    return std::move(vec);
}

inline string join(const std::vector<string> &vec, char sep)
{
	string str;
    std::vector<string>::const_iterator it;

    it = vec.begin();
    if (it != vec.end())
    {
        str = *it;
        it++;
    }
    else
    {
        return str;
    }

    while (it != vec.end())
    {
        str += sep;
        str += *it;
        it++;
    }

    return std::move(str);
}

inline std::set<string> split_set(const string &str, char sep)
{
	std::set<string> set;

	if (str.empty())
		return set;

    string::size_type pos1 = 0;
    string::size_type pos2 = 0;
	string::size_type len = str.length();

	while (true)
    {
        if (pos1 >= len)
        {
            set.insert("");
            break;
        }

        pos2 = str.find_first_of(sep, pos1);
        if (pos2 == string::npos)
        {
            set.insert(trim(str.substr(pos1, string::npos)));
            break;
        }
        else
        {
            set.insert(trim(str.substr(pos1, pos2-pos1)));
            pos1 = pos2+1;
        }
    }

    return std::move(set);
}

inline string join_set(const std::set<string> &set, char sep)
{
	string str;
    std::set<string>::iterator it;

    it = set.begin();
    if (it != set.end())
    {
        str = *it;
        it++;
    }
    else
    {
        return str;
    }

    while (it != set.end())
    {
        str += sep;
        str += *it;
        it++;
    }

    return std::move(str);
}

inline std::pair<string, string> split_pair(const string &str, char sep)
{
	std::pair<string, string> pair;

	string::size_type pos = str.find_first_of(sep);
	if (pos == string::npos)
	{
		pair = std::make_pair(str, string());
	}
	else
	{
		pair = std::make_pair(str.substr(0, pos), str.substr(pos+1));
	}

	return pair;
}

inline std::map<string, string> split_map(const string &str, char sep1, char sep2)
{
	std::map<string, string> map;

	if (str.empty())
		return map;

    string::size_type pos1 = 0;
    string::size_type pos2 = 0;
	string::size_type len = str.length();

	while (true)
    {
        if (pos1 >= len)
        {
            break;
        }

        pos2 = str.find_first_of(sep1, pos1);
        if (pos2 == string::npos)
        {
            map.insert(split_pair(str.substr(pos1, string::npos), sep2));
            break;
        }
        else
        {
            map.insert(split_pair(str.substr(pos1, pos2-pos1), sep2));
            pos1 = pos2+1;
        }
    }

    return std::move(map);
}

inline string to_string(bool val)
{
    if (val)
        return "true";
    else
        return "false";
}

inline string randstr(size_t len)
{
	const char *seed = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";
	const size_t seed_len = 62; //10+26+26

	string tmp;
	tmp.resize(len);

	for (size_t i=0; i<len; i++)
	{
		tmp[i] = seed[rand() % seed_len];
	}

	return std::move(tmp);
}

inline int hash_code(const string &str)
{
	int x = 0;
	size_t len = str.size();

	for (size_t i=0; i<len; i++)
	{
		x = (x << 5) - x + (int)str[i];
	}

	return std::abs(x);
}

inline string format(const string &fmt, const std::vector<const string *> &args)
{
	string tmp;
	size_t start = 0;
	size_t pos;

	size_t n = args.size();

	for (size_t i = 0; i < n; i++)
	{
		pos = fmt.find("%", start);
		if (pos == string::npos)
		{
			tmp.append(fmt.substr(start));
			break;
		}

		tmp.append(fmt.substr(start, pos-start));
		tmp.append(*args[i]);
		start = pos + 1;
	}

	if (pos != string::npos)
	{
		tmp.append(fmt.substr(start));
	}

	return std::move(tmp);
}

inline string format(const string &fmt, const string &arg1)
{
	return format(fmt, std::vector<const string *>{&arg1});
}

inline string format(const string &fmt, const string &arg1, const string &arg2)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8, &arg9});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8,
													&arg9, &arg10});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10, const string &arg11)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8,
													&arg9, &arg10, &arg11});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10, const string &arg11, const string &arg12)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8,
													&arg9, &arg10, &arg11, &arg12});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10, const string &arg11, const string &arg12, const string &arg13)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8,
													&arg9, &arg10, &arg11, &arg12, &arg13});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10, const string &arg11, const string &arg12, const string &arg13,
	const string &arg14)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8,
													&arg9, &arg10, &arg11, &arg12, &arg13, &arg14});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10, const string &arg11, const string &arg12, const string &arg13,
	const string &arg14, const string &arg15)
{
	return format(fmt, std::vector<const string *>{&arg1, &arg2, &arg3, &arg4, &arg5, &arg6, &arg7, &arg8,
													&arg9, &arg10, &arg11, &arg12, &arg13, &arg14, &arg15});
}

}
#endif //STDEX_STRING_H_
