/*
* pyc Python Compact Library - util header
* copyright 2017-2018 dixyes
* MIT License
*/

#ifndef _UTIL_HEADER
#define _UTIL_HEADER
// config: logx is enabled
#define LOGX_ENABLED 1
// config: logx show detail filename line number
#define LOGX_SHOW_LINE_FILE 1
// config logx buffer size
#define LOGX_BUFFER_SIZE 1024
// config: lastError process(needs logx
#define LE_ENABLED 1
// config: lastError buffer size
#define LE_BUFFER_SIZE 4096

#include <windows.h>
#include <string.h>
#define __FILENAME__ (strrchr(__FILE__, '/') ? strrchr(__FILE__, '/') + 1 : __FILE__)
#if LOGX_ENABLED
//include cqp and std libs for logx
#   include "cqp.h"
#   include <stdlib.h>
#   include <stdio.h>
// prepare logx buffer
extern char * _logxbuf;
extern int ac;
inline void util_alloc_logx_buf(ac) {_logxbuf = malloc(LOGX_BUFFER_SIZE); memset(_logxbuf,0,LOGX_BUFFER_SIZE); ac =ac;}
inline void util_free_logx_buf(void) {free(_logxbuf);}

#   define _logx(tag, prefix, type, msg, ...) snprintf(_logxbuf, LOGX_BUFFER_SIZE, prefix msg , ##__VA_ARGS__);CQ_addLog(ac, type, tag, _logxbuf)
#   define logx(tag, type, msg, ...) snprintf(_logxbuf, LOGX_BUFFER_SIZE, msg , ##__VA_ARGS__);CQ_addLog(ac, type, tag, _logxbuf)

#   if LOGX_SHOW_LINE_FILE
// get filename, line num
#       define _STRINGIZE(x) _STRINGIZE2(x)
#       define _STRINGIZE2(x) #x
#       define LINE_STRING _STRINGIZE(__LINE__)
// define logx( with ln )
#       define logd(tag, msg, ...) _logx(tag, __FILE__ ":" LINE_STRING " ", CQLOG_DEBUG, msg, ##__VA_ARGS__)
#       define logi(tag, msg, ...) _logx(tag, __FILE__ ":" LINE_STRING " ", CQLOG_INFO, msg, ##__VA_ARGS__)
#       define loge(tag, msg, ...) _logx(tag, __FILE__ ":" LINE_STRING " ", CQLOG_ERROR, msg, ##__VA_ARGS__)
#   else // LOGX_SHOW_LINE_FILE
// define logx
#       define logd(tag, msg, ...) _logx(tag,"", CQLOG_DEBUG, msg, ##__VA_ARGS__)
#       define logi(tag, msg, ...) _logx(tag,"", CQLOG_INFO, msg, ##__VA_ARGS__)
#       define loge(tag, msg, ...) _logx(tag,"", CQLOG_ERROR, msg, ##__VA_ARGS__)
#   endif // LOGX_SHOW_LINE_FILE
#else // LOGX_ENABLED
#   define logd(...)
#   define logi(...)
#   define loge(...)
#   define util_alloc_logx_buf()
#   define util_free_logx_buf()
#endif // LOGX_ENABLED
// if not in debug build, disable logd
#if !_DEBUG
#   undef logd
#   define logd(...)
#endif // _DEBUG

#if (LOGX_ENABLED && LE_ENABLED)
extern char * _leMsg;
extern uint32_t _leCode;
inline void util_alloc_le_buf(void) {_leMsg = malloc(LE_BUFFER_SIZE); memset(_leMsg,0,LE_BUFFER_SIZE);}
inline void util_free_le_buf(void) {free(_leMsg);}
#   define showLe(x) _showLe(x, __FILE__ ":" LINE_STRING)
inline void _showLe(const char * reason,const char * ln){
    _leCode = GetLastError();
    //loge("showLe","code: %d",_leCode);
    if (0!=_leCode){
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, _leCode, 0, _leMsg, 1024, NULL); // TODO: add from_module support
        snprintf(_logxbuf, LOGX_BUFFER_SIZE, "%s reason:%s code:%08X msg:%s", ln, reason, _leCode, _leMsg);
        CQ_addLog(ac, CQLOG_ERROR, "showLe", _logxbuf);
    }
}
#else
#   define util_alloc_le_buf()
#   define util_free_le_buf()
#   define showLe()
#endif // (LOGX_ENABLED && LE_ENABLED)
#endif // _UTIL_HEADER