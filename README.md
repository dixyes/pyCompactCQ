# pyCompact

又一个CoolQ的python3插件

## 用法
### 最终用户

0. 下载酷Q，安装 ***32位*** Python3： [Python下载](https://www.python.org/downloads)
1. 将dll开发者打包的插件放入app目录
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

待补全

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
CQPDir | CPQ.exe所在目录 | 以“\”结尾， 可以使用内置宏 如`$(ProjectDir)..\CQP\`
DebugablePythonDir | 可以调试的python3x_d.dll/python3x.dll所在目录 | 最好使用绝对路径，需要转义，不以“\\”结尾，如`..\\Python-3.7.0\\PCbuild\\win32`
USE_PYD | "USE_PYD=L"$(DebugablePythonDir)"" | <-复制粘贴

* USE_PYD的逻辑参考 mian.c#L63

## 问题
### 已知问题

- 重载dll时Py_Finalize玄学崩溃

### 反馈与求助

直接在issue提出问题，若为bug，提供详细环境
## 授权

本软件由MIT许可证授权： [LICENSE](https://github.com/dixyes/pyCompactCQ/blob/master/LICENSE)