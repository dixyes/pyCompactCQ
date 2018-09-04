#!/usr/bin/env python3
#-*- coding:utf-8 -*-

# pyCompact helper
# copyright 2017-2018 Yun Dou (dixyes) <me@dixy.es>
# MIT License

__author__="dixyes"
__copyright__="Copyright 2017-2018, dixyes"
__license__="MIT"
__version__="0.0.1"
__email__="me@dixy.es"

import inspect,ntpath,sys,re

caller = lambda : "{0[1]}:{0[2]}".format(inspect.getouterframes(inspect.currentframe(),0)[2])

# import cqapi or pesudoCqapi for commandline debug
class pseudoCqapi():
    '''
    pseudo cqapi module for console debug
    '''
    __author__="dixyes"
    __copyright__="Copyright 2017-2018, dixyes"
    __license__="MIT"
    __version__="0.0.2"
    __email__="me@dixy.es"
    EVENT_IGNORE=1	# 事件_忽略
    EVENT_BLOCK=2	# 事件_拦截
    REQUEST_ALLOW=3	# 请求_通过
    REQUEST_DENY=4	# 请求_拒绝
    REQUEST_GROUPADD=5	# 请求_群添加
    REQUEST_GROUPINVITE=6	# 请求_群邀请
    LOGGER_DEBUG=1	# 调试 灰色
    LOGGER_INFO=2	# 信息 黑色
    LOGGER_INFOSUCCESS=3	# 信息(成功) 紫色
    LOGGER_INFORECV=4	# 信息(接收) 蓝色
    LOGGER_INFOSEND=5	# 信息(发送) 绿色
    LOGGER_WARNING=6	# 警告 橙色
    LOGGER_ERROR=7	# 错误 红色
    LOGGER_FATAL=8	# 致命错误 深红
    appdir=""
    selfqq = 10000
    def logger(self,obj,type=LOGGER_DEBUG,tag="pseudoPyLogger"):
        print(caller(),tag,obj.__repr__())
    def sendpm(self,qqid,msg):
        print(caller(), "sending priv msg '%s' to %d"%(msg,qqid))
    def sendgm(self,qqid,msg):
        print(caller(), "sending group msg '%s' to %d"%(msg,qqid))
    def senddm(self,qqid,msg):
        print(caller(), "sending discuss msg '%s' to %d"%(msg,qqid))
    def delmsg(self,msgid):
        print(caller(), "deleting Message %d"%(msgid))
try:
    cqapi = __import__("cqapi")
except ImportError as e:
    cqapi = pseudoCqapi()
    cqapi.log("using psudoCqapi")
    raise e
if not cqapi.__version__ == __version__:
    cqapi.log("not matched api version, update you helper.py!", type=cqapi.LOGGER_WARNING, tag="develpoerWaring")
try:
    from enum import Enum
except ImportError:
    if sys.version_info[1] < 4:
        cqapi.logger("python is older then 3.4, install enum34 package!", type=cqapi.LOGGER_WARNING, tag="develpoerWaring")
    Enum = Object # dirty patch here

class MsgFrom(Enum):
    FRIEND = 11 # 来自好友
    ONLINESTATE = 1 # 来自在线状态
    GROUP =2 # 来自群
    DISCUSS =3 # 来自讨论组

def fuckGB(bytesMsg:bytes):
    """you can also fuck GB18030 by this utility"""
    return bytesMsg.decode("GB18030")
def defuckGB(strMsg:str):
    """you can also defuck GB18030 by this utility"""
    return strMsg.encode("GB18030")

class classproperty(object):
    def __init__(self, getter):
        self.getter= getter
    def __get__(self, instance, owner):
        return self.getter(owner)
    
class Event():
    known_events = {
    "privMsg":"__eventPrivateMsg",# v is string meaning it's alias
    "PrivateMsg":"__eventPrivateMsg",
    "__eventPrivateMsg":[("subType",MsgFrom), "msgId", "fromQQ", ("msg", fuckGB), "font"],# [valueName,or (valueName, converter),...]
    "grpMsg":"__eventGroupMsg",
    "__eventGroupMsg":[("subType",MsgFrom), "msgId", "fromGroup", "fromQQ", ("fromAnonymous", fuckGB), ("msg", fuckGB), "font"],
    }
    @classproperty
    def event_alias(cls):
        if hasattr(cls, "_event_alias"):
            return cls._event_alias
        cls._event_alias = {}
        for k in cls.known_events.keys():
            if isinstance(cls.known_events[k], str):
                if not cls._event_alias.get(cls.known_events[k]):
                    cls._event_alias[cls.known_events[k]] = []
                cls._event_alias[cls.known_events[k]].append(k) 
        return cls._event_alias
    def __init__(self, ev = (), name="BaseEvent"):
        self._event = ev
        self._name = name
     
class DummyBot():
    """
    BaseBot class
    """
    name = "BaseBot"
    callbacks={}

    def __init__(self,name=None):
        self.name = name or self.name
    @staticmethod
    def recvEvent(eventName:str, event:tuple):
        """
        parse event according to known_events dict
        """
        ret = Event(event, eventName)
        if not eventName in Event.known_events: # not supported
            return ret
        if isinstance(Event.known_events[eventName],str): # this an alias
            eventName = Event.known_events[eventName]
        argFormats = Event.known_events.get(eventName,[])
        if not len(event) == len(argFormats):
            cqapi.logger('%s: different length between event and format' % caller(), tag="developerWaring", type=cqapi.LOGGER_WARNING)
        for i in range(min(len(argFormats),len(event))):
            #print(i,argFormats[i],event[i])
            if isinstance(argFormats[i], str):
                setattr(ret, argFormats[i], event[i])
            else:
                setattr(ret, argFormats[i][0], argFormats[i][1](event[i]))
        #print(ret)
        return ret
    def on(self, eventName, defaultReturn = None):
        def _on(func):
            if not eventName in Event.known_events.keys():
                cqapi.logger('%s: unknown event name %s' % (caller(), eventName), tag="developerWaring", type=cqapi.LOGGER_WARNING)
            def wrapper(event):
                g = func.__globals__
                eventDict = DummyBot.recvEvent(eventName,event)
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
        if eventName in Event.event_alias.keys():
            for alias in Event.event_alias[eventName]:
                print(alias,eventName)
                self.emit(alias, event)
        return cqapi.EVENT_IGNORE
      
# from https://d.cqp.me/Pro/CQ%E7%A0%81
class CQCode():
    #class attr/methods
    _groupRe = None
    @classproperty
    def groupRe(cls):
        return cls._groupRe or re.compile(r"\[CQ:(?P<name>face|emoji|bface|sface|image|record|at|rps|dice|shake|anonymous|music|share)(?P<args>[^]]*)\]")
    _argsRe = None
    @classproperty
    def argsRe(cls):
        return cls._groupRe or re.compile(r",(?P<key>[a-z]+)=(?P<value>[^],]+) *")
    @classmethod
    def parse(cls, s:str):
        ret = []
        for cq in cls.groupRe.findall(s):
            kwargs = {}
            for k,v in cls.argsRe.findall(cq[1]):
                kwargs[k] = v
            ret.append(CQCode(cq[0],**kwargs))
        return ret
    @staticmethod
    def isAt(s:str, qq:int):
        for cqc in CQCode.parse(s):
            if cqc.type == "at" and cqc.kvs["qq"] == str(qq):
                return True
        return False
    @staticmethod
    def isAtMe(s:str):
        return CQCode.isAt(s, cqapi.selfqq)
    @classmethod
    def at(cls, qqid:int):
        return cls("at",qq=str(qqid))
    # instance attr/methods
    def __init__(self, type, **args):
        self.type = type
        self.kvs = args
        #print(self.kvs)
        #print(self)
    def __str__(self):
        return "[CQ:%s%s]" % (self.type, "".join([",%s=%s" % (k,v) for k,v in self.kvs.items()]))
    def __repr__(self):
        return "<helper.CQCode, type = \"%s\"%s>" % (self.type, "".join([",%s = \"%s\"" % (k,v) for k,v in self.kvs.items()]))
# TODO: multiple bot support
def addBot(bot):
    """
    add bot.
    """
    raise NotImplementedError
        
if "__main__" == __name__:
    #recvEvent('__eventGroupMsg', (1, 469, 312123423, 123432231, b'', b'\xd0\xbb\xd0\xbb\xd0\xbb\xd0\xbb', 87760336))
    pass