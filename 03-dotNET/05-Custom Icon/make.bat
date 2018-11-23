REM Compile
csc.exe /t:module CustomIconWindow.cs 

REM Link
al.exe /t:winexe /out:CustomIconWindow.exe /win32icon:earth.ico /main:CustomIconWindow.Main CustomIconWindow.netmodule

REM Execute
CustomIconWindow.exe