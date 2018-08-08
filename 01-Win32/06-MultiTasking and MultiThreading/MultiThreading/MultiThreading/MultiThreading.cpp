// Headers
#include <Windows.h>
#include <tchar.h>

// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

DWORD WINAPI ThreadProcOne(LPVOID);
DWORD WINAPI ThreadProcTwo(LPVOID);

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
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register class
	RegisterClassEx(&wndclass);

	// Create Window
	hwnd = CreateWindow(szClassName,
		TEXT("Multi Threading"),
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
	static HANDLE hThreadOne = NULL;
	static HANDLE hThreadTwo = NULL;

	switch (iMsg)
	{
	case WM_CREATE:

		// Create Threads
		hThreadOne = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProcOne, (LPVOID)hwnd, 0, NULL);
		hThreadTwo = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadProcTwo, (LPVOID)hwnd, 0, NULL);

		break;

	case WM_LBUTTONDOWN:
		MessageBox(hwnd, TEXT("This is 4th Thread!"), TEXT("Message Box"), MB_OK);
		break;

	case WM_DESTROY:
		
		// Close Thread handles
		CloseHandle(hThreadOne);
		CloseHandle(hThreadTwo);

		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}

// Thread Procs
DWORD WINAPI ThreadProcOne(LPVOID param)
{
	HWND hwnd = (HWND)param;
	TCHAR str[255];

	HDC hdc = GetDC(hwnd);
	SetTextColor(hdc, RGB(0, 255, 0));
	SetBkColor(hdc, RGB(0, 0, 0));
	for (int i = 0; i <= 32767; i++)
	{
		wsprintf(str, TEXT("Incrementing: %d"), i);
		TextOut(hdc, 5, 5, str, _tcsclen(str));
		Sleep(1000);
	}

	ReleaseDC(hwnd, hdc);
	return(0);
}

DWORD WINAPI ThreadProcTwo(LPVOID param)
{

	HWND hwnd = (HWND)param;
	TCHAR str[255];

	HDC hdc = GetDC(hwnd);
	SetTextColor(hdc, RGB(0, 255, 0));
	SetBkColor(hdc, RGB(0, 0, 0));
	for (int i = 32676; i >= 0; i--)
	{
		wsprintf(str, TEXT("Decrementing: %d"), i);
		TextOut(hdc, 5, 25, str, _tcsclen(str));
		Sleep(1000);		
	}

	ReleaseDC(hwnd, hdc);
	return(0);
}
