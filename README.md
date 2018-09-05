# pyCompact

又一个CoolQ的python3插件

> Life is short, use Python

## 用法
### 最终用户

0. 下载酷Q，安装 ***32位*** Python3： [Python下载](https://www.python.org/downloads)
1. 将dll开发者打包的插件[下载](https://github.com/dixyes/pyCompactCQ/releases)放入app目录
2. 以1中包名为名称在app目录新建文件夹
3. 将py开发者发布的脚本（一定含有__entrypoint__.py，可能有helper.py）放入2中的文件夹
4. 启动登陆酷Q

### py开发者

编写你的入口点文件__entrypoint__.py

可以参考pyfiles文件夹中的范例机器人和helper，helper会随C程序更新

你需要实现

```Python3
class Bot():
    def emit(self, eventName:str, event:tuple):
        return cqapi.EVENT_IGNORE # 或者 cqapi.EVENT_BLOCK
```
如果你的脚本需要额外的库，提醒用户使用`%PATH_TO_USED_PY%\python.exe -m pip install xxx`安装

## python API文档

### cqapi封装（内建模块cqapi）
版本0.0.2

```Python3
def cqapi(module):
    """
    CoolQ APIs
    """
    __version__ = "0.0.2"
    EVENT_IGNORE = EVENT_IGNORE # 事件_忽略
    EVENT_BLOCK = EVENT_BLOCK # 事件_拦截
    REQUEST_ALLOW = REQUEST_ALLOW # 请求_通过
    REQUEST_DENY = REQUEST_DENY # 请求_拒绝
    REQUEST_GROUPADD = REQUEST_GROUPADD # 请求_群添加
    REQUEST_GROUPINVITE = REQUEST_GROUPINVITE # 请求_群邀请
    
    LOGGER_DEBUG = 1 # 调试 灰色
    LOGGER_INFO = 2 # 信息 黑色
    LOGGER_INFOSUCCESS = 3 # 信息(成功) 紫色
    LOGGER_INFORECV = 4 # 信息(接收) 蓝色
    LOGGER_INFOSEND = 5 # 信息(发送) 绿色
    LOGGER_WARNING = 6 # 警告 橙色
    LOGGER_ERROR = 7 # 错误 红色
    LOGGER_FATAL = 8 # 致命错误 深红
    
    selfqq = 10000 # 机器人q号，整型常量
    appdir = "" # CQ的appDirectionary，字符串常量
    nickname = "" # 昵称 - 字符串常量 （此api将会使用getter和setter替代）
    
    def logger(msgObj:Object, type=cqapi.LOGGER_INFO, tag="pyLogger"):
        return None
    def sendpm(toQQ:int, msg:unicode):
        return int(msgId)
    def sendgm(toGroup:int, msg:unicode):
        return int(msgId)
    def senddm(toDiscussion:int, msg:unicode):
        return int(msgId)
    def delmsg(msgId:int):
        return int(unknown_from_CQAPIs)
    def sendlike(id:int):
        return int(unknown_from_CQAPIs)
    def groupkick(groupId:int, QQID:int, rejectaddrequest = False):
        return int(unknown_from_CQAPIs)
    def groupban(groupId:int, QQID:int, duration = 0):
        return int(unknown_from_CQAPIs)
```

### 事件

装饰器工作依赖于DummyBot的on方法，实现详见helper.py

使用`@someBot_inherit_from_DummyBot.on("someEvent")`来修饰对应回调，则设置环境变量event

事件 | 对应回调名称 | 简称 | 所设置环境变量 |
---- | ------------ | ---- | -------------- |
收到私信 | __eventPrivateMsg | privMsg | event.subType:MsgFrom:消息来源类型，参见helper中的MsgFrom枚举类, event.msgId:int:消息ID, event.fromQQ:int:来源QQ, event.msg:unicode:消息内容, event.font:int:字体类型 |
收到群信息 | __eventGroupMsg | grpMsg | event.subType:MsgFrom:消息来源类型, event.msgId:int:消息ID, event.fromGroup:int来源Q群, event.fromQQ:int:来源QQ, event.fromAnonymous:unicode:来源匿名, event.msg:unicode消息内容, event.font:int字体类型 |

### 用例

使用helper构建简单机器人：

```Python3
from helper import DummyBot, cqapi, CQCode ,MsgFrom
myBot = DummyBot()
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
        cqapi.sendgm(event.fromGroup,event.msg)
    if event.fromQQ == badGuyQQID:
        cqapi.groupkick(event.fromGroup, badGuyQQID)
    if random.random() >0.9:
        cqapi.sendgm(event.fromGroup,"恭喜%s喜提禁言1分钟" % CQCode.at(event.fromQQ))
        cqapi.groupban(event.fromGroup, event.fromQQ ,1)
    return cqapi.EVENT_IGNORE

__bot__ = myBot
```

### 关于helper

helper.py是一个简单的助手模块，其中有许多助手函数如CQCode.at(qq:int)可以参照其源码使用

由于更新独立于c部分且较快，先不写文档了 ~（其实就是懒~

## dll开发文档

待补全

## 构建

首先获取源码：
    
    git clone <本仓库地址> 
    git submodule update proplib/cqsdk-vc # 更新sdk依赖

### Clang/gcc直接编译 不使用ide/gnumake之类的

1. 安装clang/gcc(gcc 在win怎么安来着), 将build.bat里面`set CC=clang`改成你的编译器名称

2. 在目录运行build.bat

### visual studio

打开pyCompactCQ.sln，然后构建（或者msbuild一样的

如果要调试：在左侧“属性管理器”双击Microsoft.Cpp.Win32.user,在左侧选择“用户宏”,在右侧添加以下变量(参考pyCompactCQ.vcxproj.user.example)

宏名称 | 值 | 说明 |
------ | -- | ---- |
CQPDir | CQP.exe所在目录 | 以“\”结尾， 可以使用内置宏 如`$(ProjectDir)..\CQP\`
DebugablePythonDir | 可以调试的python3x_d.dll/python3x.dll所在目录 | 最好使用绝对路径，需要转义，不以“\\”结尾，如`..\\Python-3.7.0\\PCbuild\\win32`
USE_PYD | "USE_PYD=L"$(DebugablePythonDir)"" | <-复制粘贴

* USE_PYD的逻辑参考 mian.c#L63

## 问题
### 已知问题

- ~重载dll时Py_Finalize玄学崩溃~ （使用python3x.dll的Release构建中）重载功能已移除，只能加载一次dll/脚本（由于Py_FinalizeEx的 [BUG](https://docs.python.org/3/c-api/init.html#c.Py_FinalizeEx)，py解释器在python3x.dll下只能finalize一次）

### 反馈与求助

直接在issue提出问题，若为bug，提供详细环境

## 授权

本软件包含部分python3.7头文件的源代码

本软件由MIT许可证授权： [LICENSE](https://github.com/dixyes/pyCompactCQ/blob/master/LICENSE)