CLS

REM Register the dll
ClientOfAggregationComponentWithRegFile.reg

REM Compile
cl.exe /c /EHsc ClientOfAggregationComponentWithRegFile.cpp

REM Link
link.exe ClientOfAggregationComponentWithRegFile.obj user32.lib gdi32.lib ole32.lib oleaut32.lib /SUBSYSTEM:WINDOWS /MACHINE:X64 

