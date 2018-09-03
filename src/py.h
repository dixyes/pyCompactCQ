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
extern void py_reinit();
extern void py_endEp();
extern int py_callCallback(const char *eventName, const char * format, ...);
extern HANDLE py_module;
extern bool py_moduleLoaded; // python3x.dll是否加载
extern bool py_entrypointLoaded; // 入口点文件是否加载
extern bool py_intLoaded; // python解释器是否加载

#endif // _PY_HEADER

#include "pys.h"