#define UNICODE
#include <Windows.h>
#include "MonikerDllServer.h"

// global functions declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declaration
IOddNumber *pIOddNumber = NULL;
IOddNumberFactory *pIOddNumberFactory = NULL;

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("ComClient");
	HRESULT hr;

	// code
	// COM Initialization
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Library Can Not Be Initialized.\nProgram will not Exit."), TEXT("Program Error"), MB_OK);
		exit(0);
	}

	// WNDCLASSEX initialization
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hInstance = hInstance;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register window class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szClassName,
		TEXT("Client of Moniker DLL Server"),
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

	// Message Loop
	while (GetMessage(&msg, hwnd, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	// COM Uninitialization
	CoUninitialize();
	return ((int)msg.wParam);
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// function declaration
	void SafeInterfaceRelease(void);

	// variable declaration
	IBindCtx *pIBindCtx = NULL;
	IMoniker *pIMoniker = NULL;
	ULONG uEaten;
	HRESULT hr;
	LPOLESTR szCLSID = NULL;
	wchar_t wszCLSID[255], wszTemp[255], *ptr;
	int iFirstOddNumber, iNextOddNumber;
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:

		// create a bind context
		if (hr = CreateBindCtx(0, &pIBindCtx) != S_OK)
		{
			MessageBox(hwnd, TEXT("Failed to get IBindCtx Interface pointer"), TEXT("COM Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// get string form of Binary CLSID
		StringFromCLSID(CLSID_OddNumber, &szCLSID);

		wcscpy_s(wszTemp, szCLSID);
		ptr = wcschr(wszTemp, '{');
		ptr = ptr + 1; // remove opening '{'
		wcscpy_s(wszTemp, ptr);
		wszTemp[(int)wcslen(wszTemp) - 1] = '\0'; // remove closing '}'
		wsprintf(wszCLSID, TEXT("clsid:%s"), wszTemp);

		// Get moniker for this CLSID
		hr = MkParseDisplayName(pIBindCtx, wszCLSID, &uEaten, &pIMoniker);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to Make parse display name"), TEXT("COM Error"), MB_OK);
			pIBindCtx->Release();
			pIBindCtx = NULL;

			DestroyWindow(hwnd);
		}

		// bind moniker to named object
		hr = pIMoniker->BindToObject(pIBindCtx, NULL, IID_IOddNumberFactory, (void **)&pIOddNumberFactory);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Failed to Get custom action activation - IOddNumberFactory interface pointer"), TEXT("COM Error"), MB_OK);
			pIMoniker->Release();
			pIMoniker = NULL;
			pIBindCtx->Release();
			pIBindCtx = NULL;

			DestroyWindow(hwnd);
		}

		// release moniker and binder
		pIMoniker->Release();
		pIMoniker = NULL;
		pIBindCtx->Release();
		pIBindCtx = NULL;

		// initialize arguments
		iFirstOddNumber = 57;

		hr = pIOddNumberFactory->SetFirstOddNumber(iFirstOddNumber, &pIOddNumber);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Cannot obtain IOddNumber interface"), TEXT("COM Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// release IOddNumberFactory interface
		pIOddNumberFactory->Release();
		pIOddNumberFactory = NULL;

		// call GetNextNumber() function
		pIOddNumber->GetNextOddNumber(&iNextOddNumber);

		// release IOddNumber interface
		pIOddNumber->Release();
		pIOddNumber = NULL;

		// show the result
		wsprintf(str, TEXT("The next odd number from %2d is %2d"), iFirstOddNumber, iNextOddNumber);
		MessageBox(hwnd, str, TEXT("Odd Number"), MB_OK | MB_TOPMOST);

		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

void SafeInterfaceRelease(void)
{
	// code
	if (pIOddNumber)
	{
		pIOddNumber->Release();
		pIOddNumber = NULL;
	}

	if (pIOddNumberFactory)
	{
		pIOddNumberFactory->Release();
		pIOddNumberFactory = NULL;
	}
}

