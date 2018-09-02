/*
* pyc Python Compact Library - python struct header
* copyright 2017-2018 dixyes
* MIT License
*/

// py.c status enum
#define ERR_EP_FAIL -7
#define ERR_FUNCTION_INIT_FAIL -6
#define ERR_EP_CANT_OPEN -5
#define ERR_MODULE_NOT_LOAD -4
#define ERR_PY_INIT_FAIL -3
#define ERR_PYAPI_FAIL -2
#define PY_NOT_INIT -1

// python things
#if !defined(_PY_STRUCT_HEADER) && !defined(_PY_HEADER)
#define _PY_STRUCT_HEADER

// pyapi enum
#define Py_single_input 256
#define Py_file_input 257
#define Py_eval_input 258
#define METH_VARARGS 0x0001
#define METH_KEYWORDS 0x0002
#define METH_NOARGS   0x0004
#define METH_O 0x0008

// structs
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
typedef int32_t Py_ssize_t;
typedef Py_ssize_t Py_hash_t;

#ifdef _DEBUG
#define _PyObject_HEAD_EXTRA            \
    struct _object *_ob_next;           \
    struct _object *_ob_prev;
#define _PyObject_EXTRA_INIT 0, 0,
#else
#define _PyObject_HEAD_EXTRA
#define _PyObject_EXTRA_INIT
#endif
typedef struct _object {
    _PyObject_HEAD_EXTRA
    Py_ssize_t ob_refcnt;
    struct _typeobject *ob_type;
} PyObject;
typedef struct {
    PyObject ob_base;
    Py_ssize_t ob_size;
} PyVarObject;
typedef struct bufferinfo {
    void *buf;
    PyObject *obj;
    Py_ssize_t len;
    Py_ssize_t itemsize;

    int readonly;
    int ndim;
    char *format;
    Py_ssize_t *shape;
    Py_ssize_t *strides;
    Py_ssize_t *suboffsets;
    void *internal;
} Py_buffer;

typedef void (*freefunc)(void *);
typedef void (*destructor)(PyObject *);
typedef int (*printfunc)(PyObject *, FILE *, int);
typedef PyObject *(*getattrfunc)(PyObject *, char *);
typedef PyObject *(*getattrofunc)(PyObject *, PyObject *);
typedef int (*setattrfunc)(PyObject *, char *, PyObject *);
typedef int (*setattrofunc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*reprfunc)(PyObject *);
typedef Py_hash_t (*hashfunc)(PyObject *);
typedef PyObject *(*richcmpfunc) (PyObject *, PyObject *, int);
typedef PyObject *(*getiterfunc) (PyObject *);
typedef PyObject *(*iternextfunc) (PyObject *);
typedef PyObject *(*descrgetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*descrsetfunc) (PyObject *, PyObject *, PyObject *);
typedef int (*initproc)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*newfunc)(struct _typeobject *, PyObject *, PyObject *);
typedef PyObject *(*allocfunc)(struct _typeobject *, Py_ssize_t);
typedef PyObject * (*unaryfunc)(PyObject *);
typedef PyObject * (*binaryfunc)(PyObject *, PyObject *);
typedef PyObject * (*ternaryfunc)(PyObject *, PyObject *, PyObject *);
typedef int (*inquiry)(PyObject *);
typedef Py_ssize_t (*lenfunc)(PyObject *);
typedef PyObject *(*ssizeargfunc)(PyObject *, Py_ssize_t);
typedef PyObject *(*ssizessizeargfunc)(PyObject *, Py_ssize_t, Py_ssize_t);
typedef int(*ssizeobjargproc)(PyObject *, Py_ssize_t, PyObject *);
typedef int(*ssizessizeobjargproc)(PyObject *, Py_ssize_t, Py_ssize_t, PyObject *);
typedef int(*objobjargproc)(PyObject *, PyObject *, PyObject *);
typedef int (*getbufferproc)(PyObject *, Py_buffer *, int);
typedef void (*releasebufferproc)(PyObject *, Py_buffer *);
typedef int (*objobjproc)(PyObject *, PyObject *);
typedef int (*visitproc)(PyObject *, void *);
typedef int (*traverseproc)(PyObject *, visitproc, void *);

typedef struct {
    binaryfunc nb_add;
    binaryfunc nb_subtract;
    binaryfunc nb_multiply;
    binaryfunc nb_remainder;
    binaryfunc nb_divmod;
    ternaryfunc nb_power;
    unaryfunc nb_negative;
    unaryfunc nb_positive;
    unaryfunc nb_absolute;
    inquiry nb_bool;
    unaryfunc nb_invert;
    binaryfunc nb_lshift;
    binaryfunc nb_rshift;
    binaryfunc nb_and;
    binaryfunc nb_xor;
    binaryfunc nb_or;
    unaryfunc nb_int;
    void *nb_reserved;
    unaryfunc nb_float;
    binaryfunc nb_inplace_add;
    binaryfunc nb_inplace_subtract;
    binaryfunc nb_inplace_multiply;
    binaryfunc nb_inplace_remainder;
    ternaryfunc nb_inplace_power;
    binaryfunc nb_inplace_lshift;
    binaryfunc nb_inplace_rshift;
    binaryfunc nb_inplace_and;
    binaryfunc nb_inplace_xor;
    binaryfunc nb_inplace_or;
    binaryfunc nb_floor_divide;
    binaryfunc nb_true_divide;
    binaryfunc nb_inplace_floor_divide;
    binaryfunc nb_inplace_true_divide;
    unaryfunc nb_index;
    binaryfunc nb_matrix_multiply;
    binaryfunc nb_inplace_matrix_multiply;
} PyNumberMethods;
typedef struct {
    lenfunc sq_length;
    binaryfunc sq_concat;
    ssizeargfunc sq_repeat;
    ssizeargfunc sq_item;
    void *was_sq_slice;
    ssizeobjargproc sq_ass_item;
    void *was_sq_ass_slice;
    objobjproc sq_contains;

    binaryfunc sq_inplace_concat;
    ssizeargfunc sq_inplace_repeat;
} PySequenceMethods;

typedef struct {
    lenfunc mp_length;
    binaryfunc mp_subscript;
    objobjargproc mp_ass_subscript;
} PyMappingMethods;

typedef struct {
    unaryfunc am_await;
    unaryfunc am_aiter;
    unaryfunc am_anext;
} PyAsyncMethods;

typedef struct {
     getbufferproc bf_getbuffer;
     releasebufferproc bf_releasebuffer;
} PyBufferProcs;

typedef struct _typeobject {
    PyVarObject ob_base;
    const char *tp_name;
    Py_ssize_t tp_basicsize, tp_itemsize;
    destructor tp_dealloc;
    printfunc tp_print;
    getattrfunc tp_getattr;
    setattrfunc tp_setattr;
    PyAsyncMethods *tp_as_async;
    reprfunc tp_repr;
    PyNumberMethods *tp_as_number;
    PySequenceMethods *tp_as_sequence;
    PyMappingMethods *tp_as_mapping;
    hashfunc tp_hash;
    ternaryfunc tp_call;
    reprfunc tp_str;
    getattrofunc tp_getattro;
    setattrofunc tp_setattro;
    PyBufferProcs *tp_as_buffer;
    unsigned long tp_flags;
    const char *tp_doc;
    traverseproc tp_traverse;
    inquiry tp_clear;
    richcmpfunc tp_richcompare;
    Py_ssize_t tp_weaklistoffset;
    getiterfunc tp_iter;
    iternextfunc tp_iternext;
    struct PyMethodDef *tp_methods;
    struct PyMemberDef *tp_members;
    struct PyGetSetDef *tp_getset;
    struct _typeobject *tp_base;
    PyObject *tp_dict;
    descrgetfunc tp_descr_get;
    descrsetfunc tp_descr_set;
    Py_ssize_t tp_dictoffset;
    initproc tp_init;
    allocfunc tp_alloc;
    newfunc tp_new;
    freefunc tp_free;
    inquiry tp_is_gc;
    PyObject *tp_bases;
    PyObject *tp_mro;
    PyObject *tp_cache;
    PyObject *tp_subclasses;
    PyObject *tp_weaklist;
    destructor tp_del;
    unsigned int tp_version_tag;
    destructor tp_finalize;
} PyTypeObject;

typedef PyObject *(*PyCFunction)(PyObject *, PyObject *);
typedef PyObject *(*_PyCFunctionFast) (PyObject *, PyObject *const *, Py_ssize_t);
typedef PyObject *(*PyCFunctionWithKeywords)(PyObject *, PyObject *, PyObject *);
typedef PyObject *(*_PyCFunctionFastWithKeywords) (PyObject *, PyObject *const *, Py_ssize_t, PyObject *);
typedef PyObject *(*PyNoArgsFunction)(PyObject *);


typedef struct  {
    int cf_flags;
} PyCompilerFlags;

typedef struct PyMethodDef {
    const char *ml_name;
    PyCFunction ml_meth;
    int ml_flags;
    const char *ml_doc;
} PyMethodDef;

typedef struct PyModuleDef_Base {
    PyObject ob_base;
    PyObject* (*m_init)(void);
    Py_ssize_t m_index;
    PyObject* m_copy;
} PyModuleDef_Base;

typedef struct PyModuleDef_Slot {
    int slot;
    void *value;
} PyModuleDef_Slot;

typedef struct PyModuleDef {
    PyModuleDef_Base m_base;
    const char* m_name;
    const char* m_doc;
    Py_ssize_t m_size;
    PyMethodDef *m_methods;
    struct PyModuleDef_Slot* m_slots;
    traverseproc m_traverse;
    inquiry m_clear;
    freefunc m_free;
} PyModuleDef;
typedef enum {
    PyGILState_LOCKED,
    PyGILState_UNLOCKED
}PyGILState_STATE;
// im not fucking care abot this struct
typedef struct _ts PyThreadState;


// py api macros
#define Py_INCREF(op) (((PyObject *)(op))->ob_refcnt++)
#define Py_DECREF(op) \
do {\
    PyObject *_py_decref_tmp = (PyObject *)(op);\
    if ( --(_py_decref_tmp)->ob_refcnt == 0)\
    {\
        (*(((PyObject*)(_py_decref_tmp))->ob_type)->tp_dealloc)((PyObject *)(_py_decref_tmp));\
    };\
} while (0);
#define Py_RETURN_NONE Py_INCREF(Py_None); return *Py_None
#define PyObject_HEAD_INIT(type)        \
    { _PyObject_EXTRA_INIT              \
    1, type },
#define PyModuleDef_HEAD_INIT { \
    PyObject_HEAD_INIT(NULL)    \
    NULL, /* m_init */          \
    0,    /* m_index */         \
    NULL, /* m_copy */          \
  }
#define Py_None (&_Py_NoneStruct)
#define PyMODINIT_FUNC __declspec(dllexport) PyObject*

// py api functions
// marks are used in generate loadFunctions
// DONOT REMOVE OR CHANGE THE MARK BELOW AND END MARK
/*MAKR*/
void (*Py_InitializeEx) (int);
int (*Py_IsInitialized) (void);
void (*Py_Finalize) (void);
int (*PyImport_AppendInittab) (const char *name, PyObject* (*initfunc)(void));
PyObject *(*PyImport_ImportModule) (const char * name);
PyObject *(*PyImport_AddModule) (const char * name);
PyObject *(*PyObject_GetAttrString) (PyObject * obj, const char * str);
int (*PyList_Append)(PyObject *list, PyObject *item);
PyObject *(*PyUnicode_FromString) (const char *u);
PyObject* (*PyModule_GetDict) (PyObject *module);
PyObject* (*PyRun_FileEx) (FILE *fp, const char *filename, int start, PyObject *globals, PyObject *locals, int closeit);
void (*PyErr_Fetch)(PyObject **ptype, PyObject **pvalue, PyObject **ptraceback);
PyObject* (*PyObject_Repr)(PyObject *o);
PyObject * (*PyEval_CallMethod)(PyObject *obj, const char *name, const char *format, ...);
const char* (*PyUnicode_AsUTF8) (PyObject *unicode);
PyObject* (*PyDict_GetItemString)(PyObject *p, const char *key);
int (*PyArg_ParseTuple)(PyObject *args, const char *format, ...);
int(*PyArg_VaParseTupleAndKeywords)(PyObject *args, PyObject *kw, const char *format, char *keywords[], va_list vargs);
int(*PyArg_ParseTupleAndKeywords)(PyObject *args, PyObject *kw, const char *format, char *keywords[], ...);
PyObject* _Py_NoneStruct;
int(*PyModule_AddIntConstant)(PyObject *module, const char *name, long value);
int (*PyErr_BadArgument)(void);
int(*PyModule_AddStringConstant)(PyObject *module, const char *name, const char *value);
PyObject* (*PyLong_FromLong)(long v);
PyObject* (*PyLong_FromLongLong)(long long v);
PyTypeObject * PyUnicode_Type;
PyObject* (*PyUnicode_AsEncodedString)(PyObject *unicode, const char *encoding, const char *errors);
char* (*PyBytes_AsString)(PyObject *o);
PyObject* (*Py_VaBuildValue)(const char *format, va_list vargs);
int (*PyCallable_Check)(PyObject *o);
PyObject* (*PyObject_Call)(PyObject *callable, PyObject *args, PyObject *kwargs);
PyObject* (*PyDict_New)();
int (*PyArg_Parse)(PyObject *args, char *format, ...);
void (*PyEval_InitThreads)(void);
int (*PyGILState_Check)(void);
PyGILState_STATE(*PyGILState_Ensure)(void);
void(*PyGILState_Release)(PyGILState_STATE);
void (*PyEval_ReleaseThread)(PyThreadState *tstate);
PyThreadState *(*PyThreadState_Get)(void);
PyObject* (*Py_BuildValue)(const char *format, ...);
/*ENDMAKR*/
//#define PyGILState_Ensure() PyGILState_UNLOCKED
//#define PyGILState_Release(x)
PyObject * (*PyModule_Create2)(struct PyModuleDef*,int apiver);
#define PyModule_Create(x) PyModule_Create2(x, 1013) // this api version not same as python, see pydir/include/modsupport.hL132

#endif // _PY_STRUCT_HEADER