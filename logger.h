#ifndef STDEX_LOGGER_H_
#define STDEX_LOGGER_H_

#include "stdex/stdcc.h"
#include <sys/timeb.h>
#include <atomic>
#include <mutex>
#include <thread>
#include "string.h"

namespace stdex {

class Logger
{
public:
	Logger()
    {
        _level = 7;

#ifdef _DEBUG
		_debug = true;
#else
		_debug = false;
#endif
    }

	~Logger()
    {
        close();
    }

	int open(const string &path)
	{
        _file.open(path, std::ios::app);
        if (!_file)
            return 1;

        return 0;
    }

	void close()
    {
        if (_file.is_open())
            _file.close();
    }

    void set_debug(bool debug)
    {
        _debug = debug;
    }

    void set_level(int level)
    {
        _level = level;
    }

    int get_level()
    {
    	return _level;
    }

    void log_fatal(const char *format, ...)
    {
    	error_count++;

        if (_level < 0)
            return;

        va_list args;
        va_start(args, format);
        write(0, "FTL", format, args);
        va_end(args);
    }

    void log_emerg(const char *format, ...)
    {
    	error_count++;

        if (_level < 1)
            return;

        va_list args;
        va_start(args, format);
        write(1, "EMG", format, args);
        va_end(args);
    }

    void log_alert(const char *format, ...)
    {
    	error_count++;

        if (_level < 2)
            return;

        va_list args;
        va_start(args, format);
        write(2, "ALT", format, args);
        va_end(args);
    }

    void log_stat(const char *format, ...)
    {
        if (_level < 2)
            return;

        va_list args;
        va_start(args, format);
        write(2, "STA", format, args);
        va_end(args);
    }

    void log_error(const char *format, ...)
    {
    	error_count++;

        if (_level < 3)
            return;

        va_list args;
        va_start(args, format);
        write(3, "ERR", format, args);
        va_end(args);
    }

    void log_warn(const char *format, ...)
    {
    	warn_count++;

        if (_level < 4)
            return;

        va_list args;
        va_start(args, format);
        write(4, "WRN", format, args);
        va_end(args);
    }

    void log_notice(const char *format, ...)
    {
        if (_level < 5)
            return;

        va_list args;
        va_start(args, format);
        write(5, "NTC", format, args);
        va_end(args);
    }

    void log_info(const char *format, ...)
    {
        if (_level < 6)
            return;

        va_list args;
        va_start(args, format);
        write(6, "INF", format, args);
        va_end(args);
    }

    void log_debug(const char *format, ...)
    {
        if (_level < 7)
            return;

        va_list args;
        va_start(args, format);
        write(7, "DBG", format, args);
        va_end(args);
    }

	void log_verbose(const char *format, ...)
	{
		if (_level < 8)
			return;

		va_list args;
		va_start(args, format);
		write(8, "VBS", format, args);
		va_end(args);
	}

    void reset_count()
    {
    	error_count = 0;
    	warn_count = 0;
    }

    int get_error_count()
    {
    	return error_count;
    }

    int get_warn_count()
    {
    	return warn_count;
    }

	void flush()
	{
		_file << std::flush;
	}

private:
    std::ofstream _file;
    int _level;
    bool _debug;
    std::atomic<int> error_count;
    std::atomic<int> warn_count;

	void write(int level, const char *title, const char *fmt, va_list args)
	{
		if (!_debug && !_file)
			return;

		char buf[4096];
		int ret;

		char *ptr = buf;
		int len = sizeof(buf);

#ifdef _MSC_VER
		struct _timeb tb;
		_ftime(&tb);
#else
		struct timeb tb;
		ftime(&tb);
#endif

		ret = strftime(ptr, len, "\n%Y%m%d %H:%M:%S.", localtime(&tb.time));
		ptr += ret;
		len -= ret;

		std::stringstream tid;
		tid << std::this_thread::get_id();

		ret = snprintf(ptr, len, "%03d %s %s: ", tb.millitm, tid.str().c_str(), title);
		ptr += ret;
		len -= ret;

		ret = vsnprintf(ptr, len, fmt, args);

		if (_debug)
		{
			std::cout << buf << std::flush;
		}

		if (_file)
		{
			_file << buf;
			
			if (level >= 7)
			{
				_file << std::flush;
			}
		}
	}

};

}
#endif //STDEX_LOGGER_H_
