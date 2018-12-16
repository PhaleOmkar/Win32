REM compile
cl.exe /MD /c /ZW /EHsc MyApp.cpp

REM link
link.exe MyApp.obj /APPCONTAINER vccorlib.lib runtimeobject.lib /SUBSYSTEM:WINDOWS

REM Certificate
makecert.exe -n "CN=astromedicomp.org" -r -a sha1 -sv astromedicomp.org.pvk astromedicomp.org.cer -ss root

REM Client Certificate
makecert.exe -n "CN=astromedicomp.org" -a sha1 -sk astromedicomp.org -iv astromedicomp.org.pvk -ic astromedicomp.org.cer -sr currentuser -ss My

REM Creating PFX (Personal Exchange Format) file
pvk2pfx.exe -pvk astromedicomp.org.pvk -spc astromedicomp.org.cer -pfx astromedicomp.org.pfx -po 67890

REM Make Appx
makeappx.exe pack /d ".\My Package" /p MyApp.appx

REM sign the Appx
signtool.exe sign /fd sha256 /sha1 2C26F272AC69EA401996EEC3B472EEBB10E9E148 MyApp.appx

