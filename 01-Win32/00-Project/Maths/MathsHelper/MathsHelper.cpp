#define UNICODE

#import "MathsDll.tlb" no_namespace, raw_interfaces_only
#include "MathsDll.tlh"
#include <Windows.h>

// global variables
IMath *pIMaths = NULL;

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
		
		// release interface
		if (pIMaths)
		{
			pIMaths->Release();
			pIMaths = NULL;
		}

		// uninitialize COM
		CoUninitialize();

		break;
	}

	return(TRUE);
}

void Initialize(void)
{
    if(pIMaths != NULL)
        return;
    
    HRESULT hr = S_OK;
    CLSID clsidMath;

	// initialize COM
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Initialization failed in MathsHelper!"), TEXT("COM Error"), MB_OK);
        return;
	}

    // get CLSID of Math
    hr = CLSIDFromProgID(L"MathsManagedServer.Math", &clsidMath);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("MathsHelper failed to obtain the CLSID of MathsManagedServer.Math type"), TEXT("COM Error"), MB_OK);
        return;
	}

	// get IMaths interface
	hr = CoCreateInstance(clsidMath, NULL, CLSCTX_INPROC_SERVER, __uuidof(IMath), (void **)&pIMaths);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("MathsHelper failed to obtain the pointer to IMath"), TEXT("COM Error"), MB_OK);
        return;
	}
}

// Exported Functions
extern "C" long CalculateRootsOfEq(double dA, double dB, double dC, double *pRoot1, double *pRoot2)
{
	// variables
	HRESULT hr = S_OK;
    struct SolutionOfQuadraticEq solution;

    // Initialize, if not
    if (pIMaths == NULL)
    {
        Initialize();
        
        // If it is still NULL, Dll must be missing
        if(pIMaths == NULL)
            return NULL;
    }

	// code
	hr = pIMaths->CalculateRootsOfEq(dA, dB, dC, &solution);
	
	if (SUCCEEDED(hr))
    {
        switch(solution.NoOfRoots)
        {
            case 2:
                *pRoot2 = solution.root1;
                *pRoot1 = solution.root2;
                break;

            case 1:
                *pRoot1 = solution.root1;
                break;

            case 0:
                break;
        }
        
		return(solution.NoOfRoots);
    }
	else
		return(NULL);

}
