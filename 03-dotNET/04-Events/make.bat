REM Compile
csc.exe /t:module EventWindow.cs 

REM Link
al.exe /t:winexe /out:EventWindow.exe /main:EventWindow.Main EventWindow.netmodule

REM Execute
EventWindow.exe