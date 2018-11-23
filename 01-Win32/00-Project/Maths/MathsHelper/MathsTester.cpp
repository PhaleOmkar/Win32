#define UNICODE

#import "MathsDll.tlb" no_namespace, raw_interfaces_only
#include "MathsDll.tlh"
#include <Windows.h>
#include <iostream>
using namespace std;

// global variables
IMath *pIMaths = NULL;

int main()
{
    HRESULT hr;
    CLSID clsidMath;
    TCHAR str[255];
    OLECHAR* guidString;

    hr = CoInitializeEx(NULL, COINIT_APARTMENTTHREADED);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("COM Initialization failed in MathsHelper!"), TEXT("COM Error"), MB_OK);
        return(FALSE);
    }
    MessageBox(NULL, TEXT("COM Initalized"), TEXT("DEBUG"), MB_OK);
    // get CLSID of Math
    hr = CLSIDFromProgID(L"MathsManagedServer.Math", &clsidMath);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("MathsHelper failed to obtain the CLSID of MathsManagedServer.Math type"), TEXT("COM Error"), MB_OK);
        return(FALSE);
    }
    StringFromCLSID(clsidMath, &guidString);
    MessageBox(NULL, guidString, TEXT("DEBUG"), MB_OK);
    // get IMaths interface
    hr = CoCreateInstance(clsidMath, NULL, CLSCTX_INPROC_SERVER, __uuidof(IMath), (void **)&pIMaths);
    if (FAILED(hr))
    {
        MessageBox(NULL, TEXT("MathsHelper failed to obtain the pointer to IMath"), TEXT("COM Error"), MB_OK);
        return(FALSE);
    }
    MessageBox(NULL, TEXT("IMath created"), TEXT("DEBUG"), MB_OK);
    SolutionOfQuadraticEq soln;
    hr = pIMaths->CalculateRootsOfEq(1, 2, 1, &soln);
    cout << "Roots: "  << soln.NoOfRoots << endl;
    cout << "Root 1: " << soln.root1 << endl;
    cout << "Root 2: " << soln.root2 << endl;

}