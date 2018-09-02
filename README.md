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

编写你的入口点文件__entrypoint__.py：

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
### Clang/gcc直接编译 不使用ide/gnumake之类的

1. 安装clang/gcc(gcc 在win怎么安来着), 将build.bat里面`set CC=clang`改成你的编译器名称

2. 在目录运行build.bat

### visual studio

(vs分支,待完成)

## 问题
### 已知问题

- 重载dll时Py_Finalize玄学崩溃

### 反馈与求助

直接在issue提出问题，若为bug，提供详细环境
## 授权

本软件由MIT许可证授权： [LICENSE](https://github.com/dixyes/pyCompactCQ/blob/master/LICENSE)