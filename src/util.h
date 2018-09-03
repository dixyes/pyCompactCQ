/*
* pyc Python Compact Library - util header
* copyright 2017-2018 dixyes
* MIT License
*/

#ifndef _UTIL_HEADER
#define _UTIL_HEADER
// config: logx show detail filename line number

#define LOGX_SHOW_LINE_FILE 1

#define _STRINGIZE(x) _STRINGIZE2(x)
#define _STRINGIZE2(x) #x
#define LINE_STRING _STRINGIZE(__LINE__)

#if LOGX_SHOW_LINE_FILE
#   define LOG_PREFIX __FILE__ ":" LINE_STRING
#else
#   define LOG_PREFIX ""
#endif // LOGX_SHOW_LINE_FILE

typedef enum {
    LOGGER_DEBUG,
    LOGGER_INFO,
    LOGGER_INFOSUCCESS,
    LOGGER_INFORECV,
    LOGGER_INFOSEND,
    LOGGER_WARNING,
    LOGGER_ERROR,
    LOGGER_FATAL,
} LOGGER_TYPE;

extern int ac; // CQAPI AC
extern int pyVer;
extern const char* appPath;

extern void util_init();
extern void util_free();

extern void logx(const char * tag, LOGGER_TYPE type, const char * format, ...);
extern void showLe(const char * reason, const char * ln);
extern void _catchPyExc(const char * ln);
#define catchPyExc() _catchPyExc(LOG_PREFIX)

// define logxs( with ln )
#define logd(tag, msg, ...) logx(tag, LOGGER_DEBUG, LOG_PREFIX " " msg, ##__VA_ARGS__)
#define logi(tag, msg, ...) logx(tag, LOGGER_INFO, LOG_PREFIX " " msg, ##__VA_ARGS__)
#define loge(tag, msg, ...) logx(tag, LOGGER_ERROR, LOG_PREFIX " " msg, ##__VA_ARGS__)

// if not in debug build, disable logd
#if !_DEBUG
#   undef logd
#   define logd(...)
#endif // _DEBUG

#define showLeLn(x) showLe(x, __FILE__ ":" LINE_STRING)

#endif // _UTIL_HEADER