#define UNICODE

#include <Windows.h>
#include <process.h> // for _wspawnlp() & exit()
#include "ExeServer.h"

// global function
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR szCmdLine, int iCmdShow)
{
	// variable declaraion
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("Client");

	// Window code
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

	// retgister class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szClassName, TEXT("Client Of Exe Server"), WS_OVERLAPPEDWINDOW, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, CW_USEDEFAULT, NULL, NULL, hInstance, NULL);

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
	// variable declaration
	ISum *pISum = NULL;
	ISubtract *pISubtract = NULL;
	HRESULT hr;
	int error, n1, n2, n3;
	TCHAR szWinSysDir[255];
	static TCHAR szPath[_MAX_PATH];
	TCHAR str[255];

	switch (iMsg)
	{
	case WM_CREATE:
		// first register ExeProxySub.dll
		GetSystemDirectory(szWinSysDir, 255);

		wsprintf(szPath, TEXT("%s\\regsvr32.exe"), szWinSysDir);

		error = _wspawnlp(P_WAIT, szPath, szPath, TEXT("/s"), TEXT("C:\\Windows\\SysWOW64\\ExeProxyStub.dll"), NULL);
		if (error == -1)
		{
			MessageBox(hwnd, TEXT("ProxyStub dll cannot be registered."), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
			exit(0);
		}

		// initialize COM library
		hr = CoInitialize(NULL);

		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("COM Library cannot be initialized"), TEXT("COM Error"), MB_OK);
			DestroyWindow(hwnd);
			exit(0);
		}

		// get ISum Interface
		hr = CoCreateInstance(CLSID_CSumSubtract, NULL, CLSCTX_LOCAL_SERVER, IID_ISum, (void **)&pISum);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("cannot obtain ISum interface"), TEXT("COM Error"), MB_OK);
			DestroyWindow(hwnd);
			exit(0);
		}

		n1 = 25;
		n2 = 5;
		pISum->SumOfTwoIntegers(n1, n2, &n3);
		wsprintf(str, TEXT("Sum of %d and %d is %d"), n1, n2, n3);

		MessageBox(hwnd, str, TEXT("Sum"), MB_OK);
		
		// get ISubtract interface
		hr = pISum->QueryInterface(IID_ISubtract, (void **)&pISubtract);
		if (FAILED(hr))
		{
			MessageBox(hwnd, TEXT("Cannot Obtain ISubtract interface"), TEXT("COM Error"), MB_OK);
			pISum->Release();
			DestroyWindow(hwnd);
			exit(0);
		}

		pISum->Release();
		pISubtract->SubtractionOfTwoIntegers(n1, n2, &n3);

		wsprintf(str, TEXT("Subtraction of %d and %d is %d"), n1, n2, n3);
		MessageBox(hwnd, str, TEXT("Subtraction"), MB_OK);

		pISubtract->Release();
		DestroyWindow(hwnd);
		break;

	case WM_DESTROY:

		CoUninitialize();

		error = _wspawnlp(P_WAIT, szPath, szPath, TEXT("/u"), TEXT("/s"), TEXT("C:\\Windows\\SysWOW64\\ExeProxyStub.dll"), NULL);
		if (error == -1)
		{
			MessageBox(hwnd, TEXT("ProxyStub dll cannot be un-registered."), TEXT("ERROR"), MB_OK);
			DestroyWindow(hwnd);
			exit(0);
		}

		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}