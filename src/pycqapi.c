/*
* pyc Python Compact Library - python "cqapi" module
* copyright 2017-2018 dixyes
* MIT License
*/

#include "util.h"
#include "cqp.h"
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
PyMODINIT_FUNC PyInit_cqapi(void) {
    logd("cqapiInit", "initializing cqapi Python module");
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
    PyModule_AddIntConstant(module, "LOGGER_INFO", LOGGER_INFO);// ��Ϣ ��ɫ
    PyModule_AddIntConstant(module, "LOGGER_INFOSUCCESS", LOGGER_INFOSUCCESS);// ��Ϣ(�ɹ�) ��ɫ
    PyModule_AddIntConstant(module, "LOGGER_INFORECV", LOGGER_INFORECV);// ��Ϣ(����) ��ɫ
    PyModule_AddIntConstant(module, "LOGGER_INFOSEND", LOGGER_INFOSEND);// ��Ϣ(����) ��ɫ
    PyModule_AddIntConstant(module, "LOGGER_WARNING", LOGGER_WARNING);// ���� ��ɫ
    PyModule_AddIntConstant(module, "LOGGER_ERROR", LOGGER_ERROR);// ���� ��ɫ
    PyModule_AddIntConstant(module, "CQLOG_FATAL", CQLOG_FATAL);// �������� ���
    PyGILState_Release(gstate);
    return module;
}

// ������CQAPIs�ķ�װʵ��
/*
* ʹ��CQ_AddLog��¼obj.__repr__()����־
* ��Ӧpy��api cqapi.logger(obj,type=cqapi.LOGGER_INFO,type=cqapi.LOGGER_INFO,)
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
    int type = LOGGER_INFO;
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
    //fuckGIL();
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