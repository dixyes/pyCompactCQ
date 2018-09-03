/*
* pyc - texts.h - FUCK GB!
* copyright 2017-2018 dixyes
* MIT License
*/

// set texts here:
// TEXT_LOADENTRYPOINT "加载入口点文件 "%s""
// TEXT_LOADENTRYPOINT_FAIL "加载入口点文件失败"
// TEXT_NOBOTGLOBALVAR "入口点未设置__bot__全局变量"
// LTEXT_CHOOSEPYTHON "选择要加载的Python DLL（32位，3.2-3.7）"
// TEXT_CHOSENPYTHON "选择了%ls"
// TEXT_REQUIRERELOAD "重载已请求"
// TEXT_RELOADDLL "开始roll"
// TEXT_PYTHONDLLLOADFAIL "无法加载python3x.dll，确保你已经安装了\32位python3，并添加于PATH环境变量"
// TEXT_MODULENOTLOAD "回调被调用，但\dll还未加载，可能是加载\dll还未完成或者失败"
// TEXT_PYINTNOTLOAD "回调被调用，但py解释其还未加载，可能是加载解释器还未完成或者失败"
// TEXT_BOTNOTLOAD "回调被调用，但__bot__全局变量不存在，可能是入口点有错误"

#define LTEXT_PSF L"\u672c\u8f6f\u4ef6\u90e8\u5206\u5305\u542bPython-3.7.0\u7684\u5934\u6587\u4ef6\u6e90\u7801\uff0c\u6388\u6743\u5982\u4e0b\n\
\n\
Copyright © 2001-2018 Python Software Foundation; All Rights Reserved\n\
PSF LICENSE AGREEMENT FOR PYTHON 3.7.0\n\
\n\
1. This LICENSE AGREEMENT is between the Python Software Foundation(\"PSF\"), and\n\
the Individual or Organization(\"Licensee\") accessing and otherwise using Python\n\
3.7.0 software in source or binary form and its associated documentation.\n\
\n\
2. Subject to the terms and conditions of this License Agreement, PSF hereby\n\
grants Licensee a nonexclusive, royalty - free, world - wide license to reproduce,\n\
analyze, test, perform and/or display publicly, prepare derivative works,\n\
distribute, and otherwise use Python 3.7.0 alone or in any derivative\n\
version, provided, however, that PSF's License Agreement and PSF's notice of\n\
copyright, i.e., \"Copyright © 2001-2018 Python Software Foundation; All Rights\n\
Reserved\" are retained in Python 3.7.0 alone or in any derivative version\n\
prepared by Licensee.\n\
\n\
3. In the event Licensee prepares a derivative work that is based on or\n\
incorporates Python 3.7.0 or any part thereof, and wants to make the\n\
derivative work available to others as provided herein, then Licensee hereby\n\
agrees to include in any such work a brief summary of the changes made to Python\n\
3.7.0.\n\
\n\
4. PSF is making Python 3.7.0 available to Licensee on an \"AS IS\" basis.\n\
PSF MAKES NO REPRESENTATIONS OR WARRANTIES, EXPRESS OR IMPLIED.BY WAY OF\n\
EXAMPLE, BUT NOT LIMITATION, PSF MAKES NO AND DISCLAIMS ANY REPRESENTATION OR\n\
WARRANTY OF MERCHANTABILITY OR FITNESS FOR ANY PARTICULAR PURPOSE OR THAT THE\n\
USE OF PYTHON 3.7.0 WILL NOT INFRINGE ANY THIRD PARTY RIGHTS.\n\
\n\
5. PSF SHALL NOT BE LIABLE TO LICENSEE OR ANY OTHER USERS OF PYTHON 3.7.0\n\
FOR ANY INCIDENTAL, SPECIAL, OR CONSEQUENTIAL DAMAGES OR LOSS AS A RESULT OF\n\
MODIFYING, DISTRIBUTING, OR OTHERWISE USING PYTHON 3.7.0, OR ANY DERIVATIVE\n\
THEREOF, EVEN IF ADVISED OF THE POSSIBILITY THEREOF.\n\
\n\
6. This License Agreement will automatically terminate upon a material breach of\n\
its terms and conditions.\n\
\n\
7. Nothing in this License Agreement shall be deemed to create any relationship\n\
of agency, partnership, or joint venture between PSF and Licensee.This License\n\
Agreement does not grant permission to use PSF trademarks or trade name in a\n\
trademark sense to endorse or promote products or services of Licensee, or any\n\
third party.\n\
\n\
8. By copying, installing or otherwise using Python 3.7.0, Licensee agrees\n\
to be bound by the terms and conditions of this License Agreement."
#define LTEXT_PSFTITLE L"PSF\u8bb8\u53ef\u8bc1"
#define LTEXT_MITSELF L"pyCompact for CoolQ - yet another CoolQ python3 plugin\n\
pyCompact for CoolQ - \u4e00\u4e2a\uff08\u8fa3\u9e21\uff09\u9177Q Python3\u63d2\u4ef6\n\
copyright 2017-2018 dixyes <me@dixy.es>\n\
\u672c\u8f6f\u4ef6\u91c7\u7528\u004d\u0049\u0054\u5f00\u6e90\u8bb8\u53ef\u8bc1\u6388\u6743\u3002"

// generated definations here:
// MERK
#define TEXT_LOADENTRYPOINT "\xbc\xd3\xd4\xd8\xc8\xeb\xbf\xda\xb5\xe3\xce\xc4\xbc\xfe \"%s\""
#define TEXT_LOADENTRYPOINT_FAIL "\xbc\xd3\xd4\xd8\xc8\xeb\xbf\xda\xb5\xe3\xce\xc4\xbc\xfe\xca\xa7\xb0\xdc"
#define TEXT_NOBOTGLOBALVAR "\xc8\xeb\xbf\xda\xb5\xe3\xce\xb4\xc9\xe8\xd6\xc3__bot__\xc8\xab\xbe\xd6\xb1\xe4\xc1\xbf"
#define LTEXT_CHOOSEPYTHON L"\xd1\xa1\xd4\xf1\xd2\xaa\xbc\xd3\xd4\xd8\xb5\xc4Python DLL\xa3\xa832\xce\xbb\xa3\xac3.2-3.7\xa3\xa9"
#define TEXT_CHOSENPYTHON "\xd1\xa1\xd4\xf1\xc1\xcb%ls"
#define TEXT_REQUIRERELOAD "\xd6\xd8\xd4\xd8\xd2\xd1\xc7\xeb\xc7\xf3"
#define TEXT_RELOADDLL "\xbf\xaa\xca\xbcroll"
#define TEXT_PYTHONDLLLOADFAIL "\xce\xde\xb7\xa8\xbc\xd3\xd4\xd8python3x.dll\xa3\xac\xc8\xb7\xb1\xa3\xc4\xe3\xd2\xd1\xbe\xad\xb0\xb2\xd7\xb0\xc1\xcb\\32\xce\xbbpython3\xa3\xac\xb2\xa2\xcc\xed\xbc\xd3\xd3\xdaPATH\xbb\xb7\xbe\xb3\xb1\xe4\xc1\xbf"
#define TEXT_MODULENOTLOAD "\xbb\xd8\xb5\xf7\xb1\xbb\xb5\xf7\xd3\xc3\xa3\xac\xb5\xab\\dll\xbb\xb9\xce\xb4\xbc\xd3\xd4\xd8\xa3\xac\xbf\xc9\xc4\xdc\xca\xc7\xbc\xd3\xd4\xd8\\dll\xbb\xb9\xce\xb4\xcd\xea\xb3\xc9\xbb\xf2\xd5\xdf\xca\xa7\xb0\xdc"
#define TEXT_PYINTNOTLOAD "\xbb\xd8\xb5\xf7\xb1\xbb\xb5\xf7\xd3\xc3\xa3\xac\xb5\xabpy\xbd\xe2\xca\xcd\xc6\xe4\xbb\xb9\xce\xb4\xbc\xd3\xd4\xd8\xa3\xac\xbf\xc9\xc4\xdc\xca\xc7\xbc\xd3\xd4\xd8\xbd\xe2\xca\xcd\xc6\xf7\xbb\xb9\xce\xb4\xcd\xea\xb3\xc9\xbb\xf2\xd5\xdf\xca\xa7\xb0\xdc"
#define TEXT_BOTNOTLOAD "\xbb\xd8\xb5\xf7\xb1\xbb\xb5\xf7\xd3\xc3\xa3\xac\xb5\xab__bot__\xc8\xab\xbe\xd6\xb1\xe4\xc1\xbf\xb2\xbb\xb4\xe6\xd4\xda\xa3\xac\xbf\xc9\xc4\xdc\xca\xc7\xc8\xeb\xbf\xda\xb5\xe3\xd3\xd0\xb4\xed\xce\xf3"
// ENDMERK