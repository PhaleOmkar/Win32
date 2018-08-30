// Headers
#include <windows.h>
#include <wchar.h>	
#include <stdio.h>
#include "SquareClient_Def.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
typedef int(*lpfnMakeSquare) (int);
typedef float(*lpfnMakeSquareF) (float);
typedef double(*lpfnMakeSquareD) (double);

// WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("MyApp");

	// code
	// initialization of WNDCLASSEX
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// register above class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szClassName,
		TEXT("Explicit Linking"),
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
	return ((int)msg.wParam);
}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	// code
	int iResult = 0;
	float fResult = 0.0f;
	double dResult = 0.0;
	TCHAR szMsg[100];
	static HMODULE hDll;
	static lpfnMakeSquare MakeSquare;
	static lpfnMakeSquareF MakeSquareF;
	static lpfnMakeSquareD MakeSquareD;

	switch (iMsg)
	{
	case WM_CREATE:
		hDll = LoadLibrary(TEXT("Square_Def.dll"));
		
		if (hDll == NULL)
		{
			MessageBox(hwnd, TEXT("Unable to load dll"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		MakeSquare  = (lpfnMakeSquare)  GetProcAddress(hDll, "MakeSquare");
		MakeSquareF = (lpfnMakeSquareF) GetProcAddress(hDll, "MakeSquareF");
		MakeSquareD = (lpfnMakeSquareD) GetProcAddress(hDll, "MakeSquareD");
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'I':
			iResult = MakeSquare(4);
			sprintf(szMsg, "Square of 4 is: %d", iResult);
			MessageBox(hwnd, szMsg, TEXT("Square"), MB_OK);
			break;

		case 'F':
			fResult = MakeSquareF(5.5f);
			sprintf_s(szMsg, "Square of 5.5 is: %f", fResult);
			MessageBox(hwnd, szMsg, TEXT("Square"), MB_OK);
			break;
				
		case 'D':
			dResult = MakeSquareD(12.34);
			sprintf_s(szMsg, "Square of 12.34 is: %g", dResult);
			MessageBox(hwnd, szMsg, TEXT("Square"), MB_OK);
			break;
		}
		break;

	case WM_DESTROY:
		FreeLibrary(hDll);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
