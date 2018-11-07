#define UNICODE 
#include <Windows.h>
#include "ClientOfContainmentComponentWithRegFile.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// global variable declaration
ISum *pISum = NULL;
ISubtract *pISubtract = NULL;
IMultiplication *pIMultiplication = NULL;
IDivision *pIDivision = NULL;

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variables
	WNDCLASSEX wndclass;
	HWND  hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("ComClient");
	HRESULT hr = S_OK;

	// code
	// COM Initialize
	hr = CoInitialize(NULL);
	if (FAILED(hr))
	{
		MessageBox(NULL, TEXT("COM Initialization Failed"), TEXT("Error"), MB_OK);
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
	int iNum1, iNum2, iResult;
	TCHAR str[255];

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		hr = CoCreateInstance(CLSID_SumSubtract, NULL, CLSCTX_INPROC_SERVER, IID_ISum, (void **)&pISum);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("ISum Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// initialize arguments hardcoded
		iNum1 = 10;
		iNum2 = 20;

		// call SumOfTwoIntegers() Of ISum to get the sum
		pISum->SumOfTwoIntegers(iNum1, iNum2, &iResult);

		// display the result
		wsprintf(str, TEXT("Sum Of %d and %d = %d"), iNum1, iNum2, iResult);
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
		iNum1 = 123;
		iNum2 = 111;

		// call SubtractionOfTwoIntegers() Of ISubtract to get the subtraction
		pISubtract->SubtractionOfTwoIntegers(iNum1, iNum2, &iResult);

		// display the result
		wsprintf(str, TEXT("Subtraction of %d and %d = %d"), iNum1, iNum2, iResult);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);
		
		// call QueryInterface() on ISubtract, to get IMultiplication's pointer
		hr = pISubtract->QueryInterface(IID_IMultiplication, (void **)&pIMultiplication);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("IMultiplication Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// as ISubtract is now not needed onwards, release it
		pISubtract->Release();
		pISubtract = NULL;

		// again initialize arguments hardcoded
		iNum1 = 25;
		iNum2 = 4;

		// call MultiplicationOfTwoIntegers() of IMultiplication to get the multiplication
		pIMultiplication->MultiplicationOfTwoIntegers(iNum1, iNum2, &iResult);

		// display the result
		wsprintf(str, TEXT("Multiplication of %d and %d = %d"), iNum1, iNum2, iResult);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// call QueryInterface() on IMultiplication, to get IDivision's pointer
		hr = pIMultiplication->QueryInterface(IID_IDivision, (void **)&pIDivision);
		if (FAILED(hr))
		{
			MessageBox(NULL, TEXT("IDivision Interface Can Not Be Obtained"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		// as IMultiplication is now not needed onwards, release it
		pIMultiplication->Release();
		pIMultiplication = NULL;

		// again initialize arguments
		iNum1 = 250;
		iNum2 = 50;

		// call DivisionOfTwoIntegers() of IDivision to get the Division
		pIDivision->DivisionOfTwoIntegers(iNum1, iNum2, &iResult);

		// display the result
		wsprintf(str, TEXT("Division of %d and %d = %d"), iNum1, iNum2, iResult);
		MessageBox(hwnd, str, TEXT("Result"), MB_OK);

		// finally release IDivision Interface
		pIDivision->Release();
		pIDivision = NULL;

		// exit the application
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:
		SafeInterfaceRelease();
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

	if (pIMultiplication)
	{
		pIMultiplication->Release();
		pIMultiplication = NULL;
	}

	if (pIDivision)
	{
		pIDivision->Release();
		pIDivision = NULL;
	}
}
