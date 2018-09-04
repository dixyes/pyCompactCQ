/*
* pyc Python Compact Library - python dynamic(dirty) load
* copyright 2017-2018 dixyes
* MIT License
*/
#include <time.h>
#include <stdbool.h>
#include <stdio.h>
#include <string.h>
#include <windows.h>

#include "cqp.h"

#include "mian.h"
#include "util.h"
#include "texts.h"
#include "pycqapi.h"
#include "pys.h"

HANDLE py_module = NULL;
bool py_moduleLoaded = false;
PyObject *py_botObj = NULL;
bool py_entrypointLoaded = false;
bool py_intLoaded = false;
bool py_msmLoaded = false;

PyObject *pyoGlobal = NULL;
PyObject *pyoGlobalDict = NULL;
PyThreadState *pyMainThreadState = NULL;
//char * _cqLoggerBuf;

#define _checkPyFunction(name) {\
    if(NULL==name){\
        loge("loadFunction", #name " failed");\
        showLeLn(#name);\
        return ERR_FUNCTION_INIT_FAIL;\
    };\
}

/*
* dirty patch for py3.0,3.2,3.2
*/
const char* dirtyPyUnicode_AsUTF8(PyObject *unicode) {
    PyObject * pyoRet;
    int retCode = -1;
    char * ret;
    pyoRet = PyTuple_New(1);
    if (NULL == pyoRet) {
        return NULL;
    }
    retCode = PyTuple_SetItem(pyoRet, 0, unicode);
    if (0 != retCode) {
        return NULL;
    }
    retCode = PyArg_ParseTuple(pyoRet, "es", "gb18030", &ret);
    if (0 == retCode) {
        return NULL;
    }
    return (const char *)ret;
}

/*
* 装载python3x.dll
* @param dllName 要使用的dll名称，NULL时使用默认的python37_d.dll和python73.dll
* @return 返回装载结果枚举值
*         ERR_EP_CANT_OPEN 打不开入口点文件，检查入口点文件是否位于appdirectionary
*         ERR_MODULE_NOT_LOAD dll未装载
*         S_OK 成功装载
*/
int py_load(wchar_t * dllName){
    if (NULL == dllName) {
#ifdef _DEBUG
        py_module = LoadLibraryExW(dllName, NULL,0);
#else
        py_module = LoadLibraryExW(dllName, NULL,0);
#endif    
    }
    else {
        py_module = LoadLibraryExW(dllName, NULL, 0);
    }

    if (NULL == py_module){
        return ERR_MODULE_NOT_LOAD;
    }
    // auto make loadFunctions list from pys.h
    // DONOT REMOVE OR CHANGE THE MARK BELOW AND END MARK
    /*AUTOLISTMARK*/
    PyArg_Parse = (int (__cdecl *)(PyObject *args, char *format, ...))GetProcAddress(py_module, "PyArg_Parse");
    _checkPyFunction(PyArg_Parse);
    PyArg_ParseTuple = (int (__cdecl *)(PyObject *args, const char *format, ...))GetProcAddress(py_module, "PyArg_ParseTuple");
    _checkPyFunction(PyArg_ParseTuple);
    PyArg_ParseTupleAndKeywords = (int(__cdecl *)(PyObject *args, PyObject *kw, const char *format, char *keywords[], ...))GetProcAddress(py_module, "PyArg_ParseTupleAndKeywords");
    _checkPyFunction(PyArg_ParseTupleAndKeywords);
    PyArg_VaParseTupleAndKeywords = (int(__cdecl *)(PyObject *args, PyObject *kw, const char *format, char *keywords[], va_list vargs))GetProcAddress(py_module, "PyArg_VaParseTupleAndKeywords");
    _checkPyFunction(PyArg_VaParseTupleAndKeywords);
    PyBytes_AsString = (char* (__cdecl *)(PyObject *o))GetProcAddress(py_module, "PyBytes_AsString");
    _checkPyFunction(PyBytes_AsString);
    PyCallable_Check = (int (__cdecl *)(PyObject *o))GetProcAddress(py_module, "PyCallable_Check");
    _checkPyFunction(PyCallable_Check);
    PyDict_Clear = (void (__cdecl *)(PyObject *p))GetProcAddress(py_module, "PyDict_Clear");
    _checkPyFunction(PyDict_Clear);
    PyDict_DelItemString = (int (__cdecl *)(PyObject *p, const char *key))GetProcAddress(py_module, "PyDict_DelItemString");
    _checkPyFunction(PyDict_DelItemString);
    PyDict_GetItemString = (PyObject* (__cdecl *)(PyObject *p, const char *key))GetProcAddress(py_module, "PyDict_GetItemString");
    _checkPyFunction(PyDict_GetItemString);
    PyDict_New = (PyObject* (__cdecl *)())GetProcAddress(py_module, "PyDict_New");
    _checkPyFunction(PyDict_New);
    PyDict_SetItemString = (int (__cdecl *)(PyObject *p, const char *key, PyObject *val))GetProcAddress(py_module, "PyDict_SetItemString");
    _checkPyFunction(PyDict_SetItemString);
    PyErr_BadArgument = (int (__cdecl *)(void))GetProcAddress(py_module, "PyErr_BadArgument");
    _checkPyFunction(PyErr_BadArgument);
    PyErr_Fetch = (void (__cdecl *)(PyObject **ptype, PyObject **pvalue, PyObject **ptraceback))GetProcAddress(py_module, "PyErr_Fetch");
    _checkPyFunction(PyErr_Fetch);
    PyEval_AcquireThread = (void (__cdecl *)(PyThreadState *tstate))GetProcAddress(py_module, "PyEval_AcquireThread");
    _checkPyFunction(PyEval_AcquireThread);
    PyEval_CallMethod = (PyObject * (__cdecl *)(PyObject *obj, const char *name, const char *format, ...))GetProcAddress(py_module, "PyEval_CallMethod");
    _checkPyFunction(PyEval_CallMethod);
    PyEval_GetBuiltins = (PyObject* (__cdecl *)(void))GetProcAddress(py_module, "PyEval_GetBuiltins");
    _checkPyFunction(PyEval_GetBuiltins);
    PyEval_InitThreads = (void (__cdecl *)(void))GetProcAddress(py_module, "PyEval_InitThreads");
    _checkPyFunction(PyEval_InitThreads);
    PyEval_ReInitThreads = (void (__cdecl *)(void))GetProcAddress(py_module, "PyEval_ReInitThreads");
    _checkPyFunction(PyEval_ReInitThreads);
    PyEval_ReleaseThread = (void (__cdecl *)(PyThreadState *tstate))GetProcAddress(py_module, "PyEval_ReleaseThread");
    _checkPyFunction(PyEval_ReleaseThread);
    PyEval_RestoreThread = (void (__cdecl *)(PyThreadState *tstate))GetProcAddress(py_module, "PyEval_RestoreThread");
    _checkPyFunction(PyEval_RestoreThread);
    PyEval_SaveThread = (PyThreadState* (__cdecl *)(void))GetProcAddress(py_module, "PyEval_SaveThread");
    _checkPyFunction(PyEval_SaveThread);
    PyGILState_Ensure = (PyGILState_STATE(__cdecl *)(void))GetProcAddress(py_module, "PyGILState_Ensure");
    _checkPyFunction(PyGILState_Ensure);
    PyGILState_Release = (void(__cdecl *)(PyGILState_STATE))GetProcAddress(py_module, "PyGILState_Release");
    _checkPyFunction(PyGILState_Release);
    PyImport_AddModule = (PyObject *(__cdecl *) (const char * name))GetProcAddress(py_module, "PyImport_AddModule");
    _checkPyFunction(PyImport_AddModule);
    PyImport_AppendInittab = (int (__cdecl *) (const char *name, PyObject* (*initfunc)(void)))GetProcAddress(py_module, "PyImport_AppendInittab");
    _checkPyFunction(PyImport_AppendInittab);
    PyImport_ImportModule = (PyObject *(__cdecl *) (const char * name))GetProcAddress(py_module, "PyImport_ImportModule");
    _checkPyFunction(PyImport_ImportModule);
    PyList_Append = (int (__cdecl *)(PyObject *list, PyObject *item))GetProcAddress(py_module, "PyList_Append");
    _checkPyFunction(PyList_Append);
    PyLong_FromLong = (PyObject* (__cdecl *)(long v))GetProcAddress(py_module, "PyLong_FromLong");
    _checkPyFunction(PyLong_FromLong);
    PyLong_FromLongLong = (PyObject* (__cdecl *)(long long v))GetProcAddress(py_module, "PyLong_FromLongLong");
    _checkPyFunction(PyLong_FromLongLong);
    PyModule_AddIntConstant = (int(__cdecl *)(PyObject *module, const char *name, long value))GetProcAddress(py_module, "PyModule_AddIntConstant");
    _checkPyFunction(PyModule_AddIntConstant);
    PyModule_AddObject = (int (__cdecl *)(PyObject *module, const char *name, PyObject *value))GetProcAddress(py_module, "PyModule_AddObject");
    _checkPyFunction(PyModule_AddObject);
    PyModule_AddStringConstant = (int(__cdecl *)(PyObject *module, const char *name, const char *value))GetProcAddress(py_module, "PyModule_AddStringConstant");
    _checkPyFunction(PyModule_AddStringConstant);
    PyModule_GetDict = (PyObject* (__cdecl *) (PyObject *module))GetProcAddress(py_module, "PyModule_GetDict");
    _checkPyFunction(PyModule_GetDict);
    PyObject_Call = (PyObject* (__cdecl *)(PyObject *callable, PyObject *args, PyObject *kwargs))GetProcAddress(py_module, "PyObject_Call");
    _checkPyFunction(PyObject_Call);
    PyObject_GetAttrString = (PyObject *(__cdecl *) (PyObject * obj, const char * str))GetProcAddress(py_module, "PyObject_GetAttrString");
    _checkPyFunction(PyObject_GetAttrString);
    PyObject_Repr = (PyObject* (__cdecl *)(PyObject *o))GetProcAddress(py_module, "PyObject_Repr");
    _checkPyFunction(PyObject_Repr);
    PyRun_FileEx = (PyObject* (__cdecl *) (FILE *fp, const char *filename, int start, PyObject *globals, PyObject *locals, int closeit))GetProcAddress(py_module, "PyRun_FileEx");
    _checkPyFunction(PyRun_FileEx);
    PyRun_SimpleFileEx = (int (__cdecl *)(FILE *fp, const char *filename, int closeit))GetProcAddress(py_module, "PyRun_SimpleFileEx");
    _checkPyFunction(PyRun_SimpleFileEx);
    PyRun_String = (PyObject* (__cdecl *)(const char *str, int start, PyObject *globals, PyObject *locals))GetProcAddress(py_module, "PyRun_String");
    _checkPyFunction(PyRun_String);
    PySys_SetPath = (void (__cdecl *)(const wchar_t *path))GetProcAddress(py_module, "PySys_SetPath");
    _checkPyFunction(PySys_SetPath);
    PyThreadState_Get = (PyThreadState *(__cdecl *)(void))GetProcAddress(py_module, "PyThreadState_Get");
    _checkPyFunction(PyThreadState_Get);
    PyTuple_New = (PyObject* (__cdecl *)(Py_ssize_t len))GetProcAddress(py_module, "PyTuple_New");
    _checkPyFunction(PyTuple_New);
    PyTuple_SetItem = (int (__cdecl *)(PyObject *p, Py_ssize_t pos, PyObject *o))GetProcAddress(py_module, "PyTuple_SetItem");
    _checkPyFunction(PyTuple_SetItem);
    Py_BuildValue = (PyObject* (__cdecl *)(const char *format, ...))GetProcAddress(py_module, "Py_BuildValue");
    _checkPyFunction(Py_BuildValue);
    Py_EndInterpreter = (void(__cdecl *)(PyThreadState *tstate))GetProcAddress(py_module, "Py_EndInterpreter");
    _checkPyFunction(Py_EndInterpreter);
    Py_Finalize = (void (__cdecl *) (void))GetProcAddress(py_module, "Py_Finalize");
    _checkPyFunction(Py_Finalize);
    Py_GetVersion = (const char* (__cdecl *)(void))GetProcAddress(py_module, "Py_GetVersion");
    _checkPyFunction(Py_GetVersion);
    Py_InitializeEx = (void (__cdecl *) (int))GetProcAddress(py_module, "Py_InitializeEx");
    _checkPyFunction(Py_InitializeEx);
    Py_IsInitialized = (int (__cdecl *) (void))GetProcAddress(py_module, "Py_IsInitialized");
    _checkPyFunction(Py_IsInitialized);
    Py_NewInterpreter = (PyThreadState* (__cdecl *)(void))GetProcAddress(py_module, "Py_NewInterpreter");
    _checkPyFunction(Py_NewInterpreter);
    Py_VaBuildValue = (PyObject* (__cdecl *)(const char *format, va_list vargs))GetProcAddress(py_module, "Py_VaBuildValue");
    _checkPyFunction(Py_VaBuildValue);
    _Py_NoneStruct = (PyObject*)GetProcAddress(py_module, "_Py_NoneStruct");
    _checkPyFunction(_Py_NoneStruct);
    /*ENDAUTOLISTMARK*/
    // spectial for different export name in .dll and _d.dll
    // for py below 3.4
    const char * pyVerString = Py_GetVersion();
    char pyVer = pyVerString[2];
    switch(pyVer) {
    case '7':
    case '6':
    case '5':
    case '4':
        PyGILState_Check = (int(__cdecl *)(void))GetProcAddress(py_module, "PyGILState_Check");
    case '3':
    case '2':
        PyUnicode_AsEncodedString = (PyObject * (__cdecl*)(PyObject *unicode, const char *encoding, const char *errors))GetProcAddress(py_module, "PyUnicode_AsEncodedString");
        _checkPyFunction(PyUnicode_AsEncodedString);
        PyUnicode_FromString = (PyObject *(__cdecl *) (const char *u))GetProcAddress(py_module, "PyUnicode_FromString");
        _checkPyFunction(PyUnicode_FromString);
        
    case '1':
    case '0':
        PyUnicode_Type = (PyTypeObject *)GetProcAddress(py_module, "PyUnicode_Type");
        if (NULL == PyUnicode_Type)
            PyUnicode_Type = (PyTypeObject *)GetProcAddress(py_module, "_PyUnicode_Type");
        _checkPyFunction(PyUnicode_Type);
        PyUnicode_AsUTF8 = (const char * (__cdecl*)(PyObject *unicode))GetProcAddress(py_module, "PyUnicode_AsUTF8");
        if (NULL == PyUnicode_AsUTF8)
            PyUnicode_AsUTF8 = dirtyPyUnicode_AsUTF8;
        _checkPyFunction(PyUnicode_AsUTF8);
        PyUnicode_AsEncodedString = (PyObject * (__cdecl*)(PyObject *unicode, const char *encoding, const char *errors))GetProcAddress(py_module, "PyUnicode_AsEncodedString");
        if (NULL == PyUnicode_AsEncodedString)
            PyUnicode_AsEncodedString = (PyObject * (__cdecl*)(PyObject *unicode, const char *encoding, const char *errors))GetProcAddress(py_module, "PyUnicodeUCS2_AsEncodedString");
        if (NULL == PyUnicode_AsEncodedString) 
            PyUnicode_AsEncodedString = (PyObject * (__cdecl*)(PyObject *unicode, const char *encoding, const char *errors))GetProcAddress(py_module, "PyUnicodeUCS4_AsEncodedString");
        _checkPyFunction(PyUnicode_AsEncodedString);
        PyUnicode_FromString = (PyObject *(__cdecl *) (const char *u))GetProcAddress(py_module, "PyUnicode_FromString");
        if (NULL == PyUnicode_FromString)
            PyUnicode_FromString = (PyObject *(__cdecl *) (const char *u))GetProcAddress(py_module, "PyUnicodeUCS2_FromString");
        if (NULL == PyUnicode_FromString)
            PyUnicode_FromString = (PyObject * (__cdecl*) (const char *u))GetProcAddress(py_module, "PyUnicodeUCS4_FromString");
        _checkPyFunction(PyUnicode_FromString);
        PyModule_Create2 = (PyObject * (__cdecl*)(struct PyModuleDef*, int))GetProcAddress(py_module, "PyModule_Create2");
        if (NULL == PyModule_Create2) {
            PyModule_Create2 = (PyObject * (__cdecl*)(struct PyModuleDef*, int))GetProcAddress(py_module, "PyModule_Create2TraceRefs");
        }
        _checkPyFunction(PyModule_Create2);
        py_moduleLoaded = true;
        return 0;
    default:
        loge("pyLoad", "unsupported py ver:\"%c\"", pyVer);
        loge("pyLoad", "unsupported py ver:\"%s\"", pyVerString);
        return -1;
    }
    
    return 404; // never executed
}
/*
* 卸载python3x.dll
*  这个函数可能并不能完全的清干净内存，如果发现泄露请及时报告
* @return 返回卸载结果(真成功 假失败)
*/
bool py_unload() {
    if (!py_moduleLoaded) {
        logx("unloadModule", LOGGER_WARNING, "calling py_unload with module not load!");
        return false;
    };
    PyUnicode_AsUTF8 = NULL;
    BOOL unloaded = FreeLibrary(py_module);
    if (!unloaded) {
        showLeLn("unloadModule");
    }
    return unloaded;
}

PyMODINIT_FUNC PyInit_cqapi(void); // 声明函数，用于PyImport_AppendInittab


#define _checkPyObj(name) {\
    if(NULL==name){\
        loge("checkPyObj", "fucked up with:%s",#name);\
        Py_Finalize();\
        RELEASE_GIL;\
        return ERR_PYAPI_FAIL;\
    }\
}

/*
* 初始化python， 新建（子）解释器
* @return 返回初始化结果枚举：
*         ERR_PYAPI_FAIL Python API失败
*/
int py_init(){
    // init py machinisms
    if(0==Py_IsInitialized()){
        // make cqapi pymodule
        PyImport_AppendInittab("cqapi", PyInit_cqapi);
        Py_InitializeEx(0);
        if(0 == Py_IsInitialized())
            return ERR_PYAPI_FAIL;
        PyEval_InitThreads();
        wchar_t * pathBuf = malloc(sizeof(wchar_t)*MAX_PATH);
        wsprintfW(pathBuf, L"%s", appPath);
        PyObject *pyoSys = pyoSys = PyImport_ImportModule("sys");
        PyObject *pyoSys_path = PyObject_GetAttrString(pyoSys, "path");
        if (0 != PyList_Append(pyoSys_path, PyUnicode_FromString(appPath))) {
            logd("checkPyObj", "fucked up with sys.path setting");
            return ERR_PYAPI_FAIL;
        };

        free(pathBuf);
        py_msmLoaded = true;
    }
    PyEval_ReleaseThread(PyThreadState_Get());
    py_intLoaded = true;
    FUCKGIL;
    return 0;
}
/*
*  结束python解释器
*    这个函数可能
*     * 实际上能以任何姿势崩溃
*/
void py_end() {
    if (!py_msmLoaded) {
        loge("pyEnd", "mechanism is not loaded");
        return;
    }
    if (py_intLoaded) {
        py_intLoaded = false;

        if (pyMainThreadState) {
            PyEval_AcquireThread(pyMainThreadState);
            Py_EndInterpreter(pyMainThreadState);
        }

        PyDict_Clear(pyoGlobalDict);
        logd("pyEnd", "finalize done");
    }
}
void py_finalize() {
    ENSURE_GIL;
    Py_Finalize();
}

/*
* 加载蛋疼（Egg Pain）文件
* @return 返回初始化结果枚举：
*         ERR_PYAPI_FAIL Python API失败
*/
int py_initEp() {
    int retCode = 0;
    ENSURE_GIL;
    PyObject *pyoSys = pyoSys = PyImport_ImportModule("sys"); _checkPyObj(pyoSys);
    PyObject *pyoSys_path = PyObject_GetAttrString(pyoSys, "path"); _checkPyObj(pyoSys_path);
    logd("ri", "%s", PyUnicode_AsUTF8(PyObject_Repr(pyoSys_path)));

    pyoGlobal = PyImport_AddModule("__entrypoint__"); _checkPyObj(pyoGlobal);
    pyoGlobalDict = PyModule_GetDict(pyoGlobal); _checkPyObj(pyoGlobal);
    PyDict_SetItemString(pyoGlobalDict, "__builtins__", PyEval_GetBuiltins());
    
    // open file
    logd("openEntrypoint", "load entrypoint file from %s", appPath);
    char * epPath = malloc(1024);
    FILE * epFd;
    strcpy_s(epPath, strlen(appPath) + 1, appPath);// pit here, however im too lazy to fix
    strcpy_s(epPath + strlen(appPath), 18, "__entrypoint__.py\0");
    logi("openEntrypoint", TEXT_LOADENTRYPOINT, epPath);
    errno_t err = fopen_s(&epFd, epPath, "r");
    if (err != 0) {
        //size_t errLen = strerrorlen_s(err); // why not c11?
        char* errMsg = malloc(1024);
        strerror_s(errMsg, 128, err);
        loge("openEntrypoint", "failed 2 open entrypoint file:%s", errMsg);
        free(errMsg);
        free(epPath);
        RELEASE_GIL;
        return ERR_EP_CANT_OPEN;
    }
    logd("openEntrypoint", "open entrypoint file %s done", epPath);
    //logd("openEntrypoint","fd addr %d",epFd);
    //logd("openEntrypoint","d addr %d",pyo__main___d);
    //logd("openEntrypoint","d addr %d",PyRun_FileEx);
    free(epPath);
    

    // run entrypoint
    //PyImport_ImportModule("cqapi"); // let user import it is also fine
    py_botObj = PyRun_FileEx(epFd, "__entrypoint__.py", Py_file_input, pyoGlobalDict, pyoGlobalDict, 1);
    //PyRun_SimpleFileEx(epFd, "__entrypoint__.py", 1);
    //
    if (NULL == py_botObj) {
        loge("runEntryPoint", TEXT_LOADENTRYPOINT_FAIL);
        catchPyExc();
        RELEASE_GIL;
        return ERR_EP_FAIL;
    }
    
    logd("ri2", "%s", PyUnicode_AsUTF8(PyObject_Repr(py_botObj)));
    logd("ri2", "%s", PyUnicode_AsUTF8(PyObject_Repr(pyoGlobalDict)));
    py_botObj = PyDict_GetItemString(pyoGlobalDict, "__bot__");
    if (NULL == py_botObj) {
        logx("runEntryPoint", LOGGER_WARNING, TEXT_NOBOTGLOBALVAR);
        RELEASE_GIL;
        return 0;
    }
    py_entrypointLoaded = true;
    RELEASE_GIL;
    return 0;
}
/*
*  卸载入口点
*/
void py_endEp() {
    if (py_entrypointLoaded) {
        py_entrypointLoaded = false;
        ENSURE_GIL;
        //PyDict_Clear(pyoGlobalDict);
        PyDict_DelItemString(pyoGlobalDict, "__bot__");
        Py_CLEAR(py_botObj);
        //PyDict_SetItemString(pyoGlobalDict, "__builtins__", PyEval_GetBuiltins());
        RELEASE_GIL;
    }
}

time_t lastWarned = 0;
int warnLevel = LOGGER_WARNING;
#define WARNCOOLDOWN  (time_t)3
/*
* 调用__bot__的事件回调
*   PYCQAPI_VERSION 当前行为
*             0.0.1 将会获取__bot__的emit方法，并将由format通过Py_VaBuildValue获取可变参生成eventTuple，调用emit(__bot__,eventName,eventTuple)
* @param cbName 事件名称
* @param format 事件名称
* @param ... 事件名称
* @return 返回enum{EVENT_BLOCK;EVENT_IGNORE};
* @note 当发生错误时这个函数不abort/抛出异常，仅做日志并返回EVENT_IGNORE
*/
int py_callCallback(const char *eventName, const char * format, ...) {
    // warn not inited things and warn rate limit
    if (!(py_moduleLoaded && py_intLoaded && py_entrypointLoaded)) {
        if (time(NULL) - lastWarned > WARNCOOLDOWN) {
            warnLevel = LOGGER_WARNING;
        }
        else {
            warnLevel = LOGGER_DEBUG;
        }
        lastWarned = time(NULL);
        if (!py_moduleLoaded) {
            logx("callCallback", warnLevel, TEXT_MODULENOTLOAD);
        }else if (!py_intLoaded) {
            logx("callCallback", warnLevel, TEXT_PYINTNOTLOAD);
        }else if (!py_entrypointLoaded) {
            logx("callCallback", warnLevel, TEXT_BOTNOTLOAD);
        }
        return EVENT_IGNORE;
    }

    // get py GIL
    ENSURE_GIL;

    // build argsTuple
    va_list arg;
    va_start(arg, format);
    int retCode = EVENT_IGNORE;
    PyObject *eventTuple = Py_VaBuildValue(format, arg);
    va_end(arg);
    if (NULL == eventTuple) {
        //loge("callCallback", "fail!");
        RELEASE_GIL;
        catchPyExc();
        return retCode;
    }
    PyObject *argsTuple = Py_BuildValue("sO", eventName,eventTuple);
    if (NULL == argsTuple) {
        //loge("callCallback", "fail!");
        RELEASE_GIL;
        catchPyExc();
        return retCode;
    }
    
    // call that callback
#ifdef _DEBUG
    logd("callCallback", "kwargs: %s",PyUnicode_AsUTF8(PyObject_Repr(argsTuple)));
    logd("callCallback", "eventName: %s", eventName);
#endif // _DEBUG
    PyObject* pyoCb = PyObject_GetAttrString(py_botObj, "emit");
    if (NULL == pyoCb) {
        catchPyExc();
        RELEASE_GIL;
        return retCode;
    }
    if (!PyCallable_Check(pyoCb)) {
        loge("callCallback", "emitter is not callable");
        RELEASE_GIL;
        return retCode;
    }
#ifdef _DEBUG
    logd("callCallback", "method: %s", PyUnicode_AsUTF8(PyObject_Repr(pyoCb)));
#endif // _DEBUG
    PyObject *pyoRet = PyObject_Call(pyoCb, argsTuple, PyDict_New());
    if (NULL == pyoRet) {
        catchPyExc();
        RELEASE_GIL;
        return retCode;
    }

    // receive return value from __bot__.emit
    if (!PyArg_Parse(pyoRet, "i", &retCode)) {
        PyObject * ptype, * pvalue, * ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);// clear pyerr
        logi("callCallback", "failed convert return value(%s) of emitter(%s)", PyUnicode_AsUTF8(PyObject_Repr(pyoRet)), PyUnicode_AsUTF8(PyObject_Repr(pyoCb)));
        RELEASE_GIL;
        return EVENT_IGNORE;
    }

    // release GIL
    RELEASE_GIL;
    //fuckGIL();
    return retCode;
}