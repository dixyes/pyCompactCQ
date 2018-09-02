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
//char * _cqLoggerBuf;

#define _checkPyFunction(name) {\
    if(NULL==name){\
        loge("loadFunction", #name " failed");\
        showLeLn(#name);\
        return ERR_FUNCTION_INIT_FAIL;\
    };\
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
        py_module = LoadLibraryExW(L"python37_d.dll", NULL,0);
#else
        py_module = LoadLibraryExW(L"python37.dll", NULL,0);
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
    PyDict_GetItemString = (PyObject* (__cdecl *)(PyObject *p, const char *key))GetProcAddress(py_module, "PyDict_GetItemString");
    _checkPyFunction(PyDict_GetItemString);
    PyDict_New = (PyObject* (__cdecl *)())GetProcAddress(py_module, "PyDict_New");
    _checkPyFunction(PyDict_New);
    PyErr_BadArgument = (int (__cdecl *)(void))GetProcAddress(py_module, "PyErr_BadArgument");
    _checkPyFunction(PyErr_BadArgument);
    PyErr_Fetch = (void (__cdecl *)(PyObject **ptype, PyObject **pvalue, PyObject **ptraceback))GetProcAddress(py_module, "PyErr_Fetch");
    _checkPyFunction(PyErr_Fetch);
    PyEval_CallMethod = (PyObject * (__cdecl *)(PyObject *obj, const char *name, const char *format, ...))GetProcAddress(py_module, "PyEval_CallMethod");
    _checkPyFunction(PyEval_CallMethod);
    PyEval_InitThreads = (void (__cdecl *)(void))GetProcAddress(py_module, "PyEval_InitThreads");
    _checkPyFunction(PyEval_InitThreads);
    PyEval_ReleaseThread = (void (__cdecl *)(PyThreadState *tstate))GetProcAddress(py_module, "PyEval_ReleaseThread");
    _checkPyFunction(PyEval_ReleaseThread);
    PyGILState_Check = (int (__cdecl *)(void))GetProcAddress(py_module, "PyGILState_Check");
    _checkPyFunction(PyGILState_Check);
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
    PyThreadState_Get = (PyThreadState *(__cdecl *)(void))GetProcAddress(py_module, "PyThreadState_Get");
    _checkPyFunction(PyThreadState_Get);
    PyUnicode_AsEncodedString = (PyObject* (__cdecl *)(PyObject *unicode, const char *encoding, const char *errors))GetProcAddress(py_module, "PyUnicode_AsEncodedString");
    _checkPyFunction(PyUnicode_AsEncodedString);
    PyUnicode_AsUTF8 = (const char* (__cdecl *) (PyObject *unicode))GetProcAddress(py_module, "PyUnicode_AsUTF8");
    _checkPyFunction(PyUnicode_AsUTF8);
    PyUnicode_FromString = (PyObject *(__cdecl *) (const char *u))GetProcAddress(py_module, "PyUnicode_FromString");
    _checkPyFunction(PyUnicode_FromString);
    PyUnicode_Type = (PyTypeObject *)GetProcAddress(py_module, "PyUnicode_Type");
    _checkPyFunction(PyUnicode_Type);
    Py_BuildValue = (PyObject* (__cdecl *)(const char *format, ...))GetProcAddress(py_module, "Py_BuildValue");
    _checkPyFunction(Py_BuildValue);
    Py_Finalize = (void (__cdecl *) (void))GetProcAddress(py_module, "Py_Finalize");
    _checkPyFunction(Py_Finalize);
    Py_InitializeEx = (void (__cdecl *) (int))GetProcAddress(py_module, "Py_InitializeEx");
    _checkPyFunction(Py_InitializeEx);
    Py_IsInitialized = (int (__cdecl *) (void))GetProcAddress(py_module, "Py_IsInitialized");
    _checkPyFunction(Py_IsInitialized);
    Py_VaBuildValue = (PyObject* (__cdecl *)(const char *format, va_list vargs))GetProcAddress(py_module, "Py_VaBuildValue");
    _checkPyFunction(Py_VaBuildValue);
    _Py_NoneStruct = (PyObject*)GetProcAddress(py_module, "_Py_NoneStruct");
    _checkPyFunction(_Py_NoneStruct);
    /*ENDAUTOLISTMARK*/
    // spectial for different export name in .dll and _d.dll
    PyModule_Create2 = (PyObject * (__cdecl*)(struct PyModuleDef*, int))GetProcAddress(py_module, "PyModule_Create2");
    if (NULL == PyModule_Create2) {
        PyModule_Create2 = (PyObject * (__cdecl*)(struct PyModuleDef*, int))GetProcAddress(py_module, "PyModule_Create2TraceRefs");
        if (NULL == PyModule_Create2) {
            loge("loadFunction", "PyModule_Create failed");
            return ERR_FUNCTION_INIT_FAIL;
        }
    }
    py_moduleLoaded = true;
    
    return 0;
}
/*
* 卸载python3x.dll
*  这个函数可能并不能完全的清干净内存，如果发现泄露请及时报告
* @return 返回卸载结果(真成功 假失败)
*/
bool py_unload() {
    if (!py_moduleLoaded) {
        logx("unloadModule", CQLOG_WARNING, "calling py_unload with module not load!");
        return false;
    };
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
        PyGILState_Release(gstate);\
        return ERR_PYAPI_FAIL;\
    }\
}
PyObject *pyoGlobal = NULL;
PyObject *pyoGlobalDict = NULL;
/*
* 初始化python虚拟机/解释器/随便你叫它啥,加载入口点
* @return 返回初始化结果枚举：
*         ERR_PYAPI_FAIL Python API失败
*         ERR_EP_CANT_OPEN ep文件打不开
*         ERR_EP_FAIL ep执行失败
*/
int py_init(){
    int retCode = 0; // py api return code

    // make cqapi pymodule
    PyImport_AppendInittab("cqapi", PyInit_cqapi);

    // init py interupter
    Py_InitializeEx(0);
    if(0==Py_IsInitialized()){
        return ERR_PYAPI_FAIL;
    }

    // release GIL and re-accquire it
    PyEval_InitThreads();
    PyEval_ReleaseThread(PyThreadState_Get()); //TODO: use un checked version of this
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    
    py_intLoaded = true;

    // append appdir to python src path
    const char* appPath = CQ_getAppDirectory(ac);
    //  pyoSys is py object sys
    //  pyoSys_path is py object sys.path
    PyObject *pyoSys = PyImport_ImportModule("sys");_checkPyObj(pyoSys);
    PyObject *pyoSys_path = PyObject_GetAttrString(pyoSys, "path");_checkPyObj(pyoSys_path);
    if ( 0 != (retCode = PyList_Append(pyoSys_path, PyUnicode_FromString(appPath)))){
        logd("checkPyObj","fucked up with code %d", retCode);
        PyGILState_Release(gstate);
        return ERR_PYAPI_FAIL;
    };
    
    // prepare entrypoint file
    pyoGlobal = PyImport_AddModule("__main__");_checkPyObj(pyoGlobal);
    //logd("miao","sreftotal %d" ,_Py_RefTotal);
    Py_INCREF(pyoGlobal);
    //logd("miao","reftotal %d" ,_Py_RefTotal);
    pyoGlobalDict = PyModule_GetDict(pyoGlobal);_checkPyObj(pyoGlobalDict);
    Py_INCREF(pyoGlobalDict);
    
    // open file
    logd("openEntrypoint","load entrypoint file from %s",appPath);
    char * epPath = malloc(1024);
    FILE * epFd;
    strcpy_s(epPath,strlen(appPath)+1,appPath);// pit here, however im too lazy to fix
    strcpy_s(epPath+strlen(appPath), 18, "__entrypoint__.py\0");
    logi("openEntrypoint", TEXT_LOADENTRYPOINT, epPath);
    errno_t err = fopen_s(&epFd, epPath, "r");
    if (err!=0){
        //size_t errLen = strerrorlen_s(err); // why not c11?
        char* errMsg = malloc(1024);
        strerror_s(errMsg,128,err);
        loge("openEntrypoint", "failed 2 open entrypoint file:%s", errMsg);
        free(errMsg);
        free(epPath);
        PyGILState_Release(gstate);
        return ERR_EP_CANT_OPEN;
    }
    logd("openEntrypoint","open entrypoint file %s done",epPath);
    //logd("openEntrypoint","fd addr %d",epFd);
    //logd("openEntrypoint","d addr %d",pyo__main___d);
    //logd("openEntrypoint","d addr %d",PyRun_FileEx);
    free(epPath);
    
    // run entrypoint
    //PyImport_ImportModule("cqapi"); let user import it is also fine
    py_botObj = PyRun_FileEx(epFd, "__entrypoint__.py", Py_file_input, pyoGlobalDict, pyoGlobalDict,1);
    if (NULL == py_botObj){
        loge("runEntryPoint",TEXT_LOADENTRYPOINT_FAIL);
        catchPyExc();
        PyEval_ReleaseThread(PyThreadState_Get());
        return ERR_EP_FAIL;
    }
    Py_INCREF(py_botObj);

    py_botObj = PyDict_GetItemString(pyoGlobalDict, "__bot__");
    if (NULL == py_botObj) {
        logx("runEntryPoint", CQLOG_WARNING, TEXT_NOBOTGLOBALVAR);
        return 0;
    }
    py_entrypointLoaded = true;

    PyGILState_Release(gstate);
    //fuckGIL();
    return 0;
}

/*
*  卸载入口点，结束python解释器
*    这个函数可能并不能完全的清干净内存，如果发现泄露请及时报告
*/
void py_end() {
    if (py_intLoaded) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();
        Py_Finalize();
        //PyGILState_Release(gstate);
    }
}

time_t lastWarned = 0;
int warnLevel = CQLOG_WARNING;
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
    // warn not initedthings and warn rate limit
    if (!(py_moduleLoaded && py_intLoaded && py_entrypointLoaded)) {
        if (time(NULL) - lastWarned > WARNCOOLDOWN) {
            warnLevel = CQLOG_WARNING;
        }
        else {
            warnLevel = CQLOG_DEBUG;
        }
        lastWarned = time(NULL);
        if (!py_moduleLoaded) {
            logx("callCallback", warnLevel,"module is not loaded, maybe callback called too early");
        }
        if (!py_intLoaded) {
            logx("callCallback", warnLevel, "py interpreter not intialized, maybe callback called too early");
        }
        if (!py_entrypointLoaded) {
            logx("callCallback", warnLevel, "entryPoint not intialized, maybe callback called too early or ep file corrupt");
        }
        return EVENT_IGNORE;
    }

    // get py GIL
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    // build argsTuple
    va_list arg;
    va_start(arg, format);
    int retCode = EVENT_IGNORE;
    PyObject *eventTuple = Py_VaBuildValue(format, arg);
    va_end(arg);
    if (NULL == eventTuple) {
        //loge("callCallback", "fail!");
        PyGILState_Release(gstate);
        catchPyExc();
        return retCode;
    }
    PyObject *argsTuple = Py_BuildValue("sO", eventName,eventTuple);
    if (NULL == argsTuple) {
        //loge("callCallback", "fail!");
        PyGILState_Release(gstate);
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
        PyGILState_Release(gstate);
        return retCode;
    }
    if (!PyCallable_Check(pyoCb)) {
        loge("callCallback", "emitter is not callable");
        PyGILState_Release(gstate);
        return retCode;
    }
#ifdef _DEBUG
    logd("callCallback", "method: %s", PyUnicode_AsUTF8(PyObject_Repr(pyoCb)));
#endif // _DEBUG
    PyObject *pyoRet = PyObject_Call(pyoCb, argsTuple, PyDict_New());
    if (NULL == pyoRet) {
        catchPyExc();
        PyGILState_Release(gstate);
        return retCode;
    }

    // receive return value from __bot__.emit
    if (!PyArg_Parse(pyoRet, "i", &retCode)) {
        PyObject * ptype, * pvalue, * ptraceback;
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);// clear pyerr
        logi("callCallback", "failed convert return value(%s) of emitter(%s)", PyUnicode_AsUTF8(PyObject_Repr(pyoRet)), PyUnicode_AsUTF8(PyObject_Repr(pyoCb)));
        PyGILState_Release(gstate);
        return EVENT_IGNORE;
    }

    // release GIL
    PyGILState_Release(gstate);
    //fuckGIL();
    return retCode;
}