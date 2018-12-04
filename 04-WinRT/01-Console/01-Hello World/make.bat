REM Compile
cl.exe /c /EHsc /ZW WinRTApp.cpp

REM Link
link.exe WinRTApp.obj vccorlib.lib runtimeobject.lib /SUBSYSTEM:CONSOLE

REM Execute!
WinRTApp.exe
