#ifndef STDEX_OPTION_H_
#define STDEX_OPTION_H_

#include "stdex/stdcc.h"
#include "stdex/string.h"
namespace stdex {

//usage: app [options]
//option: -key [value]

class Option
{
public:

	Option()
	{
	}

	Option(int argc, char *argv[])
	{
		parse(argc, argv);
	}

    void parse(int argc, char *argv[])
    {
		_application = argv[0];

		for (int i=1; i<argc; i++)
		{
			if (argv[i][0] == '-')
			{
				if (i+1 < argc && argv[i+1][0] != '-')
				{
					_arguments[argv[i] + 1] = argv[i+1];
					i++;
				}
				else
				{
					_arguments[argv[i] + 1] = "";
				}
			}
		}
    }

    inline void clear()
    {
    	_application = "";
    	_arguments.clear();
    }

    inline const string &get(const string &key, const string &def) const
    {
        auto iter = _arguments.find(key);

        if (iter != _arguments.end())
            return iter->second;

        return def;
    }

    inline int get(const string &key, int def) const
    {
        auto iter = _arguments.find(key);

        if (iter != _arguments.end())
            return atoi(iter->second.c_str());

        return def;
    }

    inline double get(const string &key, double def) const
    {
        auto iter = _arguments.find(key);

        if (iter != _arguments.end())
            return atof(iter->second.c_str());

        return def;
    }

    inline std::vector<string> get(const string &key, char sep) const
    {
        auto iter = _arguments.find(key);

        if (iter != _arguments.end())
            return split(iter->second, sep);

        return std::vector<string>();
    }

	inline const bool exist(const string &key) const
	{
		if (_arguments.find(key) != _arguments.end())
			return true;

		return false;
	}

private:
	string _application;
    std::map<string, string> _arguments;
};

}
#endif //STDEX_OPTION_H_
