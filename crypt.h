#ifdef STDEX_HAS_OPENSSL
#ifndef STDEX_CRYPT_H_
#define STDEX_CRYPT_H_

#include "stdex/stdcc.h"
#include "openssl/md5.h"
#include "openssl/sha.h"
#include <openssl/rsa.h>
#include <openssl/pem.h>
#include <openssl/err.h>

namespace stdex {

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