CLS

REM Compile
cl.exe /c /EHsc ContainmentInnerComponentWithRegFile.cpp

REM Link
link.exe ContainmentInnerComponentWithRegFile.obj /DLL /DEF:ContainmentInnerComponentWithRegFile.def ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

