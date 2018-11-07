CLS

REM Register the dll
SumSubMul.reg

REM Compile
cl.exe /c /EHsc SumSubMulDll.cpp

REM Link
link.exe SumSubMulDll.obj /DLL /DEF:SumSubMulDll.def ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

