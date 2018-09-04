/*
* pyc Python Compact Library - header for py.c
* copyright 2017-2018 dixyes
* MIT License
*/
#ifndef _PY_HEADER
#define _PY_HEADER

#include <stdbool.h>
#include <windows.h>
// export functions and variables
extern int py_load(wchar_t * dllName);
extern int py_unload();
extern int py_init();
extern int py_initEp();
extern void py_end();
extern void py_finalize();
extern void py_endEp();
extern int py_callCallback(const char *eventName, const char * format, ...);
extern HANDLE py_module;
extern bool py_moduleLoaded; // python3x.dll�Ƿ����
extern bool py_entrypointLoaded; // ��ڵ��ļ��Ƿ����
extern bool py_intLoaded; // python�������Ƿ����
extern bool py_msmLoaded; // python��������Ƿ����

#ifdef _DEBUG
extern void py_finalize(void);
#endif

#endif // _PY_HEADER

#include "pys.h"