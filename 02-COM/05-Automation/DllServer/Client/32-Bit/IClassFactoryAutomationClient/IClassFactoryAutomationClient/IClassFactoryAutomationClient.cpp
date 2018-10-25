#define UNICODE
#include <Windows.h>
#include <stdio.h> // for swprint_s()
#include "AutomationServerWithRegFile.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declaration
IMyMath *pIMyMath = NULL;

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("ComClient");
	HRESULT hr;

	// COM Initialize
	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Library cannot be initialized.\nProgram will now Exit"), TEXT("Program Error"), MB_OK);
		exit(0);
	}

	// WNDCLASSEX Initialization
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register the class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szClassName,
		TEXT("Client of COM DLL Server"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, iCmdShow);

	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// COM uninitialize
	CoUninitialize();
	return((int)msg.wParam);
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declarations 
	void ComErrorDescriptionString(HWND, HRESULT);
	void SafeInterfaceRelease(void);

	// variable declarations
	HRESULT hr;
	int iNum1, iNum2, iResult;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:
		hr = CoCreateInstance(CLSID_MyMath, NULL, CLSCTX_INPROC_SERVER, IID_IMyMath, (void **)&pIMyMath);
		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}

		// Initialize hardcoded arguments
		iNum1 = 155;
		iNum2 = 145;

		// call SumOfTwoIntegers()
		pIMyMath->SumOfTwoIntegers(iNum1, iNum2, &iResult);
		wsprintf(str, TEXT("Sum of %d and %d is %d"), iNum1, iNum2, iResult);
		MessageBox(hwnd, str, TEXT("Sum Of Two Integers"), MB_OK);

		// call SumOfTwoIntegers()
		pIMyMath->SubtractionOfTwoIntegers(iNum1, iNum2, &iResult);
		wsprintf(str, TEXT("Subtraction of %d and %d is %d"), iNum1, iNum2, iResult);
		MessageBox(hwnd, str, TEXT("Subtraction Of Two Integers"), MB_OK);

		// release
		pIMyMath->Release();
		pIMyMath = NULL;

		DestroyWindow(hwnd);

		break;

	case WM_DESTROY:
		SafeInterfaceRelease();
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

void SafeInterfaceRelease(void)
{
	if (pIMyMath)
	{
		pIMyMath->Release();
		pIMyMath = NULL;
	}
}

