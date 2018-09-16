#ifndef MODLOADER_LOG_H
#define MODLOADER_LOG_H

#ifdef __cplusplus
#include <cstdarg>
#else
#include <stdarg.h>
#endif

#ifdef __cplusplus
extern "C" {
#endif

typedef enum {
    MODLOADER_LOG_TRACE,
    MODLOADER_LOG_DEBUG,
    MODLOADER_LOG_INFO,
    MODLOADER_LOG_WARN,
    MODLOADER_LOG_ERROR
} modloader_log_level;

void modloader_log(modloader_log_level level, const char *tag, const char *format, ...);
void modloader_vlog(modloader_log_level level, const char *tag, const char *format, va_list args);

const char *modloader_log_level_str(modloader_log_level level);

void modloader_logv(const char *tag, const char *format, ...);
void modloader_logd(const char *tag, const char *format, ...);
void modloader_logi(const char *tag, const char *format, ...);
void modloader_logw(const char *tag, const char *format, ...);
void modloader_loge(const char *tag, const char *format, ...);

#ifdef __cplusplus
}
#endif


#ifdef __cplusplus
namespace modloader {

class Log {

public:
    using LogLevel = modloader_log_level;

    static const char *getLogLevelString(LogLevel level) {
        return modloader_log_level_str(level);
    }

    static void vlog(LogLevel level, const char* tag, const char* text, va_list args) {
        modloader_vlog(level, tag, text, args);
    }

    static void log(LogLevel level, const char* tag, const char* text, ...);

    static void verbose(const char* tag, const char* text, ...);
    static void debug(const char* tag, const char* text, ...);
    static void info(const char* tag, const char* text, ...);
    static void warn(const char* tag, const char* text, ...);
    static void error(const char* tag, const char* text, ...);

};

}
#endif

#endif
