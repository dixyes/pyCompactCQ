/*
* pyc Python Compact Library - menu callbacks
* copyright 2017-2018 dixyes
* MIT License
*/

#include "cqp.h"

#include "texts.h"
#include "util.h"
#include "py.h"

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
    ofn.lpstrFilter = L"Python Debug DLL\0python3*_d.dll\0Python DLL\0python3*.dll\0\0";
    ofn.lpstrFile = L"%ProgramFiles%\\Python37-32\\python37_d.dll";
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = LTEXT_CHOOSEPYTHON;
    ofn.Flags = OFN_NONETWORKBUTTON | OFN_FILEMUSTEXIST;
    if (!GetOpenFileNameW(&ofn))
        return -1;
    logi("loadSelectedModule", TEXT_CHOSENPYTHON, ofn.lpstrFile);
    py_load(ofn.lpstrFile);
    py_init();
    py_initEp();
    return 0;
}

CQEVENT(int32_t, __menuShowLicense, 0)() {
    //logi("showLicense",);
    MessageBoxW(NULL, LTEXT_PSF, LTEXT_PSFTITLE, MB_OK);
    MessageBoxW(NULL, LTEXT_MITSELF, LTEXT_SELFTITLE, MB_OK);
    return 0;
}
