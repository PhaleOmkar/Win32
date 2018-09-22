#include <Windows.h>
#include <iostream>
#include "ClassFactoryServerWithRegFile.h"

using namespace std;

int main()
{
    HRESULT hr = S_OK;
    ISum *pISum = NULL;
    ISum *pISumAgain = NULL;
    ISubtract *pISubtract = NULL;

    hr = CoInitialize(NULL);
    if (FAILED(hr))
    {
        cout << "COM Library cannot be initialized." << endl;
        return (1);
    }
    cout << "COM Library initialized.." << endl;

    hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&pISum);
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

    hr = pISubtract->QueryInterface(IID_ISum, (void **)&pISumAgain);
    if(FAILED(hr))
    {
        cout << "ISubtract Interface cannot be obtained from ISum" << endl;
        return(1);
    }

    if (pISum == pISumAgain)
    {
        cout << "Both ISum are equal..\nSymmetry Property verified.." << endl;
    }

    pISumAgain->Release();
    pISumAgain = NULL;

    pISubtract->Release();
    pISubtract = NULL;

    pISum->Release();
    pISum = NULL;
    
    CoUninitialize();
    cout << "COM Uninitialized.." << endl;
    return(0);
}