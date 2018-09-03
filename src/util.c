
#include <stdint.h>
#include <windows.h>

#include "pys.h"
#include "cqp.h"

int ac = -1;
char * _leMsg = NULL;
uint32_t _leCode = 0;
char * _logxbuf = NULL;
int pyVer = 0;

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

inline int convertType(LOGGER_TYPE lt) {
    switch (lt) {
    case LOGGER_DEBUG:
        return CQLOG_DEBUG;
    case LOGGER_INFO:
        return CQLOG_INFO;
    case LOGGER_INFOSUCCESS:
        return CQLOG_INFOSUCCESS;
    case LOGGER_INFORECV:
        return CQLOG_INFORECV;
    case LOGGER_INFOSEND:
        return CQLOG_INFOSEND;
    case LOGGER_WARNING:
        return CQLOG_WARNING;
    case LOGGER_ERROR:
        return CQLOG_ERROR;
    case LOGGER_FATAL:
        return CQLOG_FATAL;
    default:
        return CQLOG_INFO;
    }
    return -1; // never excuted
}

void util_init() {
    _logxbuf = malloc(4096);
    _leMsg = malloc(4096);
}
void util_free() {
    free(_logxbuf);
    free(_leMsg);
}

/*
* 日个志
*/
void logx(const char * tag, LOGGER_TYPE type, const char * format, ...) {
    int cqlogtype = convertType(type);
    va_list args;
    va_start(args, format);
    vsnprintf(_logxbuf, 4096, format, args);
    va_end(args);
    CQ_addLog(ac, cqlogtype, tag, _logxbuf);
}

/*
* 打印lasterror
*/
void showLe(const char * reason, const char * ln) {
    _leCode = GetLastError();
    if (0 != _leCode) {
        FormatMessage(FORMAT_MESSAGE_FROM_SYSTEM, NULL, _leCode, 0, _leMsg, 1024, NULL); // TODO: add from_module support
        snprintf(_logxbuf, 4096, "%s reason:%s code:%08X msg:%s", ln, reason, _leCode, _leMsg);
        CQ_addLog(ac, LOGGER_ERROR, "showLe", _logxbuf);
    }
}

PyObject* ptype, *pvalue, *ptraceback;
/*
* 打印python exception
*/
void _catchPyExc(const char * ln) {
    PyErr_Fetch(&ptype, &pvalue, &ptraceback);
    //char * te = (char *)PyUnicode_AsUTF8(PyObject_Repr(ptype));
    logx("pyException", LOGGER_ERROR, "%s etype: %s", ln, PyUnicode_AsUTF8(PyObject_Repr(ptype)));
    logx("pyException", LOGGER_ERROR, "%s evalue: %s", ln, PyUnicode_AsUTF8(PyObject_Repr(pvalue)));
    //seems no use...
    /*
    PyObject* pyoExcText = PyEval_CallMethod(ptraceback, "format_exc", "()");
    if (NULL != pyoExcText) {
        logx("pyException", LOGGER_ERROR, "%s stacktrace: %s", ln, PyUnicode_AsUTF8(PyObject_Repr(pyoExcText)));
    }
    else {
        logx("pyException", LOGGER_ERROR, "%s no stacktrace", ln);
    }
    */
}