// Headers
#include <Windows.h>
#include <tchar.h>
#include "Snake.h"


// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD ThreadSnakeMove(LPVOID);
DWORD ThreadProcFour(LPVOID);

// Structs
typedef struct tagCOLORS {
	int iR;
	int iG;
	int iB;
	HWND hwnd;
} COLORS, *LPCOLORS;

typedef struct tagPOSITION {
	int iX;
	int iY;
} POSITION;

typedef struct tagSTATE {
	Snake Snake;
	HWND hwnd;
} STATE, *LPSTATE;

static POSITION position = { 0, 0 };


// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variables 
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("MyWindow");

	// Window Structure
	wndclass.cbSize = sizeof(wndclass);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindow(szClassName,
		TEXT("SnAkE!"),
		WS_OVERLAPPEDWINDOW,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		CW_USEDEFAULT,
		NULL,
		NULL,
		hInstance,
		NULL);

	// Display Window
	ShowWindow(hwnd, iCmdShow);
	UpdateWindow(hwnd);

	// Message Loop
	while (GetMessage(&msg, NULL, 0, 0))
	{
		TranslateMessage(&msg);
		DispatchMessage(&msg);
	}

	return ((int)msg.wParam);
}

// Window Procedure
LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static HANDLE hThreadMove = NULL;
	static HANDLE hThreadControl = NULL;

	static COLORS colors = { 0, 0, 0, NULL };
	

	RECT rc;
	HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	int height, width, size;

	switch (iMsg)
	{
	case WM_CREATE:

		colors.hwnd = hwnd;

		// Create Threads
		hThreadMove = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadSnakeMove, (LPVOID)hwnd, 0, NULL);


		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case 'W':
		case VK_UP:
			if (position.iY == 0)
				position.iY = sizeCanvas;
			else
			{
				position.iY = (position.iY - 1) % sizeCanvas;
			}
			break;

		case 'A':
		case VK_LEFT:
			if (position.iX == 0)
				position.iX = sizeCanvas;
			else
			{
				position.iX = abs((position.iX - 1) % sizeCanvas);
			}
			break;

		case 'S':
		case VK_DOWN:
			position.iY = (position.iY + 1) % sizeCanvas;
			break;

		case 'D':
		case VK_RIGHT:
			position.iX = (position.iX + 1) % sizeCanvas;
			break;
		}

		GetClientRect(hwnd, &rc);
		InvalidateRect(hwnd, &rc, FALSE);

		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("This is 4th Thread!"), TEXT("Message Box"), MB_OK);
		break;

	case WM_PAINT:

		RECT rcPlayer, rcCanvas;

		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);

		height = rc.bottom - rc.top;
		width = rc.right - rc.left;
		size = width > height ? height : width;

		rcCanvas.left = rc.left;
		rcCanvas.top = rc.top;
		rcCanvas.right = rcCanvas.left + size;
		rcCanvas.bottom = rcCanvas.top + size;

		rcPlayer.left = rcCanvas.left + (position.iX * (size / sizeCanvas));
		rcPlayer.top = rcCanvas.top + (position.iY * (size / sizeCanvas));
		rcPlayer.right = rcPlayer.left + (size / sizeCanvas);
		rcPlayer.bottom = rcPlayer.top + (size / sizeCanvas);

		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &rcCanvas, hBrush);
		hBrush = CreateSolidBrush(RGB(255, 255, 255));
		FillRect(hdc, &rcPlayer, hBrush);

		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);

		break;

	case WM_DESTROY:

		// Close Thread handles

		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

//// Thread Procs
DWORD ThreadSnakeMove(LPVOID param)
{
	while (TRUE) 
	{
		Sleep(1000);
		position.iX = position.iX++ % sizeCanvas;
		position.iY = position.iY++ % sizeCanvas;
		HWND hwnd = (HWND)param;
		RECT rc;
		HDC hdc = GetDC(hwnd);
		GetClientRect(hwnd, &rc);
		InvalidateRect(hwnd, &rc, TRUE);
		ReleaseDC(hwnd, hdc);
	}
	return(0);
}

//DWORD ThreadProcTwo(LPVOID param)
//{
//
//	HWND hwnd = (HWND)param;
//	TCHAR str[255];
//
//	HDC hdc = GetDC(hwnd);
//	SetTextColor(hdc, RGB(0, 255, 0));
//	SetBkColor(hdc, RGB(0, 0, 0));
//	for (int i = 32676; i >= 0; i--)
//	{
//		wsprintf(str, TEXT("Decrementing: %d"), i);
//		TextOut(hdc, 5, 25, str, _tcsclen(str));
//	}
//
//	ReleaseDC(hwnd, hdc);
//	return(0);
//}
//
//DWORD ThreadProcThree(LPVOID param)
//{
//	RECT rc;
//	LPCOLORS colors = (LPCOLORS)param;
//	for (int i = 0; i <= 255; i++) {
//		colors->iR = colors->iG = colors->iB = i;
//		GetClientRect(colors->hwnd, &rc);
//		InvalidateRect(colors->hwnd, &rc, TRUE);
//		Sleep(30);
//	}
//	return(0);
//}
//
//DWORD ThreadProcFour(LPVOID param)
//{
//	RECT rc;
//	LPCOLORS colors = (LPCOLORS)param;
//	for (colors->iR = 0; colors->iR <= 255; colors->iR++) {
//		for (colors->iG = 0; colors->iG <= 255; colors->iG++) {
//			colors->iR++;
//			for (colors->iB = 0; colors->iB <= 255; colors->iB++) {
//				colors->iR++;
//				colors->iB++;
//				GetClientRect(colors->hwnd, &rc);
//				InvalidateRect(colors->hwnd, &rc, TRUE);
//				Sleep(10);
//			}
//		}
//	}
//	return(0);
//}
