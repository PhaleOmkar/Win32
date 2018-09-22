cl.exe /c /EHsc Identity.cpp
link.exe Identity.obj user32.lib gdi32.lib kernel32.lib ole32.lib oleaut32.lib uuid.lib /SUBSYSTEM:CONSOLE