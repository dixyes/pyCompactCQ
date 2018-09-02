#!/usr/bin/env python3
#-*- coding:utf8 -*-

# pyCompact helper
# copyright 2017-2018 Yun Dou (dixyes) <me@dixy.es>
# MIT License

__author__="dixyes"
__copyright__="Copyright 2017-2018, dixyes"
__license__="MIT"
__version__="0.0.1"
__email__="me@dixy.es"

import inspect,ntpath,sys
from enum import Enum

caller = lambda : "{0[1]}:{0[2]}".format(inspect.getouterframes(inspect.currentframe(),0)[2])

# import cqapi
class pseudoCqapi():
    '''
    pseudo cqapi module for console debug
    '''
    __author__="dixyes"
    __copyright__="Copyright 2017-2018, dixyes"
    __license__="MIT"
    __version__="0.0.1"
    __email__="me@dixy.es"
    EVENT_IGNORE=1	# 事件_忽略
    EVENT_BLOCK=2	# 事件_拦截
    REQUEST_ALLOW=3	# 请求_通过
    REQUEST_DENY=4	# 请求_拒绝
    REQUEST_GROUPADD=5	# 请求_群添加
    REQUEST_GROUPINVITE=6	# 请求_群邀请
    CQLOG_DEBUG=1	# 调试 灰色
    CQLOG_INFO=2	# 信息 黑色
    CQLOG_INFOSUCCESS=3	# 信息(成功) 紫色
    CQLOG_INFORECV=4	# 信息(接收) 蓝色
    CQLOG_INFOSEND=5	# 信息(发送) 绿色
    CQLOG_WARNING=6	# 警告 橙色
    CQLOG_ERROR=7	# 错误 红色
    CQLOG_FATAL=8	# 致命错误 深红
    appdir=""
    def logger(self,obj,type=CQLOG_DEBUG,tag="pseudoPyLogger"):
        print(caller(),tag,obj.__repr__())
    def sendpm(self,qqid,msg):
        print("sending priv msg '%s' to %d",msg,qqid)
    def sendgm(self,qqid,msg):
        print("sending group msg '%s' to %d",msg,qqid)
    def senddm(self,qqid,msg):
        print("sending discuss msg '%s' to %d",msg,qqid)
    def delmsg(self,msgid):
        print("sending '%s' to %d",msg,qqid)
try:
    cqapi = __import__("cqapi")
except:
    cqapi = pseudoCqapi()
    pass
if not cqapi.__version__ == __version__:
    cqapi.log("not matched api version, update you helper.py!", type=cqapi.CQLOG_WARNING, tag="develpoerWaring")

class MsgFrom(Enum):
    FRIEND = 11 # 来自好友
    ONLINESTATE = 1 # 来自在线状态
    GROUP =2 # 来自群
    DISCUSS =3 # 来自讨论组

def fuckGB(bytesMsg):
    return bytesMsg.decode("GB18030")

KNOWN_EVENTS = {
    "privMsg":"__eventPrivateMsg",# v is string meaning it's alias
    "PrivateMsg":"__eventPrivateMsg",
    "__eventPrivateMsg":[("subType",MsgFrom), "msgId", "fromQQ", ("msg", fuckGB), "font"],# [valueName,or (valueName, converter),...]
    "grpMsg":"__eventGroupMsg",
    "__eventGroupMsg":[("subType",MsgFrom), "msgId", "fromGroup", "fromQQ", ("fromAnonymous", fuckGB), ("msg", fuckGB), "font"],
    }
_EVENTS_ALIAS = {}
for k in KNOWN_EVENTS.keys():
    if isinstance(KNOWN_EVENTS[k], str):
        if not _EVENTS_ALIAS.get(KNOWN_EVENTS[k]):
            _EVENTS_ALIAS[KNOWN_EVENTS[k]] = []
        _EVENTS_ALIAS[KNOWN_EVENTS[k]].append(k)
print(_EVENTS_ALIAS)

class Event():
    def __init__(self, ev = (), name="BaseEvent"):
        self._event = ev
        self._name = name

def recvEvent(eventName:str, event:tuple):
    """
    parse event according to KNOWN_EVENTS dict
    """
    ret = Event(event, eventName)
    if not eventName in KNOWN_EVENTS: # not supported
        return ret
    if isinstance(KNOWN_EVENTS[eventName],str): # this an alias
        eventName = KNOWN_EVENTS[eventName]
    argFormats = KNOWN_EVENTS.get(eventName,[])
    if not len(event) == len(argFormats):
        cqapi.logger('%s: different length between event and format' % caller(), tag="developerWaring", type=cqapi.CQLOG_WARNING)
    for i in range(min(len(argFormats),len(event))):
        #print(i,argFormats[i],event[i])
        if isinstance(argFormats[i], str):
            setattr(ret, argFormats[i], event[i])
        else:
            setattr(ret, argFormats[i][0], argFormats[i][1](event[i]))
    #print(ret)
    return ret
     
class DummyBot():
    """
    BaseBot class
    """
    name = "BaseBot"
    callbacks={}

    def __init__(self,name=None):
        self.name = name or self.name
    def on(self, eventName, defaultReturn = None):
        def _on(func):
            if not eventName in KNOWN_EVENTS.keys():
                cqapi.logger('%s: unknown event name %s' % (caller(), eventName), tag="developerWaring", type=cqapi.CQLOG_WARNING)
            def wrapper(event):
                g = func.__globals__
                eventDict = recvEvent(eventName,event)
                oldValue = g.get("event", eventDict)
                g["event"] = eventDict
                try:
                    ret = func()
                finally:
                    if oldValue is eventDict:
                        del g['event']
                    else:
                        g['event'] = oldValue
                return ret
            if not self.callbacks.get(eventName):
                self.callbacks[eventName] = []
            self.callbacks[eventName].append(wrapper)
            return wrapper
        return _on
    #
    # PYCQAPI_VERSION 当前行为
    #           0.0.1 bot对象必须实现该方法：
    #                 emit(self:Bot, eventName:str, event:tuple)
    #                 输入：self:bot对象， eventName：事件名称，event：时间数据
    def emit(self, eventName, event):
        #cqapi.logger(cbs)
        for cb in self.callbacks.get(eventName,[]):
            ret = cb(event)
            if ret and ret == cqapi.EVENT_BLOCK:
                return ret
        if eventName in _EVENTS_ALIAS.keys():
            for alias in _EVENTS_ALIAS[eventName]:
                print(alias,eventName)
                self.emit(alias, event)
        return cqapi.EVENT_IGNORE

# TODO: multiple bot support          
def addBot(bot):
    """
    add bot.
    """
    raise NotImplementedError
        
if "__main__" == __name__:
    #recvEvent('__eventGroupMsg', (1, 469, 312123423, 123432231, b'', b'\xd0\xbb\xd0\xbb\xd0\xbb\xd0\xbb', 87760336))
    pass