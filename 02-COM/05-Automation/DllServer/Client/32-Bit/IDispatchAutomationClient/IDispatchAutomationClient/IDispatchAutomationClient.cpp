#define UNICODE
#include <Windows.h>
#include <stdio.h> // for swprintf_s()
#include "AutomationServerWithRegFile.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("COMAutomationClient");

	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hInstance = hInstance;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register the class
	RegisterClassEx(&wndclass);

	// create window 
	hwnd = CreateWindow(szClassName,
		TEXT("COM Automation Client"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// show window
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return((int)msg.wParam);
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declaration
	void ComErrorDescriptionString(HWND, HRESULT);

	// varible declaration
	IDispatch *pIDispatch = NULL;
	HRESULT hr;
	DISPID dispid;
	OLECHAR *szFunctionName1 = (OLECHAR *)L"SumOfTwoIntegers";
	OLECHAR *szFunctionName2 = (OLECHAR *)L"SubtractionOfTwoIntegers";
	VARIANT vArgs[2], vRet;
	DISPPARAMS param = { vArgs, 0, 2, NULL };
	int n1, n2;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:

		// initialize COM
		hr = CoInitialize(NULL);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			MessageBox(hwnd, TEXT("COM Library cannot be initialized "), TEXT("COM Error"), MB_OK);
			DestroyWindow(hwnd);
			exit(0);
		}

		// get ISum Interface
		hr = CoCreateInstance(CLSID_MyMath, NULL, CLSCTX_INPROC_SERVER, IID_IDispatch, (void **)&pIDispatch);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			MessageBox(hwnd, TEXT("ISum cannot be obtained"), TEXT("COM Error"), MB_OK);
			DestroyWindow(hwnd);
			exit(0);
		}

		// Code for SumOfTwoIntegers and SubtractionOfTwoIntegers
		n1 = 75;
		n2 = 25;

		// as DISPPARAMS rgvarg member recieves parameter in reverse order
		VariantInit(vArgs);
		vArgs[0].vt = VT_INT;
		vArgs[0].intVal = n2;
		vArgs[1].vt = VT_INT;
		vArgs[1].intVal = n1;
		param.cArgs = 2;
		param.cNamedArgs = 0;
		param.rgdispidNamedArgs = NULL;
		param.rgvarg = vArgs;

		// return value
		VariantInit(&vRet);

		// sum of two integers 
		hr = pIDispatch->GetIDsOfNames(IID_NULL, &szFunctionName1, 1, GetUserDefaultLCID(), &dispid);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			MessageBox(hwnd, TEXT("Cannot obtain ID of SumOfTwoInteger"), TEXT("COM Error"), MB_OK);
			pIDispatch->Release();

			DestroyWindow(hwnd);
			exit(0);
		}

		hr = pIDispatch->Invoke(dispid,
			IID_NULL,
			GetUserDefaultLCID(),
			DISPATCH_METHOD,
			&param,
			&vRet,
			NULL,
			NULL);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			MessageBox(hwnd, TEXT("Cannot Invoke of SumOfTwoInteger"), TEXT("COM Error"), MB_OK);
			pIDispatch->Release();

			DestroyWindow(hwnd);
			exit(0);
		}
		else
		{
			wsprintf(str, TEXT("Sum of %d and %d is %d"), n1, n2, vRet.lVal);
			MessageBox(hwnd, str, TEXT("Sum Of Two Integers"), MB_OK);
		}

		// subtraction of two integers 
		hr = pIDispatch->GetIDsOfNames(IID_NULL, &szFunctionName2, 1, GetUserDefaultLCID(), &dispid);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			MessageBox(hwnd, TEXT("Cannot obtain ID of SubtractionOfTwoIntegers"), TEXT("COM Error"), MB_OK);
			pIDispatch->Release();

			DestroyWindow(hwnd);
			exit(0);
		}

		hr = pIDispatch->Invoke(dispid,
			IID_NULL,
			GetUserDefaultLCID(),
			DISPATCH_METHOD,
			&param,
			&vRet,
			NULL,
			NULL);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			MessageBox(hwnd, TEXT("Cannot Invoke of SubtractionOfTwoIntegers"), TEXT("COM Error"), MB_OK);
			pIDispatch->Release();

			DestroyWindow(hwnd);
			exit(0);
		}
		else
		{
			wsprintf(str, TEXT("Subtraction of %d and %d is %d"), n1, n2, vRet.lVal);
			MessageBox(hwnd, str, TEXT("Subtraction Of Two Integers"), MB_OK);
		}

		// clean-up
		VariantClear(vArgs);
		VariantClear(&vRet);

		pIDispatch->Release();
		pIDispatch = NULL;
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		CoUninitialize();
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void ComErrorDescriptionString(HWND hwnd, HRESULT hr)
{
	// variable declaration
	TCHAR *szErrorMessage = NULL;
	TCHAR str[255];

	if (FACILITY_WINDOWS == HRESULT_FACILITY(hr))
		hr = HRESULT_CODE(hr);

	if (FormatMessage(FORMAT_MESSAGE_ALLOCATE_BUFFER | FORMAT_MESSAGE_FROM_SYSTEM, NULL, hr, MAKELANGID(LANG_NEUTRAL, SUBLANG_DEFAULT), (LPTSTR)&szErrorMessage, 0, NULL) != 0)
	{
		swprintf_s(str, TEXT("%s"), szErrorMessage);
		LocalFree(szErrorMessage);
	}
	else
		swprintf_s(str, TEXT("[Could not find a description for error #%#x.]\n"), hr);

	MessageBox(hwnd, str, TEXT("COM Error"), MB_OK);
}




