CLS

REM Register the dll
ClientOfContainmentComponentWithRefFile.reg

REM Compile
cl.exe /c /EHsc ClientOfContainmentComponentWithRegFile.cpp

REM Link
link.exe ClientOfContainmentComponentWithRegFile.obj user32.lib gdi32.lib ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

