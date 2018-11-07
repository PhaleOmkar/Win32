CLS

REM Compile
cl.exe /c /EHsc ClientOfClassFactoryServerWithRegFile.cpp

REM Link
link.exe ClientOfClassFactoryServerWithRegFile.obj user32.lib gdi32.lib ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

