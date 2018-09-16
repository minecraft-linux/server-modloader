#include <modloader/log.h>

#include <cstdio>
#include <ctime>
#include <algorithm>

using namespace modloader;

extern "C" {

const char *modloader_log_level_str(modloader_log_level level) {
    if (level == MODLOADER_LOG_TRACE) return "Trace";
    if (level == MODLOADER_LOG_DEBUG) return "Debug";
    if (level == MODLOADER_LOG_INFO) return "Info";
    if (level == MODLOADER_LOG_WARN) return "Warn";
    if (level == MODLOADER_LOG_ERROR) return "Error";
    return "?";
}

void modloader_vlog(modloader_log_level level, const char *tag, const char *format, va_list args) {
    char buffer[4096];
    int len = vsnprintf(buffer, sizeof(buffer), format, args);
    if (len > sizeof(buffer))
        len = sizeof(buffer);
    while (len > 0 && (buffer[len - 1] == '\r' || buffer[len - 1] == '\n'))
        buffer[--len] = '\0';

    char tbuf[128];
    tbuf[0] = '\0';

    time_t t = time(nullptr);
    tm tm;
    localtime_r(&t, &tm);
    strftime(tbuf, sizeof(tbuf), "%H:%M:%S", &tm);
    printf("%s %-5s [%s] %s\n", tbuf, modloader_log_level_str(level), tag, buffer);
}

void modloader_log(modloader_log_level level, const char *tag, const char *format, ...) {
    va_list args;
    va_start(args, format);
    modloader_vlog(level, tag, format, args);
    va_end(args);
}

}

void Log::log(LogLevel level, const char* tag, const char* format, ...) {
    va_list args;
    va_start(args, format);
    modloader_vlog(level, tag, format, args);
    va_end(args);
}

#define LogFuncDef(name, logLevel) \
    void name(const char* tag, const char* format, ...) { \
        va_list args; \
        va_start(args, format); \
        modloader_vlog(logLevel, tag, format, args); \
        va_end(args); \
    }

LogFuncDef(modloader_logv, MODLOADER_LOG_TRACE)
LogFuncDef(modloader_logd, MODLOADER_LOG_DEBUG)
LogFuncDef(modloader_logi, MODLOADER_LOG_INFO)
LogFuncDef(modloader_logw, MODLOADER_LOG_WARN)
LogFuncDef(modloader_loge, MODLOADER_LOG_ERROR)

LogFuncDef(Log::verbose, MODLOADER_LOG_TRACE)
LogFuncDef(Log::debug, MODLOADER_LOG_DEBUG)
LogFuncDef(Log::info, MODLOADER_LOG_INFO)
LogFuncDef(Log::warn, MODLOADER_LOG_WARN)
LogFuncDef(Log::error, MODLOADER_LOG_ERROR)