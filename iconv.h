#ifndef STDEX_ICONV_H_
#define STDEX_ICONV_H_

#include "stdex/stdcc.h"

#ifdef _MSC_VER
#include <locale>
#include <codecvt>
#else
#include <iconv.h>
#endif

namespace stdex {

#ifdef _MSC_VER

inline wstring utf8_to_utf16(const string &utf8)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	wstring utf16 = conv.from_bytes(utf8);
	return std::move(utf16);
}

inline string utf16_to_utf8(const wstring &utf16)
{
	std::wstring_convert<std::codecvt_utf8<wchar_t>> conv;
	string utf8 = conv.to_bytes(utf16);
	return std::move(utf8);
}

inline wstring gbk_to_utf16(const string &gbk)
{
#ifdef _MSC_VER
	const char* GBK_LOCALE_NAME = ".936";
#else
	const char* GBK_LOCALE_NAME = "zh_CN.GBK";
#endif

	std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> conv(new std::codecvt_byname<wchar_t, char, std::mbstate_t>(GBK_LOCALE_NAME));
	wstring utf16 = conv.from_bytes(gbk);
    return std::move(utf16);
}

inline string utf16_to_gbk(const wstring &utf16)
{
#ifdef _MSC_VER
	const char* GBK_LOCALE_NAME = ".936";
#else
	const char* GBK_LOCALE_NAME = "zh_CN.GBK";
#endif

	std::wstring_convert<std::codecvt_byname<wchar_t, char, std::mbstate_t>> conv(new std::codecvt_byname<wchar_t, char, std::mbstate_t>(GBK_LOCALE_NAME));
	string gbk = conv.to_bytes(utf16);
	return std::move(gbk);
}

inline string utf8_to_gbk(const string &utf8)
{
	return utf16_to_gbk(utf8_to_utf16(utf8));
}

inline string gbk_to_utf8(const string &gbk)
{
	return utf16_to_utf8(gbk_to_utf16(gbk));
}

inline string utf8_to_locale(const string &utf8)
{
	return utf16_to_gbk(utf8_to_utf16(utf8));
}

#else

inline wstring utf8_to_utf16(const string &utf8)
{
	wstring utf16;
	utf16.resize(utf8.size()*2);

	char *inbuf = (char *)&utf8[0];
	size_t inlen = utf8.size();
	char *outbuf = (char *)&utf16[0];
	size_t outlen = utf16.size()*2;

	iconv_t conv = iconv_open("UTF-16", "UTF-8");
	int rc = iconv(conv, &inbuf, &inlen, &outbuf, &outlen);
	iconv_close(conv);

	if (rc < 0)
		return wstring();

	utf16.resize((utf16.size()*2-outlen)/2);
	return std::move(utf16);
}

inline string utf16_to_utf8(const wstring &utf16)
{
	string utf8;
	utf8.resize(utf16.size()*6);

	char *inbuf = (char *)&utf16[0];
	size_t inlen = utf16.size()*2;
	char *outbuf = (char *)&utf8[0];
	size_t outlen = utf8.size();

	iconv_t conv = iconv_open("UTF-8", "UTF-16");
	int rc = iconv(conv, &inbuf, &inlen, &outbuf, &outlen);
	iconv_close(conv);

	if (rc < 0)
		return string();

	utf8.resize(utf8.size()-outlen);
	return std::move(utf8);
}

inline wstring gbk_to_utf16(const string &gbk)
{
	wstring utf16;
	utf16.resize(gbk.size()*2);

	char *inbuf = (char *)&gbk[0];
	size_t inlen = gbk.size();
	char *outbuf = (char *)&utf16[0];
	size_t outlen = utf16.size()*2;

	iconv_t conv = iconv_open("UTF-16", "GBK");
	int rc = iconv(conv, &inbuf, &inlen, &outbuf, &outlen);
	iconv_close(conv);

	if (rc < 0)
		return wstring();

	utf16.resize((utf16.size()*2-outlen)/2);
	return std::move(utf16);
}

inline string utf16_to_gbk(const wstring &utf16)
{
	string gbk;
	gbk.resize(utf16.size()*2);

	char *inbuf = (char *)&utf16[0];
	size_t inlen = utf16.size()*2;
	char *outbuf = (char *)&gbk[0];
	size_t outlen = gbk.size();

	iconv_t conv = iconv_open("GBK", "UTF-16");
	int rc = iconv(conv, &inbuf, &inlen, &outbuf, &outlen);
	iconv_close(conv);

	if (rc < 0)
		return string();

	gbk.resize(gbk.size()-outlen);
	return std::move(gbk);
}

inline string utf8_to_gbk(const string &utf8)
{
	string gbk;
	gbk.resize(utf8.size()*2);

	char *inbuf = (char *)&utf8[0];
	size_t inlen = utf8.size();
	char *outbuf = (char *)&gbk[0];
	size_t outlen = gbk.size();

	iconv_t conv = iconv_open("GBK", "UTF-8");
	int rc = iconv(conv, &inbuf, &inlen, &outbuf, &outlen);
	iconv_close(conv);

	if (rc < 0)
		return string();

	gbk.resize(gbk.size()-outlen);
	return std::move(gbk);
}

inline string gbk_to_utf8(const string &gbk)
{
	string utf8;
	utf8.resize(gbk.size()*6);

	char *inbuf = (char *)&gbk[0];
	size_t inlen = gbk.size();
	char *outbuf = (char *)&utf8[0];
	size_t outlen = utf8.size();

	iconv_t conv = iconv_open("UTF-8", "GBK");
	int rc = iconv(conv, &inbuf, &inlen, &outbuf, &outlen);
	iconv_close(conv);

	if (rc < 0)
		return string();

	utf8.resize(utf8.size()-outlen);
	return std::move(utf8);
}

inline string utf8_to_locale(const string &utf8)
{
	return utf8;
}

#endif
}
#endif //STDEX_ICONV_H_
