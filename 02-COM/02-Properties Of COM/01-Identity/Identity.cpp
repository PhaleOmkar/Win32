#include <Windows.h>
#include <iostream>
#include "ClassFactoryServerWithRegFile.h"

using namespace std;

int main()
{
    HRESULT hr = S_OK;
    ISum *pISum = NULL;
    ISubtract *pISubtract = NULL;
    IUnknown *pIUnknownFromISum = NULL;
    IUnknown *pIUnknownFromISubtract = NULL;

    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        cout << "COM Library cannot be initialized." << endl;
        return (1);
    }

    hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&pISum);
    if (FAILED(hr))
    {
        cout << "ISum Interface cannot be obtained" << endl;
        return (1);
    }

    hr = pISum->QueryInterface(IID_ISubtract, (void **)&pISubtract);
    if (FAILED(hr))
    {
        cout << "ISubtract Interface cannot be obtained" << endl;
        return (1);
    }

    hr = pISum->QueryInterface(IID_IUnknown, (void **)&pIUnknownFromISum);
    if(FAILED(hr))
    {
        cout << "IUnknown Interface cannot be obtained from ISum" << endl;
        return(1);
    }

    hr = pISubtract->QueryInterface(IID_IUnknown, (void **)&pIUnknownFromISubtract);
    if(FAILED(hr))
    {
        cout << "IUnknown Interface cannot be obtained from ISubtract" << endl;
        return(1);
    }

    if(pIUnknownFromISum == pIUnknownFromISubtract)
    {
        cout << "Both IUnknown are equal!\nIdentity Property Verified.." << endl;
    }
    else 
    {
        cout << "Both IUnknown are not equal!\nIdentity Property Failed.." << endl;
    }

    pIUnknownFromISubtract->Release();
    pIUnknownFromISubtract = NULL;

    pIUnknownFromISum->Release();
    pIUnknownFromISum = NULL;

    pISubtract->Release();
    pISubtract = NULL;

    pISum->Release();
    pISum = NULL;

    CoUninitialize();
    return(0);
}