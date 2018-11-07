CLS

REM Compile
cl.exe /c /EHsc AggregationOuterComponentWithRegFile.cpp

REM Link
link.exe AggregationOuterComponentWithRegFile.obj /DLL /DEF:AggregationOuterComponentWithRegFile.def user32.lib gdi32.lib ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

