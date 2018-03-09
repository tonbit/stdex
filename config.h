#ifndef STDEX_CONFIG_H_
#define STDEX_CONFIG_H_

#include "stdex/stdcc.h"
#include "stdex/string.h"
#include "stdex/crypt.h"
#include "stdex/base64.h"
#include "stdex/file.h"
namespace stdex {

class Config
{
public:
    int load(const string &path, const string &pem="")
    {
        std::vector<string> lines;
        lines = stdex::file::load_lines(path);

        for (auto &line : lines)
        {
            const char *pos = line.c_str();
            int len;

            len = pass_ws(pos);
            pos += len;

            if (pos[0] != 0 && pos[0] != '#' && pos[0] != ';')
            {
				len = get_key(pos);

				if (pos[len] == '=')
				{
					string key;
					string val;

					key.assign(pos, len);
					key = trim(key);

					pos += len+1;

					len = get_value(pos);
					val.assign(pos, len);
					val = trim(val);

					_items[key] = decrypt(pem, val);
				}
            }
        }

        return 0;
    }

    inline void clear()
    {
    	_items.clear();
    }

    inline const string &get(const string &key, const string &def="") const
    {
        auto iter = _items.find(key);

        if (iter != _items.end())
            return iter->second;

        return def;
    }

    inline int get(const string &key, int def) const
    {
        auto iter = _items.find(key);

        if (iter != _items.end())
            return std::stoi(iter->second);

        return def;
    }

    inline double get(const string &key, double def) const
    {
        auto iter = _items.find(key);

        if (iter != _items.end())
            return std::stod(iter->second.c_str());

        return def;
    }

    inline std::vector<string> get(const string &key, char sep) const
    {
        auto iter = _items.find(key);

        if (iter != _items.end())
            return split(iter->second, sep);

        return std::vector<string>();
    }

	string decrypt(const string &pem, const string &value)
	{
		if (!pem.empty() && startof(value, "RSA!"))
		{
#ifdef STDEX_HAS_OPENSSL
			string encrypted = value.substr(4, value.size() - 4);
			string decrypted;
			rsa_pub_decrypt(pem, stdex::Base64::decode(encrypted), decrypted);
			return std::move(decrypted);
#endif
		}

		return value;
	}

private:
    std::map<string, string> _items;

    inline int pass_ws(const char *buf)
    {
        int sum = 0;

        while (*buf != 0 && isspace(*buf))
        {
            ++sum;
            ++buf;
        }

        return sum;
    }

    int get_key(const char *buf)
    {
        int sum = 0;

        while (*buf != 0 && *buf != '=')
        {
            ++sum;
            ++buf;
        }

        return sum;
    }

    int get_value(const char *buf)
    {
        int sum = 0;

        while (*buf != 0 && *buf != '\r' && *buf != '\n')
        {
            ++sum;
            ++buf;
        }

        return sum;
    }

};

}
#endif //STDEX_CONFIG_H_
