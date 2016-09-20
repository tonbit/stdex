#ifndef STDEX_CRYPTED_CONFIG_H_
#define STDEX_CRYPTED_CONFIG_H_
#ifdef STDEX_HAS_OPENSSL

#include "stdex/stdcc.h"
#include "stdex/string.h"
#include "stdex/base64.h"
#include <map>

namespace stdex {

class CryptedConfig
{
public:
    int load(const string &key_path, const string &path)
    {
        FILE *pf = NULL;
        char buf[128];
        string line;

        pf = fopen(path.c_str(), "rb");
        if (!pf)
            return 1;

        while (fgets(buf, sizeof(buf), pf))
        {
			if (strnlen(buf, sizeof(buf)) >= sizeof(buf)-1)
	    	{
	    		line.append(buf);
	    		continue;
	    	}

			line.append(buf);

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

					if (stdex::startof(val, "RSA!"))
					{
						string encrypted = val.substr(4, val.size() - 4);
						string decrypted;
						stdex::rsa_pub_decrypt(key_path, stdex::Base64::decode(encrypted), decrypted);
						_items[key] = decrypted;
 					}
					else
					{
						_items[key] = val;
					}
				}
            }

            line = "";
        }

        fclose(pf);
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
            return atoi(iter->second.c_str());

        return def;
    }

    inline double get(const string &key, double def) const
    {
        auto iter = _items.find(key);

        if (iter != _items.end())
            return atof(iter->second.c_str());

        return def;
    }

    inline std::vector<string> get(const string &key, char sep) const
    {
        auto iter = _items.find(key);

        if (iter != _items.end())
            return split(iter->second, sep);

        return std::vector<string>();
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
#endif //STDEX_HAS_OPENSSL
#endif //STDEX_CRYPTED_CONFIG_H_
