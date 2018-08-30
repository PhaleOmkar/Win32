// Headers
#include <Windows.h>
#include "MyDll.h"

// DllMain
BOOL WINAPI DllMain(HMODULE hDll, DWORD dwReason, LPVOID lpReserved)
{
	// code
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
__declspec(dllexport) int MakeSquare(int iNum)
{
	return(iNum * iNum);
}
