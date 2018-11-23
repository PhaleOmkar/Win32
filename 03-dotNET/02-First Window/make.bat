REM Compile
csc.exe /t:module CSharpWindow.cs 

REM Link
al.exe /t:winexe /out:CSharpWindow.exe /main:CSharpWindow.Main CSharpWindow.netmodule

REM Execute
CSharpWindow.exe