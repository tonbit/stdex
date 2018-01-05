#ifdef STDEX_HAS_OPENSSL
#ifndef STDEX_CRYPT_H_
#define STDEX_CRYPT_H_

#include "stdex/stdcc.h"
#include "openssl/md5.h"
#include "openssl/sha.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>
#include <openssl/hmac.h>

namespace stdex {

inline string to_md5_raw(const string &in)
{
    string str;
    str.resize(16);

    MD5((const unsigned char *)in.c_str(), in.length(), (unsigned char *)&str[0]);
    return std::move(str);
}

inline string to_md5_raw(const std::vector<char> &in)
{
	string str;
	str.resize(16);

	MD5((const unsigned char *)&in[0], in.size(), (unsigned char *)&str[0]);
	return std::move(str);
}

inline string to_md5(const string &in)
{
    char out[64];
    unsigned char raw[16];
    MD5((const unsigned char *)in.c_str(), in.length(), raw);

    snprintf(out, sizeof(out), "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        raw[0],raw[1],raw[2],raw[3],raw[4],raw[5],raw[6],raw[7],raw[8],raw[9],raw[10],raw[11],raw[12],raw[13],raw[14],raw[15]);

    return string(out);
}

inline string to_sha1_raw(const string &in)
{
    string str;
    str.resize(20);

    SHA1((const unsigned char *)in.c_str(), in.length(), (unsigned char *)&str[0]);
    return std::move(str);
}

inline string to_sha1(const string &in)
{
    char out[64];
    unsigned char raw[20];
    SHA1((const unsigned char *)in.c_str(), in.length(), raw);

    snprintf(out, sizeof(out), "%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x%02x",
        raw[0],raw[1],raw[2],raw[3],raw[4],raw[5],raw[6],raw[7],raw[8],raw[9],raw[10],raw[11],raw[12],raw[13],raw[14],raw[15],raw[16],raw[17],raw[18],raw[19]);

    return string(out);
}

inline string hmac_sha1(const string &key, const string &data)
{
	string str;
    str.resize(20);
	unsigned int len = 20;
	HMAC(EVP_sha1(), key.c_str(), key.size(),
		(const unsigned char *)data.c_str(), data.size(),
		(unsigned char *)&str[0], &len);
	return std::move(str);
}

inline int rsa_pri_encrypt(const string &key_path, const string &raw, string &encrypted)
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

}
#endif //STDEX_STRING_H_
#endif //STDEX_HAS_OPENSSL
