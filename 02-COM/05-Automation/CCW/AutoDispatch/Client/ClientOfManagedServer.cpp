#define UNICODE

#import "Math.tlb" no_namespace, raw_interfaces_only
#include "Math.tlh"

#include <Windows.h>
#include <stdio.h> // for swprintf_s()

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("ComClient");
	HRESULT hr;

	// COM Initialization
	hr = CoInitialize(NULL);

	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Library cannot be initializaed!\nProgram will now termiate."), TEXT("COM Error"), MB_OK);
		exit(0);
	}

	// wndclass initialization
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_PEN);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register window class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szClassName, TEXT("Client Of COM Server"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// message loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// COM Un-Initialization
	CoUninitialize();

	return((int)msg.wParam);
}


// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declaration
	void ComErrorDescriptionString(HWND, HRESULT);

	// variable declaration
	HRESULT hr;
	int iNum1, iNum2;
	long iResult;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:
		CLSID clsidMath;
		iNum1 = 25;
		iNum2 = 5;
		
		IMath *app;
		hr = CLSIDFromProgID(L"ManagedServerForInterop2.Math", &clsidMath);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}

		hr = CoCreateInstance(clsidMath, NULL, CLSCTX_INPROC_SERVER, __uuidof(IMath), (void **)&app);

		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}

		hr = app->MultiplicationOfTwoIntegers(iNum1, iNum2, &iResult);
		
		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}

		wsprintf(str, TEXT("Multiplication Of %d and %d is %d"), iNum1, iNum2, iResult);
		MessageBox(hwnd, str, TEXT("Multiplication"), MB_OK);

		hr = app->DivisionOfTwoIntegers(iNum1, iNum2, &iResult);
		
		if (FAILED(hr))
		{
			ComErrorDescriptionString(hwnd, hr);
			DestroyWindow(hwnd);
		}

		wsprintf(str, TEXT("Division Of %d and %d is %d"), iNum1, iNum2, iResult);
		MessageBox(hwnd, str, TEXT("Division"), MB_OK);

		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
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
