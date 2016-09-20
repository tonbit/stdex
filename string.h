#ifndef STDEX_STRING_H_
#define STDEX_STRING_H_

#include "stdex/stdcc.h"
#include <vector>

#ifdef STDEX_HAS_OPENSSL
#include "openssl/md5.h"
#include "openssl/sha.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#endif //STDEX_HAS_OPENSSL

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
    string::size_type pos1 = 0;
    string::size_type pos2 = 0;
	string::size_type len = str.length();
	std::vector<string> vec;

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

inline string join(std::vector<string> &vec, char sep)
{
	string str;
    std::vector<string>::iterator it;

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

inline string to_string(bool val)
{
    if (val)
        return "true";
    else
        return "false";
}


#ifdef STDEX_HAS_OPENSSL

inline string to_md5(const string &in)
{
    string str;
    str.resize(32);

    unsigned char out[16];
    MD5((const unsigned char *)in.c_str(), in.length(), out);

    snprintf(&str[0], str.size(), "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        out[0],out[1],out[2],out[3],out[4],out[5],out[6],out[7],out[8],out[9],out[10],out[11],out[12],out[13],out[14],out[15]);

    return std::move(str);
}

inline string to_sha1(const string &in)
{
    string str;
    str.resize(40);

    unsigned char out[20];
    SHA1((const unsigned char *)in.c_str(), in.length(), out);

    snprintf(&str[0], str.size(), "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        out[0],out[1],out[2],out[3],out[4],out[5],out[6],out[7],out[8],out[9],out[10],out[11],out[12],out[13],out[14],out[15],out[16],out[17],out[18],out[19]);

    return std::move(str);
}

inline int ras_pri_encrypt(const string &key_path, const string &raw, string &encrypted)
{
	FILE *key_file;
	key_file = fopen(key_path.c_str(), "rb");
	if (key_file == NULL)
		return 1;

	RSA *rsa;
	rsa = PEM_read_RSAPrivateKey(key_file, NULL, NULL, NULL);
	if (rsa == NULL)
	{
		fclose(key_file);
		return 2;
	}

	int rsa_len = RSA_size(rsa);
	encrypted.resize(rsa_len);

	int ret = RSA_private_encrypt(raw.size(), (const unsigned char *)&raw[0], (unsigned char *)&encrypted[0], rsa, RSA_PKCS1_PADDING);
	if (ret < 0)
	{
		RSA_free(rsa);
		fclose(key_file);
		return 2;
	}

	encrypted.resize(ret);
	RSA_free(rsa);
	fclose(key_file);
	return 0;
}

inline int rsa_pub_decrypt(const string &key_path, const string &encrypted, string &decrypted)
{
	FILE *key_file;
	key_file = fopen(key_path.c_str(), "rb");
	if (key_file == NULL)
		return 1;

	RSA *rsa;
	rsa = PEM_read_RSA_PUBKEY(key_file, NULL, NULL, NULL);
	if (rsa == NULL)
	{
		fclose(key_file);
		return 2;
	}

	int rsa_len = RSA_size(rsa);
	decrypted.resize(rsa_len);

	int ret = RSA_public_decrypt(encrypted.size(), (const unsigned char *)encrypted.c_str(), (unsigned char *)&decrypted[0], rsa, RSA_PKCS1_PADDING);
	if (ret < 0)
	{
		RSA_free(rsa);
		fclose(key_file);
		return 3;
	}

	decrypted.resize(ret);
	RSA_free(rsa);
	fclose(key_file);
	return 0;
}

#endif //STDEX_HAS_OPENSSL

inline string randstr(size_t len)
{
	const char seed[] = "0123456789ABCDEFGHIJKLMNOPQRSTUVWXYZabcdefghijklmnopqrstuvwxyz";

	string tmp;
	tmp.resize(len);

	for (size_t i=0; i<len; i++)
	{
		tmp[i] = seed[rand() % sizeof(seed)];
	}

	return std::move(tmp);
}

inline string format(const string &fmt, const std::vector<string> &args)
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
		tmp.append(args[i]);
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
	return format(fmt, std::vector<string>{arg1});
}

inline string format(const string &fmt, const string &arg1, const string &arg2)
{
	return format(fmt, std::vector<string>{arg1,arg2});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5,arg6});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5,arg6,arg7});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg9});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg10});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10, const string &arg11)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg10,arg11});
}

inline string format(const string &fmt, const string &arg1, const string &arg2, const string &arg3,
	const string &arg4, const string &arg5, const string &arg6, const string &arg7, const string &arg8,
	const string &arg9, const string &arg10, const string &arg11, const string &arg12)
{
	return format(fmt, std::vector<string>{arg1,arg2,arg3,arg4,arg5,arg6,arg7,arg8,arg10,arg11,arg12});
}

}
#endif //STDEX_STRING_H_
