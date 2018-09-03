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
    py_endEp();
    py_reinit();
    py_initEp();
    return 0;
}
CQEVENT(int32_t, __menuLoadModule, 0)() {
    logi("loadSelectedModule", TEXT_RELOADDLL); // TODO: 这里漏内存， 然而不同版本api太蛋疼
    if (py_moduleLoaded) {
        py_unload();
    }
    OPENFILENAMEW ofn;
    memset(&ofn, 0, sizeof(ofn));
    ofn.lStructSize = sizeof(ofn);
    ofn.lpstrFilter = L"python3x.dll\0python3*.dll\0\0";
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

CQEVENT(int32_t, __menuShowLicense, 0)() {
    //logi("showLicense",);
    MessageBoxW(NULL, LTEXT_PSF, LTEXT_PSFTITLE, MB_OK);
    MessageBoxW(NULL, LTEXT_MITSELF, LTEXT_PSFTITLE, MB_OK);
    return 0;
}
