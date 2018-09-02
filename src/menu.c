/*
* pyc Python Compact Library - menu callbacks
* copyright 2017-2018 dixyes
* MIT License
*/

#include "cqp.h"

#include "texts.h"
#include "util.h"
#include "py.h"

CQEVENT(int32_t, __menuReloadScript, 0)() {
    logi("reloadEntrypoint", TEXT_REQUIRERELOAD);
    py_end();
    py_init();
    return 0;
}
CQEVENT(int32_t, __menuLoadModule, 0)() {
    logi("loadSelectedModule", TEXT_RELOADDLL);
    py_end();
    if (py_moduleLoaded) {
        py_unload();
    }
    OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"Python3 DLL\0Python3*.dll\0\0";
    ofn.lpstrFile = L"%ProgramFiles%\\Python37-32";
    ofn.nMaxFile = MAX_PATH;
    ofn.lpstrTitle = LTEXT_CHOOSEPYTHON;
    ofn.Flags = OFN_NONETWORKBUTTON | OFN_FILEMUSTEXIST;
    if (!GetOpenFileNameW(&ofn))
        return -1;
    logi("loadSelectedModule", TEXT_CHOSENPYTHON, ofn.lpstrFile);
    py_load(ofn.lpstrFile);
    py_init();
    return 0;
}
