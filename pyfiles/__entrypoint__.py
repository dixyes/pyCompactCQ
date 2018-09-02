#!/usr/bin/env python3
#-*- coding:utf8 -*-

from helper import cqapi, DummyBot, MsgFrom
import random

myBot = DummyBot()

def spamScan(msg):
    #check with aliapi, tencent api, baidu and so on
    return msg.startswith("这是一条广告")

myBot.name="复读机测试"
@myBot.on("privMsg")
def privMsgProc():
    if event.subType is MsgFrom.FRIEND:
        if random.random() >0.9:
            cqapi.sendpm(event.fromQQ,"真香")
        else:
            cqapi.sendpm(event.fromQQ,"我就这一句话,就算是死,从这跳下去,我也不会说别的话")
    return cqapi.EVENT_IGNORE

@myBot.on("grpMsg")
def grpMsgProc():
    if random.random() >0.9:
        cqapi.sendgm(event.fromGroup,event.msg[:random.randint(0,len(event.msg))])
    elif spamScan(event.msg):
        cqapi.delmsg(event.msgId)
    return cqapi.EVENT_IGNORE

__bot__ = myBot

cqapi.logger("im done")
