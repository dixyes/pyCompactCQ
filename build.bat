@echo off

rem set gcc is also fine
set CC=clang

set PACKAGE_NAME=cn.dixyes.pyc
set OUTPUT_NAME=%PACKAGE_NAME%.dll
set ROOTDIR=%cd%
set SRCDIR=%ROOTDIR%\src
set PROPLIBDIR=%ROOTDIR%\proplib
set BUILDDIR=%ROOTDIR%\build

set CCARGS= -I%PROPLIBDIR% -Wno-visibility -Wno-unused-command-line-argument -m32 -D_DEBUG -Werror=implicit-function-declaration -std=gnu17 -g
set LDARGS= -m32

python autolist.py %SRCDIR%\py.c %SRCDIR%\pys.h || echo "autogenlist failed" && goto end
python fuckgb.py %SRCDIR%\texts.h || echo "fuck GB failed" && goto end
python mkjson.py %BUILDDIR%\%PACKAGE_NAME%.json Debug || echo "makeJson failed" && goto end

cd %SRCDIR%
for %%f in (*.c) do (
    rem echo del /s %BUILDDIR%\%%~nf.o
    del /q %BUILDDIR%\%%~nf.o
    rem echo %CC% %CCARGS% -c %%f -o %BUILDDIR%\%%~nf.o
    %CC% %CCARGS% -c %%f -o %BUILDDIR%\%%~nf.o || break
    echo %%~nf.o
)
cd %ROOTDIR%

%CC% %PROPLIBDIR%\CQP.lib %BUILDDIR%\*.o -lcomdlg32.lib -lUser32.lib %LDARGS% -shared -o %BUILDDIR%\%OUTPUT_NAME% || goto end

echo build done(Debug target)

:end

pause
