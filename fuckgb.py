#!/usr/bin/env python3
# -*- coding: utf-8 -*-

import sys,re,shutil,codecs

COMMENT_REGEX = r"//[ \t]*(TEXT_[A-Za-z0-9_-]+)[ \t]+\"(.+)\""
COMMENTL_REGEX = r"//[ \t]*(LTEXT_[A-Za-z0-9_-]+)[ \t]+\"(.+)\""
MKR_REGEX = "//[ \t]*MERK.*"
ENDMKR_REGEX = "//[ \t]*ENDMERK.*"
TMPL = "#define %s \"%s\"\n"
LTMPL = "#define %s L\"%s\"\n"
FILE_TMPL = "\1// MERK\r\n%s// ENDMERK\n"

definations = []

def encodeGB(x):
    return codecs.escape_encode(x.encode("GB18030"))[0].decode("ascii").replace("\"","\\\"")
    #for b in x.encode("GB18030"):
        

if "__main__" == __name__:
    if not len(sys.argv)>1:
        print("usage:\r\n\t(python3) fuckgb.py [target texts.h]")
        exit(-1)
    shutil.copy(sys.argv[1],sys.argv[1]+".bak")
    try:
        cmtRegex = re.compile(COMMENT_REGEX)
        cmtlRegex = re.compile(COMMENTL_REGEX)
        mkrRegex = re.compile(MKR_REGEX)
        endmkrRegex = re.compile(ENDMKR_REGEX)
        with open(sys.argv[1], encoding="utf-8-sig") as fgbh:
            fl = fgbh.readlines()
            for line in fl:
                match = cmtRegex.match(line)
                if match:
                    grps = match.groups()
                    #print(TMPL % (grps))
                    definations.append( TMPL % (grps[0], encodeGB(grps[1])))
                    continue
                matchl = cmtlRegex.match(line)
                if matchl:
                    grps = matchl.groups()
                    #print(TMPL % (grps))
                    definations.append( LTMPL % (grps[0], encodeGB(grps[1])))
            for line in fl:
                if(mkrRegex.match(line)):
                    sIndex = fl.index(line)
                    break
            for line in fl:
                if(endmkrRegex.match(line)):
                    eIndex = fl.index(line)
                    break
            newFile = fl[:sIndex+1] + definations + fl[eIndex:]
            newFile = "".join(newFile)
            #print(newFile)
            #print(newFile.replace("\r\n","\n").replace("\r\n\r\n","\r\n"),"---xxx---\r\n","".join(fl).replace("\r\n","\n"))
            if newFile.replace("\r\n","\n") == "".join(fl).replace("\r\n","\n"):
                print("fuckGB.py: not changing")
                exit()
        with open(sys.argv[1], "w", encoding="utf-8-sig") as fgbh:
            fgbh.write(newFile)
    except Exception as e:
        print("fucked up, restore backup")
        shutil.copy(sys.argv[1]+".bak", sys.argv[1])
        raise e
                