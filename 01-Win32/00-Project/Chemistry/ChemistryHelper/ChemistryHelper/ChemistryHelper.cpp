#define UNICODE
#include <Windows.h>
#include "ChemistryDll.h"

// global variables
IChemisty *pIChemistry = NULL;

// DllMain
BOOL WINAPI DllMain(HINSTANCE hDll, DWORD dwReason, LPVOID Reserved)
{
	HRESULT hr = S_OK;

	switch (dwReason)
	{
	case DLL_PROCESS_ATTACH:
		
		// initialize COM
		hr = CoInitialize(NULL);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("COM Initialization failed in ChemistryHelper!"), TEXT("COM Error"), MB_OK);
			return(FALSE);
		}

		// get IChemistry interface
		hr = CoCreateInstance(CLSID_Chemistry, NULL, CLSCTX_INPROC_SERVER, IID_IChemistry, (void **)&pIChemistry);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("ChemistryHelper failed to obtain the pointer to IChemistry"), TEXT("COM Error"), MB_OK);
			return(FALSE);
		}

		break;

	case DLL_THREAD_ATTACH:
		break;

	case DLL_THREAD_DETACH:
		break;

	case DLL_PROCESS_DETACH:
		
		// release interface
		if (pIChemistry)
		{
			pIChemistry->Release();
			pIChemistry = NULL;
		}

		// uninitialize COM
		CoUninitialize();

		break;
	}

	return(TRUE);
}

// Exported Functions
extern "C" double CalculateNumberOfMolecules(double dMolecularMass, double dMassInGrams)
{
	// variables
	HRESULT hr = S_OK;
	double NumberOfMolecules = 0;

	// code
	hr = pIChemistry->CalculateNumberOfMolecules(dMolecularMass, dMassInGrams, &NumberOfMolecules);
	
	if (SUCCEEDED(hr))
		return(NumberOfMolecules);
	else
		return(NULL);
}
