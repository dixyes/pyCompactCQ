/*
* pyc Python Compact Library - menu callbacks
* copyright 2017-2018 dixyes
* MIT License
*/

#include "cqp.h"

#include <windows.h>
#include "texts.h"
#include "util.h"
#include "py.h"

HMODULE mShcore = NULL;
HRESULT  (__stdcall *_SetProcessDpiAwareness)(int value) = NULL;
HRESULT (__stdcall *_GetProcessDpiAwareness)(HANDLE hprocess, int *value);

int oldDpiAwareness = -1;
int setDpiSupport = 0;
inline void fuckdpi(void) {
    if (0 == setDpiSupport) {
        mShcore = LoadLibraryExW(L"SHCore.dll", NULL, 0);
        if (NULL == mShcore) {
            setDpiSupport = -1;
            logd("fuckdpi","fuckdpi not supp0rt: no dll");
            return;
        } 
        _SetProcessDpiAwareness = (HRESULT(__stdcall *)(int))GetProcAddress(mShcore, "SetProcessDpiAwareness");
        if (NULL == _SetProcessDpiAwareness) {
            logd("fuckdpi", "fuckdpi not supp0rt: no spda");
            setDpiSupport = -2;
            return;
        }
        _GetProcessDpiAwareness = (HRESULT(__stdcall *)(HANDLE, int *))GetProcAddress(mShcore, "GetProcessDpiAwareness");
        if (NULL == _GetProcessDpiAwareness) {
            logd("fuckdpi", "fuckdpi not supp0rt: no gpda");
            setDpiSupport = -3;
            return;
        }
        setDpiSupport = 1;
    }
    if (oldDpiAwareness < 0) {
        if (S_OK == _GetProcessDpiAwareness(NULL, &oldDpiAwareness)) {
            logd("fuckdpi", "oldvalue is %d", oldDpiAwareness);
        }
    }
    if (1 == setDpiSupport) {
        logd("fuckdpi", "setting 2");
        _SetProcessDpiAwareness(2);
    }
}

inline void defuckdpi(void) {
    if (1 == setDpiSupport) {
        logd("fuckdpi", "unsetting 2 to %d", oldDpiAwareness);
        _SetProcessDpiAwareness(oldDpiAwareness);
    }
}

#ifdef _DEBUG
CQEVENT(int32_t, __menuReloadScript, 0)() {
    logi("reloadEntrypoint", TEXT_REQUIRERELOAD);
    py_finalize();
    py_init();
    py_initEp();
    return 0;
}
#endif // _DEBUG
CQEVENT(int32_t, __menuLoadModule, 0)() {
    logi("loadSelectedModule", TEXT_RELOADDLL);
    if (py_msmLoaded) {
#ifdef _DEBUG
        logd("loadSelectedModule", "forceing unload python mechanism in debug build");
        py_finalize();
        py_msmLoaded = false;
#else
        //logx("loadSelectedModule", LOGGER_WARNING, "nope,you can't REload dll");
        MessageBoxW(NULL,LTEXT_NORELOADDLL,LTEXT_NORELOADDLLTITLE,MB_ICONERROR|MB_OK);
        return -1;
#endif
    }
    if (py_moduleLoaded) {
        py_unload();
    }
    OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"Python Debug DLL\0python3*_d.dll\0\0";
    ofn.lpstrFile = L"%ProgramFiles%\\Python37-32\\python37_d.dll";
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = LTEXT_CHOOSEPYTHON;
    ofn.Flags = OFN_NONETWORKBUTTON | OFN_FILEMUSTEXIST;
    fuckdpi();
    if (!GetOpenFileNameW(&ofn))
        return -1;
    defuckdpi();
    logi("loadSelectedModule", TEXT_CHOSENPYTHON, ofn.lpstrFile);
    py_load(ofn.lpstrFile);
    py_init();
    py_initEp();
    return 0;
}

CQEVENT(int32_t, __menuShowLicense, 0)() {
    //logi("showLicense",);
    fuckdpi();
    MessageBoxW(NULL, LTEXT_PSF, LTEXT_PSFTITLE, MB_OK);
    MessageBoxW(NULL, LTEXT_MITSELF, LTEXT_SELFTITLE, MB_OK);
    defuckdpi();
    return 0;
}
