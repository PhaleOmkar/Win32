// Headers 
#include <windows.h>
#include "MyDll.h"

// DllMain
BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	// Code
	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		break;

	}
	
	return TRUE;
}

// Exported Functions
extern "C" int MakeSquare(int iNum)
{
	return (iNum * iNum);
}