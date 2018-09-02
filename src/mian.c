/*
* pyc Python Compact Library - main code file
* copyright 2017-2018 dixyes
* MIT License
*/
#include <stdbool.h>
#include <windows.h>

//#include "Python.h"
#include "cqp.h"

#include "mian.h"
#include "texts.h"
#include "util.h"
#include "py.h"

bool pluginIsEnabled = false;
int pyStatus = PY_NOT_INIT;

/*
* return appinfo as "apiversion,packagename" like "9,com.example.app"
*/
CQEVENT(const char*, AppInfo, 0)() {
	return CQAPPINFO;
}

/* 
* receive authcode only DONOT DO ANYTHING ELSE
*/
CQEVENT(int32_t, Initialize, 4)(int32_t AuthCode) {
	ac = AuthCode;
	return 0;
}

/*
* called when startup cq
*/
CQEVENT(int32_t, __eventStartup, 0)() {
    // not logable, do some really quiet things
    util_init();
    //pyStatus = py_load();
	return 0;
}

/*
* when stop cq
*/
CQEVENT(int32_t, __eventExit, 0)() {
    util_free();
	return 0;
}

#define MIN_PY_VER 30
#define MAX_PY_VER 37
/*
* this plugin enabled
*/
CQEVENT(int32_t, __eventEnable, 0)() {
    // allco pyName buffer first
    wchar_t * pyName = malloc(MAX_PATH+1 * sizeof(wchar_t));
    for (int pyVer = MIN_PY_VER; pyVer <= MAX_PY_VER; pyVer++) {
#ifdef _DEBUG
#   ifdef USE_PYD
        wsprintfW(pyName, USE_PYD L"\\"  L"python%d_d.dll", pyVer);
#   else
        wsprintfW(pyName, L"python%d_d.dll", pyVer);
#   endif 
#else
        wsprintfW(pyName, L"python%d.dll", pyVer);
#endif
        logd("pyLoad", "loading %ls", pyName);
        pyStatus = py_load(pyName);
        if (0 == pyStatus) {
            break;
        }
    }
    
    if(0==pyStatus){
        int ret = py_init(ac);
        logd("py_init","ret = %d", ret);
        if (0!=ret){
            loge("py_init","python init failed");
            return -1;
        }
        pluginIsEnabled = true;
    }else{
        loge("py_load", "%s", TEXT_PYTHONDLLLOADFAIL);// todo: 写个多语言支持（或许吧
    }
    
	
	return 0;
}

/*
* disabled
*/
CQEVENT(int32_t, __eventDisable, 0)() {
	pluginIsEnabled = false;
	return 0;
}
