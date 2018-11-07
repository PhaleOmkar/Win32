#define UNICODE
#include <Windows.h>
#include "ClassFactoryServerWithRegFile.h"

// global functions declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declaration
ISum *pISum = NULL;
ISubtract *pISubtract = NULL;
IMultiply *pIMultiply = NULL;

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
	HRESULT hr;
	int iNum1, iNum2, iSum;
	TCHAR str[255];

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		hr = CoCreateInstance(CLSID_SumSubMul, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&pISum);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("ISum Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// initialize arguments hardcoded
		iNum1 = 45;
		iNum2 = 55;

		// call SumOfTwoIntegers() Of ISum to get the sum
		pISum->SumOfTwoIntegers(iNum1, iNum2, &iSum);

		// display the result
		wsprintf(str, TEXT("Sum Of %d and %d = %d"), iNum1, iNum2, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// call QueryInterface() on ISum, to get ISubtract's pointer
		hr = pISum->QueryInterface(IID_ISubtract, (void **)&pISubtract);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("ISubtract Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// as ISum is now not neede onwords, release it
		pISum->Release();
		pISum = NULL; // make release interface NULL

		// again initialize arguments hardcoded
		iNum1 = 155;
		iNum2 = 145;

		// call SubtractionOfTwoIntegers() Of ISubtract to get the subtraction
		pISubtract->SubtractionOfTwoIntegers(iNum1, iNum2, &iSum);

		// display the result
		wsprintf(str, TEXT("Subtraction of %d and %d = %d"), iNum1, iNum2, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// call QueryInterface() on ISubtract to get IMultiply's pointer
		hr = pISubtract->QueryInterface(IID_IMultiply, (void **)&pIMultiply);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("IMultiply Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// as ISubtract is now not needed onwards, release it
		pISubtract->Release();
		pISubtract = NULL;

		// again initialize arguments hardcoded
		iNum1 = 50;
		iNum2 = 10;

		// call MultiplicationOfTwoIntegers() Of IMultiply to get the subtraction
		pIMultiply->MultiplicationOfTwoIntegers(iNum1, iNum2, &iSum);

		// display the result
		wsprintf(str, TEXT("Multiplication of %d and %d = %d"), iNum1, iNum2, iSum);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// exit the application
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
	if (pISum)
	{
		pISum->Release();
		pISum = NULL;
	}

	if (pISubtract)
	{
		pISubtract->Release();
		pISubtract = NULL;
	}

	if (pIMultiply)
	{
		pIMultiply->Release();
		pIMultiply = NULL;
	}
}
