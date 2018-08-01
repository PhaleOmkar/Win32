// Headers 
#include <Windows.h>

// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variables
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szAppName[] = TEXT("My Window");

	// code
	// initialization of wndclass
	wndclass.cbSize = sizeof(wndclass);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hInstance = hInstance;
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szAppName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// register class
	RegisterClassEx(&wndclass);

	// create window
	hwnd = CreateWindow(szAppName,
		TEXT("My Window"),
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
	HDC hdc;
	PAINTSTRUCT ps;
	RECT rc;
	HBRUSH hBrush;
	static char key;

	switch (iMsg)
	{
	case WM_KEYDOWN:
		switch (wParam)
		{
		case 'R':
			key = 'r';
			break;
		case 'G':
			key = 'g';
			break;
		case 'B':
			key = 'b';
			break;
		case 'Y':
			key = 'y';
			break;
		case 'M':
			key = 'm';
			break;
		case 'C':
			key = 'c';
			break;
		case 'W':
			key = 'w';
			break;
		case 'K':
			key = 'k';
			break;
		default:
			key = 'd';
			break;
		}
		GetClientRect(hwnd, &rc);
		InvalidateRect(hwnd, &rc, TRUE);
		break;

	case WM_PAINT:
		switch (key)
		{
		case 'r':
			hBrush = CreateSolidBrush(RGB(255, 0, 0));
			break;
		case 'g':
			hBrush = CreateSolidBrush(RGB(0, 255, 0));
			break;
		case 'b':
			hBrush = CreateSolidBrush(RGB(0, 0, 255));
			break;
		case 'y':
			hBrush = CreateSolidBrush(RGB(255, 255, 0));
			break;
		case 'm':
			hBrush = CreateSolidBrush(RGB(255, 0, 255));
			break;
		case 'c':
			hBrush = CreateSolidBrush(RGB(0, 255, 255));
			break;
		case 'w':
			hBrush = CreateSolidBrush(RGB(255, 255, 255));
			break;
		case 'k':
			hBrush = CreateSolidBrush(RGB(0, 0, 0));
			break;
		case'd':
		default:
			hBrush = CreateSolidBrush(RGB(128, 128, 128));
			break;
		}

		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);
		FillRect(hdc, &rc, hBrush);
		DeleteObject(hBrush);
		EndPaint(hwnd, &ps);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}