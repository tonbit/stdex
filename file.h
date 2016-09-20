#ifndef STDEX_FILE_H_
#define STDEX_FILE_H_

#include "stdex/stdcc.h"
#include <dirent.h>

namespace stdex {
namespace file {

inline bool file_exist(const string &path)
{
	FILE *pf = fopen(path.c_str(), "r");
	if (!pf)
		return false;

    fclose(pf);
	return true;
}

inline size_t file_size(const string &path)
{
    FILE *pf = fopen(path.c_str(), "rb");
    if (!pf)
        return 0;

    fseek(pf, 0, SEEK_END);
    size_t len = ftell(pf);

    fclose(pf);
    return len;
}

inline std::vector<char> load_data(const string &path)
{
    FILE *pf = NULL;
    std::vector<char> data;

    pf = fopen(path.c_str(), "rb");
	if (!pf)
		return std::vector<char>();

    fseek(pf, 0, SEEK_END);
    size_t len = ftell(pf);
    data.resize(len);

    fseek(pf, 0, SEEK_SET);
    fread(&data[0], 1, len, pf);

    fclose(pf);
    return std::move(data);
}

inline int save_data(const string &path, const std::vector<char> &data)
{
    FILE *pf = NULL;

    pf = fopen(path.c_str(), "wb");
    if (!pf)
        return 1;

    fwrite(&data[0], 1, data.size(), pf);

    fclose(pf);
    return 0;
}

inline std::string load_text(const string &path)
{
    FILE *pf = NULL;
    string text;

    pf = fopen(path.c_str(), "rb");
    if (!pf)
        return string();

    fseek(pf, 0, SEEK_END);
    size_t len = ftell(pf);
    text.resize(len);

    fseek(pf, 0, SEEK_SET);
    fread(&text[0], 1, len, pf);

    fclose(pf);
    return std::move(text);
}

inline int save_text(const string &path, const std::string &text)
{
    FILE *pf = NULL;

    pf = fopen(path.c_str(), "wb");
    if (!pf)
        return 1;

    fwrite(&text[0], 1, text.size(), pf);

    fclose(pf);
    return 0;
}

inline std::vector<string> load_lines(const string &path)
{
    std::vector<string> lines;
	string line;

    FILE *pf = NULL;
    char buf[128];

    pf = fopen(path.c_str(), "rb");
    if (!pf)
        return lines;

    while (fgets(buf, sizeof(buf), pf))
    {
    	if (strnlen(buf, sizeof(buf)) < sizeof(buf)-1)
    	{
			line.append(buf);
			lines.push_back(std::move(line));
    	}
    	else
    	{
			line.append(buf);
    	}
    }

    fclose(pf);
    return std::move(lines);
}

inline int save_lines(const string &path, const std::vector<string> &lines)
{
    FILE *pf = NULL;

    pf = fopen(path.c_str(), "wb");
    if (!pf)
        return 1;

    for (const string &line : lines)
    {
    	fputs(line.c_str(), pf);
    }

    fclose(pf);
    return 0;
}

inline std::vector<string> list_dir(const string &path)
{
	DIR *dir;
    dir = opendir(path.c_str());

    if (dir == NULL)
    	return std::vector<string>();

	std::vector<string> tmp;
    struct dirent *ptr;

    while((ptr = readdir(dir)) != NULL) ///read the list of this dir
    {
		tmp.push_back(ptr->d_name);
    }

    closedir(dir);
    return std::move(tmp);
}

}
}
#endif //STDEX_FILE_H_
