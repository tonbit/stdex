#ifndef STDEX_URLCODEC_H_
#define STDEX_URLCODEC_H_

#include "stdex/stdcc.h"
namespace stdex {

class UrlCodec
{
private:
    static char to_hex(byte x)
	{
		const char hex[] = "0123456789ABCDEF";
	    return hex[x];
	}

	static byte from_hex(char x)
	{
	    if (x >= 'A' && x <= 'Z')
	    	return x - 'A' + 10;
	    else if (x >= 'a' && x <= 'z')
	    	return x - 'a' + 10;
	    else if (x >= '0' && x <= '9')
	    	return x - '0';
	    else
	    	return 0;
	}

public:

	static std::string url_encode(const std::string &str)
	{
	    size_t num = str.size();
	    std::string tmp;

	    for (size_t i=0; i<num; i++)
	    {
	        if (isalnum(str[i]) ||
	            (str[i] == '-') ||
	            (str[i] == '_') ||
	            (str[i] == '.') ||
	            (str[i] == '~'))
	            tmp.push_back(str[i]);
	        else if (str[i] == ' ')
	            tmp.push_back('+');
	        else
	        {
	            tmp.push_back('%');
	            tmp.push_back(to_hex((byte)str[i] >> 4));
	            tmp.push_back(to_hex((byte)str[i] % 16));
	        }
	    }
	    return std::move(tmp);
	}

	static std::string url_decode(const std::string &str)
	{
	    size_t num = str.size();
	    std::string tmp;

	    for (size_t i=0; i<num; i++)
	    {
	        if (str[i] == '+')
	        {
	        	tmp.push_back(' ');
	        }
	        else if (str[i] == '%')
	        {
	        	if (num - i < 3)
	        		break;

	            byte high = from_hex(str[i+1]);
	            byte low = from_hex(str[i+2]);
	            tmp.push_back((high<<4) + low);
	            i += 2;
	        }
	        else
	        {
	        	tmp += str[i];
	    	}
	    }
	    return std::move(tmp);
	}


};

}
#endif //STDEX_URLCODEC_H_
