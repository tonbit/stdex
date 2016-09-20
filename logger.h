#ifndef STDEX_LOGGER_H_
#define STDEX_LOGGER_H_

#include "stdex/stdcc.h"

namespace stdex {

class Logger
{
public:
	Logger()
    {
        _file = NULL;
        _level = 7;
        _debug = true;
    }

	~Logger()
    {
        close();
    }

	int open(const string &path)
	{
        _file = fopen(path.c_str(), "ab");
        if (!_file)
            return 1;

        return 0;
    }

	void close()
    {
        if (_file)
        {
            fclose(_file);
            _file = NULL;
        }
    }

    void set_debug(bool debug)
    {
        _debug=debug;
    }

    void set_level(int level)
    {
        _level=level;
    }

    static void log_sys(const string &ident, const char *format, ...)
    {
        #ifdef _MSC_VER

        va_list args;
        va_start(args, format);
        vprintf(format, args);
        va_end(args);

        #else //_MSC_VER

        va_list args;
        va_start(args, format);
        openlog(ident.c_str(), LOG_CONS | LOG_PID, LOG_USER);
        vsyslog(LOG_INFO, format, args);
        closelog();
        va_end(args);

        #endif //_MSC_VER
    }

    void log_fatal(const char *format, ...)
    {
        if (_level < 0)
            return;

        va_list args;
        va_start(args, format);
        write(0, "FTL", format, args);
        va_end(args);
    }

    void log_emerg(const char *format, ...)
    {
        if (_level < 1)
            return;

        va_list args;
        va_start(args, format);
        write(1, "EMG", format, args);
        va_end(args);
    }

    void log_alert(const char *format, ...)
    {
        if (_level < 2)
            return;

        va_list args;
        va_start(args, format);
        write(2, "ALT", format, args);
        va_end(args);
    }

    void log_error(const char *format, ...)
    {
        if (_level < 3)
            return;

        va_list args;
        va_start(args, format);
        write(3, "ERR", format, args);
        va_end(args);
    }

    void log_warn(const char *format, ...)
    {
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

private:
    FILE *_file;
    int _level;
    bool _debug;

    void write(int level, const char *title, const char *fmt, va_list args)
    {
        if (!_debug && !_file)
            return;

        char buf[4096];
        int ret;

        char *ptr = buf;
        int len = sizeof(buf);

        time_t cur = time(NULL);
        ret = strftime(ptr, len, "\n%m%d %H:%M:%S ", localtime(&cur));
        ptr += ret;
        len -= ret;

        ret = snprintf(ptr, len, "%s: ", title);
        ptr += ret;
        len -= ret;

        ret = vsnprintf(ptr, len, fmt, args);

        if (_debug)
		{
            //puts(buf);
            std::cout << buf << std::flush;;
		}

        if (_file)
		{
            fputs(buf, _file);
			fflush(_file);
		}
    }
};

}
#endif //STDEX_LOGGER_H_
