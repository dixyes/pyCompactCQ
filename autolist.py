#!/usr/bin/env python3
#-*- coding:utf-8 -*-

import shutil,re,sys

MARK = r"([ \t]*)/\*[ \t]*AUTOLISTMARK[ \t]*\*/"
ENDMARK = r"[ \t]*/\*[ \t]*ENDAUTOLISTMARK[ \t]*\*/"
MAKR = r"[ \t]*/\*[ \t]*MAKR[ \t]*\*/" # yep that's not typo, neither mian 
ENDMAKR = r"[ \t]*/\*[ \t]*ENDMAKR[ \t]*\*/"
TMPL = "{0[0]} = ({0[1]})GetProcAddress(py_module, \"{0[0]}\");\n    _checkPyFunction({0[0]});\n"

reFunction = re.compile(r"([a-zA-Z0-9_* ]*\()[ \t]*\*[ \t]*([a-zA-z_][a-zA-z0-9_]*)[ \t]*(\)[a-zA-Z0-9_* (),.\[\]]*);")
reVariable = re.compile(r"([a-zA-Z0-9_* ]*)[ \t]+([a-zA-z0-9_]+)[ \t]*[ \t]*;")

def extractIdentifier(x):
    ret = reFunction.search(x)
    if ret:
        return (ret.groups()[1], ret.groups()[0]+"__cdecl *"+ret.groups()[2])
    else:
        ret = reVariable.search(x)
        if not ret:
            raise Exception("not good partten:",x)
        #print((ret.groups()[1], ret.groups()[0]))
        return (ret.groups()[1], ret.groups()[0])
    raise Exception("not good partten:",x)

def mian():
    shutil.copy(sys.argv[1],sys.argv[1]+".bak")
    try:
        reMakr = re.compile(MAKR)
        reEndmakr = re.compile(ENDMAKR)
        reMark = re.compile(MARK)
        reEndmark = re.compile(ENDMARK)
        with open(sys.argv[2],"r", encoding="utf-8-sig") as pys:
            pysText = pys.readlines()
            pysStart = pysText.index([x for x in pysText if reMakr.match(x)][0])
            pysEnd = pysText.index([x for x in pysText if reEndmakr.match(x)][0])
            insList = pysText[pysStart+1:pysEnd]
            idsList = sorted([TMPL.format(extractIdentifier(x)) for x in insList])
            #print(insList,idsList)
            #exit(0)
            with open(sys.argv[1], encoding="utf-8-sig") as pyc:
                pycText = pyc.readlines()
                pycStart = pycText.index([x for x in pycText if reMark.match(x)][0])
                pycEnd = pycText.index([x for x in pycText if reEndmark.match(x)][0])
                indent = [reMark.match(x) for x in pycText if reMark.match(x)][0].groups()[0]
                #print("i is",indent,"end")
                pycNew = pycText[0:pycStart+1] + [(indent + x) for x in idsList] + pycText[pycEnd:]
                pyc.seek(0)
                #print("".join(pycNew),pyc.read())
                if "".join(pycNew).replace("\r\n","\n") == pyc.read().replace("\r\n","\n"):
                    print("autolist.py: not changing")
                    exit(0)
            with open(sys.argv[1],"w", encoding="utf-8-sig") as pyc:
                pyc.write("".join(pycNew))
    except Exception as e:
        print("fucked up, restore backup")
        shutil.copy(sys.argv[1]+".bak", sys.argv[1])
        raise e

if "__main__" == __name__:
    if not len(sys.argv)>2:
        print("usage:\r\n\t(python3) autolist.py [target C file] [source header]")
        exit(-1)
    mian()