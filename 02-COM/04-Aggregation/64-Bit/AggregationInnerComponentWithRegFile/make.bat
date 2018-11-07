CLS

REM Compile
cl.exe /c /EHsc AggregationInnerComponentWithRegFile.cpp

REM Link
link.exe AggregationInnerComponentWithRegFile.obj /DLL /DEF:AggregationInnerComponentWithRegFile.def ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

