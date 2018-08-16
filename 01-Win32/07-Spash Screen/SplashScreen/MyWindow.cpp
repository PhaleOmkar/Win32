// Headers
#include <windows.h>
#include "MyResources.h"

// global function declaration
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain()
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variable declaration
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("MyApp");

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
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

	// register above class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szAppName,
		TEXT("My Application"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	ShowWindow(hwnd, SW_MAXIMIZE);
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
	static int cxClient, cyClient;
	static HBITMAP hbmpImage = NULL;
	
	HINSTANCE hInstance = NULL;
	HDC hdc, hdcCompatible;
	BITMAP bmpImage;
	PAINTSTRUCT ps;
	RECT rc;
	HFONT hFont = NULL;

	// code
	switch (iMsg)
	{
	case WM_CREATE:
		hInstance = (HINSTANCE)GetWindowLong(hwnd, GWL_HINSTANCE);
		hbmpImage = LoadBitmap(hInstance, MAKEINTRESOURCE(BMPEARTH));
		break;

	case WM_KEYDOWN:
		switch (wParam)
		{
		case VK_SPACE:
			MessageBox(hwnd, TEXT("This is EARTH"), TEXT("Message Box"), MB_OK);
			break;

		case VK_ESCAPE:
			DestroyWindow(hwnd);
			break;
		}
		break;

	case WM_SIZE:
		cxClient = LOWORD(lParam);
		cyClient = HIWORD(lParam);
		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		hdcCompatible = CreateCompatibleDC(hdc);
		GetObject(hbmpImage, sizeof(BITMAP), &bmpImage);
		SelectObject(hdcCompatible, hbmpImage);
		BitBlt(hdc, 0, 0, cxClient, cyClient, hdcCompatible, 0, 0, SRCCOPY);
		//StretchBlt(hdc, 0, 0, cxClient, cyClient, hdcCompatible, 0, 0, bmpImage.bmWidth, bmpImage.bmHeight, SRCCOPY);
		DeleteDC(hdcCompatible);

		GetClientRect(hwnd, &rc);
		//hFont = (HFONT)GetStockObject(ANSI_FIXED_FONT);
		hFont = CreateFont(30, 0, 0, 0, FW_SEMIBOLD, 0, 0, 0, DEFAULT_CHARSET, OUT_DEFAULT_PRECIS, CLIP_DEFAULT_PRECIS, PROOF_QUALITY, FF_DONTCARE, TEXT("Consolas"));
		SelectObject(hdc, hFont);
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		DrawText(hdc, TEXT("Press Space to continue"), -1, &rc, DT_CENTER | DT_BOTTOM | DT_SINGLELINE);

		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		DeleteObject(hFont);
		DeleteObject(hbmpImage);
		PostQuitMessage(0);
		break;
	}
	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}
