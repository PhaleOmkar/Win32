// Headers
#include <windows.h>
#include "SquareClient_Def.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
typedef int(*lpfnMakeSquare) (int);

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
	TCHAR szMsg[100];
	static HMODULE hDll;
	static lpfnMakeSquare MakeSquare;

	switch (iMsg)
	{
	case WM_CREATE:
		hDll = LoadLibrary(TEXT("Square_Def.dll"));
		
		if (hDll == NULL)
		{
			MessageBox(hwnd, TEXT("Unable to load dll"), TEXT("Error"), MB_OK);
			DestroyWindow(hwnd);
		}

		MakeSquare = (lpfnMakeSquare) GetProcAddress(hDll, "MakeSquare");
		break;

	case WM_LBUTTONDOWN:
		iResult = MakeSquare(4);
		wsprintf(szMsg, "Square of 4 is: %d", iResult);
		MessageBox(hwnd, szMsg, TEXT("Square"), MB_OK);
		break;

	case WM_DESTROY:
		FreeLibrary(hDll);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
