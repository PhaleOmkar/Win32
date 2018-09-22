#include <Windows.h>
#include <iostream>
#include "SumSubMulDll.h"

using namespace std;

int main()
{
    HRESULT hr = S_OK;
    ISum *pISum = NULL;
    ISubtract *pISubtract = NULL;
    IMultiply *pIMultiply = NULL;
    ISum *pISumAgain = NULL;

    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        cout << "COM Library cannot be initialized." << endl;
        return (1);
    }
    cout << "COM Library initialized.." << endl;

    hr = CoCreateInstance(CLSID_SumSubMul, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&pISum);
    if (FAILED(hr))
    {
        cout << "ISum Interface cannot be obtained" << endl;
        return (1);
    }

    hr = pISum->QueryInterface(IID_ISubtract, (void **)&pISubtract);
    if (FAILED(hr))
    {
        cout << "ISubtract Interface cannot be obtained from ISum" << endl;
        return (1);
    }

    hr = pISubtract->QueryInterface(IID_IMultiply, (void **)&pIMultiply);
    if (FAILED(hr))
    {
        cout << "IMultiply Interface cannot be obtained from ISubtract" << endl;
        return (1);
    }

    pISubtract->Release();
    pISubtract = NULL;

    hr = pIMultiply->QueryInterface(IID_ISum, (void **)&pISumAgain);
    if(FAILED(hr))
    {
        cout << "ISum Interface cannot be obtained from IMultiply" << endl;
        return (1);
    }

    pIMultiply->Release();
    pIMultiply = NULL;

    if(pISum == pISumAgain)
    {
        cout << "Both ISum are equal!\nTransitivity Property Verified.." << endl;
    }
    else
    {
        cout << "Both ISum are not eqaul!\nTransitivity Property Failed.." <<endl;
    }
    
    pISumAgain->Release();
    pISumAgain = NULL;

    pISum->Release();
    pISum = NULL;

    CoUninitialize();
    cout << "COM Uninitialized.." << endl;
    return(0);
}