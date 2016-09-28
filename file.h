#ifndef STDEX_FILE_H_
#define STDEX_FILE_H_

#include "stdex/stdcc.h"
#include <dirent.h>
namespace stdex {
namespace file {

inline bool file_exist(const string &path)
{
	std::ifstream file;
	file.open(path);

	if (!file.is_open())
		return false;

    file.close();
	return true;
}

inline size_t file_size(const string &path)
{
	std::ifstream file;
	file.open(path);

	if (!file.is_open())
		return 0;

	file.seekg(0, std::ios_base::end);
	std::streamoff size = file.tellg();

    file.close();
    return static_cast<size_t>(size);
}

inline std::vector<char> load_data(const string &path)
{
	std::ifstream file;
	std::vector<char> data;

	file.open(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
		return std::move(data);

	file.seekg(0, std::ios_base::end);
	std::streamoff size = file.tellg();
    data.resize(static_cast<size_t>(size));

	file.seekg(0, std::ios_base::beg);
	file.read(&data[0], size);
	file.close();
	return std::move(data);
}

inline int save_data(const string &path, const std::vector<char> &data)
{
	std::ofstream file;

	file.open(path, std::ios::out | std::ios::binary);
	if (!file.is_open())
        return 1;

	file.write(&data[0], data.size());
	file.close();
    return 0;
}

inline std::string load_text(const string &path)
{
	std::ifstream file;
	string text;

	file.open(path, std::ios::in | std::ios::binary);
	if (!file.is_open())
        return std::move(text);

	file.seekg(0, std::ios_base::end);
	std::streamoff size = file.tellg();
	text.resize(static_cast<size_t>(size));

	file.seekg(0, std::ios_base::beg);
	file.read(&text[0], size);
	file.close();
    return std::move(text);
}

inline int save_text(const string &path, const std::string &text)
{
	std::ofstream file;

	file.open(path, std::ios::out | std::ios::binary);
	if (!file.is_open())
		return 1;

	file.write(&text[0], text.size());
	file.close();
	return 0;
}

inline std::vector<string> load_lines(const string &path)
{
	std::ifstream file;
	std::vector<string> lines;

	file.open(path);
	if (!file.is_open())
		return std::move(lines);

    char buf[128];
	string line;

    while (!file.eof())
    {
		file.getline(buf, sizeof(buf));
		line.append(buf);

		if (file.rdstate() == std::ios_base::failbit)
		{
			file.clear();
			continue;
		}

		lines.push_back(std::move(line));
    }

	file.close();
    return std::move(lines);
}

inline int save_lines(const string &path, const std::vector<string> &lines)
{
	std::ofstream file;

	file.open(path);
	if (!file.is_open())
		return 1;

    for (const string &line : lines)
		file << line << '\n';

	file.close();
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

    while((ptr = readdir(dir)) != NULL) //read the list of this dir
    {
		tmp.push_back(ptr->d_name);
    }

    closedir(dir);
    return std::move(tmp);
}

}
}
#endif //STDEX_FILE_H_
