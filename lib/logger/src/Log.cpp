#include "Log.h"
#include "CoutLogger.h"
#include <cstdio>

Log *Log::m_instance = NULL;

void Log::init(ILogger *log)
{
    instance()->m_log = log;
}

void Log::info(const char *str)
{
    instance()->m_log->info(str);
}

void Log::infof(const char *fmt, ...)
{
    char *s = NULL;
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(s, fmt, myargs);
    va_end(myargs);
    info(s);
}

void Log::warn(const char *str)
{
    instance()->m_log->warn(str);
}

void Log::warnf(const char *fmt, ...)
{
    char *s = NULL;
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(s, fmt, myargs);
    va_end(myargs);
    warn(s);
}

void Log::error(const char *str)
{
    instance()->m_log->error(str);
}

void Log::errorf(const char *fmt, ...)
{
    char *s = NULL;
    va_list myargs;
    va_start(myargs, fmt);
    vsprintf(s, fmt, myargs);
    va_end(myargs);
    error(s);
}

Log *Log::instance()
{
    if (!m_instance)
    {
        m_instance = new Log();
    }
    return m_instance;
}

Log::Log()
    : m_log(new CoutLogger())
{
}