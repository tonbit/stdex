#ifndef STDEX_FILE_H_
#define STDEX_FILE_H_

#include "stdex/stdcc.h"
#include <dirent.h>

#ifdef _MSC_VER
#include <io.h>
#include <direct.h>
#endif

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

inline int save_data(const string &path, const char *data, size_t size)
{
	std::ofstream file;

	file.open(path, std::ios::out | std::ios::binary);
	if (!file.is_open())
        return 1;

	file.write(data, size);
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

inline string dir_name(const string &path)
{
	size_t len = path.size();
	size_t p1, p2 = len - 1;

	while (path[p2] == '/')
	{
		p2--;
	}

	p1 = p2 - 1;

	while (path[p1] != '/')
	{
		p1--;
	}

	return path.substr(0, p1);
}

inline string base_name(const string &path)
{
	size_t len = path.size();
	size_t p1, p2 = len - 1;

	while (path[p2] == '/')
	{
		p2--;
	}

	p1 = p2 - 1;

	while (path[p1] != '/')
	{
		p1--;
	}

	return path.substr(p1 + 1, p2 - p1);
}

inline int make_dir(const string &dir)
{
#ifdef _MSC_VER

	if (_access(dir.c_str(), 06))
	{
		string updir = dir_name(dir);

		if (!updir.empty())
		{
			if (make_dir(updir.c_str()))
				return 1;
		}

		if (_mkdir(dir.c_str()))
			return 2;
	}

	return 0;

#else

	if (access(dir.c_str(), R_OK|W_OK))
	{
		string updir = dir_name(dir);

		if (!updir.empty())
		{
			if (make_dir(updir.c_str()))
				return 1;
		}

		if (mkdir(dir.c_str(), 0777))
			return 2;
	}

	return 0;

#endif
}

inline int remove_file(const string &src)
{
	return std::remove(src.c_str());
}

inline int copy_file(const string &src, const string &dst)
{
	if (make_dir(dir_name(dst)))
		return 1;

	std::vector<char> data = load_data(src);
	if (data.empty())
		return 2;

	if (save_data(dst, data))
		return 3;

	return 0;
}

inline int move_file(const string &src, const string &dst)
{
	int ret = copy_file(src, dst);
	if (ret)
		return ret;

	if (remove_file(src))
		return 4;

	return 0;
}

}
}
#endif //STDEX_FILE_H_
