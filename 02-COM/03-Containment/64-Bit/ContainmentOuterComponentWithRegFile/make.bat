CLS

REM Compile
cl.exe /c /EHsc ContainmentOuterComponentWithRegFile.cpp

REM Link
link.exe ContainmentOuterComponentWithRegFile.obj /DLL /DEF:ContainmentOuterComponentWithRegFile.def user32.lib gdi32.lib ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

