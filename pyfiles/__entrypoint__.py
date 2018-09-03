#!/usr/bin/env python3
#-*- coding:utf8 -*-

# sample bot for pyCompact

from helper import cqapi, DummyBot, MsgFrom
import random

myBot = DummyBot()

myBot.name="复读机测试"
@myBot.on("privMsg")
def privMsgProc():
    if event.subType is MsgFrom.FRIEND:
        cqapi.sendpm(event.fromQQ, "你还没有更改默认逻辑，打开应用目录，修改__entrypoint__.py来完成你的机器人")
    return cqapi.EVENT_IGNORE

@myBot.on("grpMsg")
def grpMsgProc():
    if random.random() >0.9:
        cqapi.sendgm(event.fromGroup,event.msg[:random.randint(0,len(event.msg))])
    return cqapi.EVENT_IGNORE

__bot__ = myBot

cqapi.logger("im done")
