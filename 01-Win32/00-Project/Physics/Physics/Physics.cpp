// Headers
#include <Windows.h>
#include "Physics.h"

// DllMain
BOOL WINAPI DllMain(HINSTANCE hInstance, DWORD dwReason, LPVOID Reserved)
{
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

	return(TRUE);
}

// Exported Functions
extern "C" double CalculateCentripetalForce(double dMass, double dVelocity, double dRadius)
{
	// calculate Centrifugal Force: m * v^2 / r
	return((dMass * dVelocity * dVelocity) / dRadius);
}

extern "C" double CalculateCentripetalAcceleration(double dVelocity, double dRadius)
{
	// calculate Centripetal Acceleration: v^2 / r
	return((dVelocity * dVelocity) / dRadius);
}
