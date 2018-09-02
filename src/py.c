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
#include "pys.h"

// if cqapi module APIs and __bot__ defination changed, changed this also
#define PYCQAPI_VERSION "0.0.1"

// documents, use sphinx style (maybe)
#define DOC_CQAPI "\
CoolQ APIs.\
"
#define DOC_CQLOGGER "\
log obj.__repr__() with CoolQ API CQ_addLog.\
\
:param Object obj: that object to log.\
:param int type: this log type mentioned in cqp.h, use cqapi.CQLOG_xxx to set this.\
:param str tag: tag of this log.\
:return: returns nothing\
:throw TypeError: if argumets have bad type.\
"
#define DOC_CQSENDPM "\
send private message to someone using CoolQ API CQ_sendPrivateMsg.\
\
:param int to: the qqid of the one you want send to.\
:param str msg: the message you want send.\
:return int: if succeed returns the id you send to\
:throw TypeError: if argumets have bad type.\
"
#define DOC_CQSENDGM "\
send group message to some group using CoolQ API CQ_sendGroupMsg.\
\
:param int to: the groupid of the one you want send to.\
:param str msg: the message you want send.\
:return int: if succeed returns the id you send to\
:throw TypeError: if argumets have bad type.\
"
#define DOC_CQSENDDM "\
send discuss message to some discussion using CoolQ API CQ_sendDiscussMsg.\
\
:param int to: the discussid of the one you want send to.\
:param str msg: the message you want send.\
:return int: if succeed returns the id you send to\
:throw TypeError: if argumets have bad type.\
"
#define DOC_CQDELMSG "\
delete message have sent using CoolQ API CQ_deleteMsg.\
\
:param int msgId: the messageid you want delete.\
:return int: unknown from CQ_deleteMsg\
:throw TypeError: if argumets have bad type.\
"
#define DOC_CQSENDLIKE "\
send like using CoolQ API CQ_sendLike.\
\
:param int id: the qqid you want like.\
:return int: unknown from CQ_sendLike\
:throw TypeError: if argumets have bad type.\
"
#ifdef _DEBUG
#define fuckGIL() {logd("fuckGIL","gil is %d", PyGILState_Check());}
#else
#define fuckGIL()
#endif

HANDLE py_module = NULL;
bool py_moduleLoaded = false;
PyObject *py_botObj = NULL;
bool py_entrypointLoaded = false;
bool py_intLoaded = false;
//char * _cqLoggerBuf;

PyObject* ptype, *pvalue, *ptraceback;

/*
* ��ӡpython exception
*/
void _catchPyExc(const char * ln){
    PyErr_Fetch(&ptype, &pvalue, &ptraceback); 
    logx("pyException", CQLOG_ERROR, "%s etype: %s", ln, PyUnicode_AsUTF8(PyObject_Repr(ptype)));
    logx("pyException", CQLOG_ERROR, "%s evalue: %s", ln, PyUnicode_AsUTF8(PyObject_Repr(pvalue)));
    PyObject* pyoExcText = PyEval_CallMethod(ptraceback, "format_exc", "()"); 
    if (NULL != pyoExcText) {    
        logx("pyException", CQLOG_ERROR, "%s stacktrace: %s", ln, PyUnicode_AsUTF8(PyObject_Repr(pyoExcText)));
    }
    else {   
        logx("pyException", CQLOG_ERROR, "%s no stacktrace", ln);
    }
}
#define catchPyExc() _catchPyExc(__FILE__ ":" LINE_STRING)

#define _checkPyFunction(name) {\
    if(NULL==name){\
        loge("loadFunction", #name " failed");\
        showLe(#name);\
        return ERR_FUNCTION_INIT_FAIL;\
    };\
}
/*
* װ��python3x.dll
* @param dllName Ҫʹ�õ�dll���ƣ�NULLʱʹ��Ĭ�ϵ�python37_d.dll��python73.dll
* @return ����װ�ؽ��ö��ֵ
*         ERR_EP_CANT_OPEN �򲻿���ڵ��ļ��������ڵ��ļ��Ƿ�λ��appdirectionary
*         ERR_MODULE_NOT_LOAD dllδװ��
*         S_OK �ɹ�װ��
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
* ж��python3x.dll
*  ����������ܲ�������ȫ����ɾ��ڴ棬�������й¶�뼰ʱ����
* @return ����ж�ؽ��(��ɹ� ��ʧ��)
*/
bool py_unload() {
    if (!py_moduleLoaded) {
        logx("unloadModule", CQLOG_WARNING, "calling py_unload with module not load!");
        return false;
    };
    BOOL unloaded = FreeLibrary(py_module);
    if (!unloaded) {
        showLe("unloadModule");
    }
    return unloaded;
}

PyMODINIT_FUNC PyInit_cqapi(void); // ��������������PyImport_AppendInittab


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
* ��ʼ��python�����/������/��������ɶ,������ڵ�
* @return ���س�ʼ�����ö�٣�
*         ERR_PYAPI_FAIL Python APIʧ��
*         ERR_EP_CANT_OPEN ep�ļ��򲻿�
*         ERR_EP_FAIL epִ��ʧ��
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
    fuckGIL();
    
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
    fuckGIL();
    return 0;
}

/*
*  ж����ڵ㣬����python������
*    ����������ܲ�������ȫ����ɾ��ڴ棬�������й¶�뼰ʱ����
*/
void py_end() {
    if (py_intLoaded) {
        PyGILState_STATE gstate;
        gstate = PyGILState_Ensure();
        Py_Finalize();
        //PyGILState_Release(gstate);
    }
}
// ��ǰ������api��װ����

static PyObject * cqLogger(PyObject *self, PyObject *args, PyObject *kw);
static PyObject * cqSendPM(PyObject *self, PyObject *args);
static PyObject * cqSendGM(PyObject *self, PyObject *args);
static PyObject * cqSendDM(PyObject *self, PyObject *args);
static PyObject * cqDeleteMsg(PyObject *self, PyObject *args);
static PyObject * cqSendLike(PyObject *self, PyObject *args);
// ��������cqapi pymodule
static PyMethodDef cqapiMethodDefs[] = {
    { "logger", (PyCFunction)cqLogger, METH_VARARGS | METH_KEYWORDS, DOC_CQLOGGER },
    { "sendpm", cqSendPM, METH_VARARGS, DOC_CQSENDPM },
    { "sendgm", cqSendGM, METH_VARARGS, DOC_CQSENDGM },
    { "senddm", cqSendDM, METH_VARARGS, DOC_CQSENDDM },
    { "delmsg", cqDeleteMsg, METH_VARARGS, DOC_CQDELMSG },
    { "sendlike", cqSendLike, METH_VARARGS, DOC_CQSENDLIKE },
    { NULL, NULL, 0, NULL }        /* Sentinel */
};
static struct PyModuleDef cqapiModuleDef = {
    PyModuleDef_HEAD_INIT,
    "cqapi", // module name
    DOC_CQAPI, // doc
    -1,
    cqapiMethodDefs,
    NULL,
    NULL,
    NULL,
    NULL
}; 


/*
* cqapi.__init__()��cʵ��
*/
PyMODINIT_FUNC PyInit_cqapi(void){
    logd("cqapiInit","initializing cqapi Python module");
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    PyObject* module = PyModule_Create(&cqapiModuleDef);
    // there constants is from cqp.h
    PyModule_AddStringConstant(module, "__author__", "dixyes");// set this as you like
    PyModule_AddStringConstant(module, "__copyright__", "Copyright 2017-2018, dixyes");
    PyModule_AddStringConstant(module, "__license__", "MIT"); // if you change license, change this also
    PyModule_AddStringConstant(module, "__version__", PYCQAPI_VERSION);
    PyModule_AddStringConstant(module, "__email__", "me@dixy.es"); // use your own email if you want

    PyModule_AddIntConstant(module, "EVENT_IGNORE", EVENT_IGNORE);// �¼�_����
    PyModule_AddIntConstant(module, "EVENT_BLOCK", EVENT_BLOCK);// �¼�_����
    PyModule_AddIntConstant(module, "REQUEST_ALLOW", REQUEST_ALLOW);// ����_ͨ��
    PyModule_AddIntConstant(module, "REQUEST_DENY", REQUEST_DENY);// ����_�ܾ�
    PyModule_AddIntConstant(module, "REQUEST_GROUPADD", REQUEST_GROUPADD);// ����_Ⱥ���
    PyModule_AddIntConstant(module, "REQUEST_GROUPINVITE", REQUEST_GROUPINVITE);// ����_Ⱥ����
    PyModule_AddIntConstant(module, "CQLOG_DEBUG", CQLOG_DEBUG);// ���� ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_INFO", CQLOG_INFO);// ��Ϣ ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_INFOSUCCESS", CQLOG_INFOSUCCESS);// ��Ϣ(�ɹ�) ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_INFORECV", CQLOG_INFORECV);// ��Ϣ(����) ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_INFOSEND", CQLOG_INFOSEND);// ��Ϣ(����) ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_WARNING", CQLOG_WARNING);// ���� ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_ERROR", CQLOG_ERROR);// ���� ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_FATAL", CQLOG_FATAL);// �������� ���
    PyGILState_Release(gstate);
    return module;
}

// ������CQAPIs�ķ�װʵ��
/*
* ʹ��CQ_AddLog��¼obj.__repr__()����־
* ��Ӧpy��api cqapi.logger(obj,type=cqapi.CQLOG_INFO,type=cqapi.CQLOG_INFO,)
* ���� obj:Ҫ��¼�Ķ������ɶ����Ҫ��__repr__����
* ��ѡ���� type:��¼��־�����ͣ��ο�cqp.h��ʹ��cqapi.CQLOG_xxx���ã�
* ��ѡ���� tag:��¼��־��tag
* ����:None
* �쳣:��kwarg���Ͳ���ʱ�׳�TypeError�����ܰɣ����������У�
*/
static PyObject * cqLogger(PyObject *self, PyObject *args, PyObject *kw)
{
    // ensure GIL
    //fuckGIL();
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();

    PyObject* obj = NULL;
    const char* tag = "pyLogger";
    int type = CQLOG_INFO;
    static char *kwlist[] = { "obj", "tag", "type", NULL };
    if (!PyArg_ParseTupleAndKeywords(args, kw, "O|sI", kwlist, &obj, &tag, &type)) {
        PyGILState_Release(gstate);
        return NULL;
    }
    logx(tag, type, "%s", PyUnicode_AsUTF8(PyObject_Repr(obj)));

    PyGILState_Release(gstate);
    //fuckGIL();
    Py_RETURN_NONE;
}
/*
* ʹ��CQ_sendXxxMsg����˽��
* ��Ӧpy��api cqapi.sendxm(to,msg) (x=d,p,g)
* ���� to:Ŀ��qq��/Ⱥ��/�������
* ���� msg:���͵���Ϣ
* ����: �ɹ�ʱ��Ϣid�����ɹ�cqp.hûд �Ҳ�֪����ɶ��
* �쳣:��kwarg���Ͳ���ʱ�׳�TypeError�����ܰɣ����������У�
*/
PyObject * cqSendXM(PyObject *self, PyObject *args, int32_t(__stdcall *cb)(int32_t, int64_t, const char *))
{
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    int64_t to;
    PyObject * msg;
    if (!PyArg_ParseTuple(args, "LO!", &to, PyUnicode_Type, &msg)) {
        catchPyExc();
        PyGILState_Release(gstate);
        return NULL;
    }
    PyObject * encoded = PyUnicode_AsEncodedString(msg, "gb2312", "strict");
    if (NULL == encoded) {
        catchPyExc();
        PyGILState_Release(gstate);
        return NULL;
    }
    char* pGB2312Text = PyBytes_AsString(encoded);
    if (NULL == pGB2312Text) {
        catchPyExc();
        PyGILState_Release(gstate);
        return NULL;
    }
#ifdef _DEBUG
    logd("sendMsg", "sendto: %lld,msg:%s", to, pGB2312Text);
#endif
    int64_t ret = cb(ac, to, pGB2312Text);
    //free(pGB2312Text);
    PyGILState_Release(gstate);
    return PyLong_FromLongLong(ret);
}
static PyObject * cqSendPM(PyObject *self, PyObject *args) {
    return cqSendXM(self, args, CQ_sendPrivateMsg);
}
static PyObject * cqSendGM(PyObject *self, PyObject *args) {
    return cqSendXM(self, args, CQ_sendGroupMsg);
}
static PyObject * cqSendDM(PyObject *self, PyObject *args) {
    return cqSendXM(self, args, CQ_sendDiscussMsg);
}

/*
* ʹ��CQ_deleteMsg������Ϣ
* ��Ӧpy��api cqapi.delmsg(msgId)
* ���� msgId:Ŀ��qq��/Ⱥ��/�������
* ����: ?? ��֪����ԭ������
* �쳣:��kwarg���Ͳ���ʱ�׳�TypeError�����ܰɣ����������У�
*/
static PyObject * cqDeleteMsg(PyObject *self, PyObject *args)
{
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    int32_t msgId;
    if (!PyArg_ParseTuple(args, "I", &msgId)) {
        catchPyExc();
        PyGILState_Release(gstate);
        return NULL;
    }
    int32_t ret = CQ_deleteMsg(ac, msgId);
    PyGILState_Release(gstate);
    return PyLong_FromLong(ret);
}
/*
* ʹ��CQ_sendLike��
* ��Ӧpy��api cqapi.sendlike(msgId)
* ���� id:Ŀ��qq��
* ����: ?? ��֪����ԭ������
* �쳣:��kwarg���Ͳ���ʱ�׳�TypeError�����ܰɣ����������У�
*/
static PyObject * cqSendLike(PyObject *self, PyObject *args)
{
    fuckGIL();
    PyGILState_STATE gstate;
    gstate = PyGILState_Ensure();
    int64_t id;
    if (!PyArg_ParseTuple(args, "L", &id)) {
        catchPyExc();
        PyGILState_Release(gstate);
        return NULL;
    }
    int32_t ret = CQ_sendLike(ac, id);
    PyGILState_Release(gstate);
    return PyLong_FromLong(ret);
}

time_t lastWarned = 0;
int warnLevel = CQLOG_WARNING;
#define WARNCOOLDOWN  (time_t)3
/*
* ����__bot__���¼��ص�
*   PYCQAPI_VERSION ��ǰ��Ϊ
*             0.0.1 �����ȡ__bot__��emit������������formatͨ��Py_VaBuildValue��ȡ�ɱ������eventTuple������emit(__bot__,eventName,eventTuple)
* @param cbName �¼�����
* @param format �¼�����
* @param ... �¼�����
* @return ����enum{EVENT_BLOCK;EVENT_IGNORE};
* @note ����������ʱ���������abort/�׳��쳣��������־������EVENT_IGNORE
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
        PyErr_Fetch(&ptype, &pvalue, &ptraceback);
        logi("callCallback", "failed convert return value(%s) of emitter(%s)", PyUnicode_AsUTF8(PyObject_Repr(pyoRet)), PyUnicode_AsUTF8(PyObject_Repr(pyoCb)));
        PyGILState_Release(gstate);
        return EVENT_IGNORE;
    }

    // release GIL
    PyGILState_Release(gstate);
    fuckGIL();
    return retCode;
}