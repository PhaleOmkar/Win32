// Headers 
#include <windows.h>
#include "Square_Def.h"

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

// Exported functions
extern "C" int MakeSquare(int iNum)
{
	return (iNum * iNum);
}

extern "C" float MakeSquareF(float fNum)
{
	return (fNum * fNum);
}

extern "C" double MakeSquareD(double dNum)
{
	return (dNum * dNum);
}
