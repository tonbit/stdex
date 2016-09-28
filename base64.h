#ifndef STDEX_BASE64_H_
#define STDEX_BASE64_H_

#include "stdex/stdcc.h"
namespace stdex {

class Base64
{
public:
    static string decode(const string &b64)
    {
        const char bs64DecMap[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1, -1, 63,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
            -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, -1,
            -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1
        };

    	const char *txt = b64.c_str();
    	size_t len = b64.length();

    	len = len & ~3;

    	string str(len/4*3, 0);
    	size_t idx = 0;

        for (size_t i=0; i<len-4; i+=4)
        {
            str[idx+0] = bs64DecMap[(int)txt[0]]<<2 | bs64DecMap[(int)txt[1]]>>4;
            str[idx+1] = bs64DecMap[(int)txt[1]]<<4 | bs64DecMap[(int)txt[2]]>>2;
            str[idx+2] = bs64DecMap[(int)txt[2]]<<6 | bs64DecMap[(int)txt[3]];

            txt += 4;
            idx += 3;
        }

        if (txt[2] == '=' && txt[3] == '=')
        {
            str[idx+0] = bs64DecMap[(int)txt[0]]<<2 | bs64DecMap[(int)txt[1]]>>4;
            str.erase(idx+1);
        }
        else if (txt[3] == '=')
        {
            str[idx+0] = bs64DecMap[(int)txt[0]]<<2 | bs64DecMap[(int)txt[1]]>>4;
            str[idx+1] = bs64DecMap[(int)txt[1]]<<4 | bs64DecMap[(int)txt[2]]>>2;
            str.erase(idx+2);
        }
        else
        {
            str[idx+0] = bs64DecMap[(int)txt[0]]<<2 | bs64DecMap[(int)txt[1]]>>4;
            str[idx+1] = bs64DecMap[(int)txt[1]]<<4 | bs64DecMap[(int)txt[2]]>>2;
            str[idx+2] = bs64DecMap[(int)txt[2]]<<6 | bs64DecMap[(int)txt[3]];
        }

        return std::move(str);
    }

    static string encode(const string &str)
    {
        const char bs64EncMap[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        const byte *txt = (const byte *)&str[0];
        size_t len = str.length();

        string b64((len+2)/3*4, 0);
        size_t idx = 0;

        while (len >= 3)
        {
            b64[idx+0] = bs64EncMap[txt[0]>>2];
            b64[idx+1] = bs64EncMap[((txt[0]<<4)|(txt[1]>>4))&0x3F];
            b64[idx+2] = bs64EncMap[((txt[1]<<2)|(txt[2]>>6))&0x3F];
            b64[idx+3] = bs64EncMap[txt[2]&0x3F];

            txt += 3;
            len -= 3;
            idx += 4;
        }

        if (len == 1)
        {
            b64[idx+0] = bs64EncMap[txt[0]>>2];
            b64[idx+1] = bs64EncMap[(txt[0]<<4)&0x3F];
            b64[idx+2] = '=';
            b64[idx+3] = '=';
        }
        else if (len == 2)
        {
            b64[idx+0] = bs64EncMap[txt[0]>>2];
            b64[idx+1] = bs64EncMap[((txt[0]<<4)|(txt[1]>>4))&0x3F];
            b64[idx+2] = bs64EncMap[(txt[1]<<2)&0x3F];
            b64[idx+3] = '=';
        }

        return std::move(b64);
    }

    static string encode(const std::vector<char> &str)
    {
        const char bs64EncMap[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789+/";

        const byte *txt = (const byte *)&str[0];
        size_t len = str.size();

        string b64((len+2)/3*4, 0);
        size_t idx = 0;

        while (len >= 3)
        {
            b64[idx+0] = bs64EncMap[(txt[0])>>2];
            b64[idx+1] = bs64EncMap[((txt[0]<<4)|(txt[1]>>4))&0x3F];
            b64[idx+2] = bs64EncMap[((txt[1]<<2)|(txt[2]>>6))&0x3F];
            b64[idx+3] = bs64EncMap[txt[2]&0x3F];

            txt += 3;
            len -= 3;
            idx += 4;
        }

        if (len == 1)
        {
            b64[idx+0] = bs64EncMap[txt[0]>>2];
            b64[idx+1] = bs64EncMap[(txt[0]<<4)&0x3F];
            b64[idx+2] = '=';
            b64[idx+3] = '=';
        }
        else if (len == 2)
        {
            b64[idx+0] = bs64EncMap[txt[0]>>2];
            b64[idx+1] = bs64EncMap[((txt[0]<<4)|(txt[1]>>4))&0x3F];
            b64[idx+2] = bs64EncMap[(txt[1]<<2)&0x3F];
            b64[idx+3] = '=';
        }

        return std::move(b64);
    }

    //for uri safe
    static string decode4safe(const string &b64)
    {
        const char bs64DecMap2[256] = {
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, 62, -1, -1,
            52, 53, 54, 55, 56, 57, 58, 59, 60, 61, -1, -1, -1, -1, -1, -1,
            -1,  0,  1,  2,  3,  4,  5,  6,  7,  8,  9, 10, 11, 12, 13, 14,
            15, 16, 17, 18, 19, 20, 21, 22, 23, 24, 25, -1, -1, -1, -1, 63,
            -1, 26, 27, 28, 29, 30, 31, 32, 33, 34, 35, 36, 37, 38, 39, 40,
            41, 42, 43, 44, 45, 46, 47, 48, 49, 50, 51, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1,
            -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1, -1 };

        const char *txt = b64.c_str();
        size_t len = b64.length();

        len = len & ~3;

        string str(len/4*3, 0);
        size_t idx = 0;

        for (size_t i=0; i<len-4; i+=4)
        {
            str[idx+0] = bs64DecMap2[(int)txt[0]]<<2 | bs64DecMap2[(int)txt[1]]>>4;
            str[idx+1] = bs64DecMap2[(int)txt[1]]<<4 | bs64DecMap2[(int)txt[2]]>>2;
            str[idx+2] = bs64DecMap2[(int)txt[2]]<<6 | bs64DecMap2[(int)txt[3]];

            txt += 4;
            idx += 3;
        }

        if (b64.length()-len == 1)
        {
            str[idx+0] = bs64DecMap2[(int)txt[0]]<<2 | bs64DecMap2[(int)txt[1]]>>4;
            str.erase(idx+1);
        }
        else if (b64.length()-len == 2)
        {
            str[idx+0] = bs64DecMap2[(int)txt[0]]<<2 | bs64DecMap2[(int)txt[1]]>>4;
            str[idx+1] = bs64DecMap2[(int)txt[1]]<<4 | bs64DecMap2[(int)txt[2]]>>2;
            str.erase(idx+2);
        }
        else {
            str[idx+0] = bs64DecMap2[(int)txt[0]]<<2 | bs64DecMap2[(int)txt[1]]>>4;
            str[idx+1] = bs64DecMap2[(int)txt[1]]<<4 | bs64DecMap2[(int)txt[2]]>>2;
            str[idx+2] = bs64DecMap2[(int)txt[2]]<<6 | bs64DecMap2[(int)txt[3]];
        }

        return std::move(str);
    }

    static string encode4safe(const string &str)
    {
        const char bs64EncMap2[] =
            "ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

        const byte *txt = (const byte *)&str[0];
        size_t len = str.length();

        string b64((len+2)/3*4, 0);
        size_t idx = 0;

        while (len >= 3)
        {
            b64[idx+0] = bs64EncMap2[(int)txt[0]>>2];
            b64[idx+1] = bs64EncMap2[((txt[0]<<4)|(txt[1]>>4))&0x3F];
            b64[idx+2] = bs64EncMap2[((txt[1]<<2)|(txt[2]>>6))&0x3F];
            b64[idx+3] = bs64EncMap2[(int)txt[2]&0x3F];

            txt += 3;
            len -= 3;
            idx += 4;
        }

        if (len == 1)
        {
            b64[idx+0] = bs64EncMap2[(int)txt[0]>>2];
            b64[idx+1] = bs64EncMap2[(txt[0]<<4)&0x3F];
            b64.erase(idx+2);
        }
        else if (len == 2)
        {
            b64[idx+0] = bs64EncMap2[(int)txt[0]>>2];
            b64[idx+1] = bs64EncMap2[((txt[0]<<4)|(txt[1]>>4))&0x3F];
            b64[idx+2] = bs64EncMap2[(txt[1]<<2)&0x3F];
            b64.erase(idx+3);
        }

        return std::move(b64);
    }

	static string encode4safe(const std::vector<char> &str)
	{
		const char bs64EncMap2[] =
			"ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz0123456789-_";

        const byte *txt = (const byte *)&str[0];
		size_t len = str.size();

		string b64((len+2)/3*4, 0);
		size_t idx = 0;

		while (len >= 3)
		{
			b64[idx+0] = bs64EncMap2[(int)txt[0]>>2];
			b64[idx+1] = bs64EncMap2[((txt[0]<<4)|(txt[1]>>4))&0x3F];
			b64[idx+2] = bs64EncMap2[((txt[1]<<2)|(txt[2]>>6))&0x3F];
			b64[idx+3] = bs64EncMap2[(int)txt[2]&0x3F];

			txt += 3;
			len -= 3;
			idx += 4;
		}

		if (len == 1)
		{
			b64[idx+0] = bs64EncMap2[(int)txt[0]>>2];
			b64[idx+1] = bs64EncMap2[(txt[0]<<4)&0x3F];
			b64.erase(idx+2);
		}
		else if (len == 2)
		{
			b64[idx+0] = bs64EncMap2[(int)txt[0]>>2];
			b64[idx+1] = bs64EncMap2[((txt[0]<<4)|(txt[1]>>4))&0x3F];
			b64[idx+2] = bs64EncMap2[(txt[1]<<2)&0x3F];
			b64.erase(idx+3);
		}

		return std::move(b64);
	}

};

}
#endif //STDEX_BASE64_H_
