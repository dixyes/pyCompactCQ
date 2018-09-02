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

/*
* this plugin enabled
*/
CQEVENT(int32_t, __eventEnable, 0)() {
    pyStatus = py_load(L"C:\\Users\\DIXXPS\\Desktop\\pycompact\\Python-3.7.0\\PCbuild\\win32\\python37_d.dll");
    logd("py_load","ret = %d", pyStatus);
    if(0==pyStatus){
        int ret = py_init(ac);
        logd("py_init","ret = %d", ret);
        if (0!=ret){
            loge("py_init","python init failed");
            return -1;
        }
        pluginIsEnabled = true;
    }else{
         //when py not found, go fuck yourself
        loge("py_load","cannot load python3x.dll, make sure you have 32bit python3 installed and added to PATH");// todo: 写个多语言支持（或许吧
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
