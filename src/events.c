/*
* pyc Python Compact Library - events callback python scripts can register
* copyright 2017-2018 dixyes
* MIT License
*/

#include <stdint.h>

#include "cqp.h"
#include "mian.h"
#include "py.h"
#include "util.h"

CQEVENT(int32_t, __eventPrivateMsg, 24)(int32_t subType, int32_t msgId, int64_t fromQQ, const char *msg, int32_t font) {
    return py_callCallback("__eventPrivateMsg", "iiLyi", subType, msgId, fromQQ, msg, font);
}

CQEVENT(int32_t, __eventGroupMsg, 36)(int32_t subType, int32_t msgId, int64_t fromGroup, int64_t fromQQ, const char *fromAnonymous, const char *msg, int32_t font) {
    return py_callCallback("__eventGroupMsg", "iiLLyyi", subType, msgId, fromGroup, fromQQ, fromAnonymous, msg, font);
}
