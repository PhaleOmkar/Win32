// Headers
#include <Windows.h>
#include <time.h>
#include "Snake.h"

// Structs
typedef struct tagCOLORS {
	int iR;
	int iG;
	int iB;
	HWND hwnd;
} COLORS, *LPCOLORS;

typedef struct tagFOOD {
	LONG iX;
	LONG iY;
} FOOD;

typedef struct tagSNAKE {
	int iDirection;
	int iLength;
	int body[SIZE*SIZE][2];
} SNAKE, *LPSNAKE;

typedef struct tagSTATE {
	int iScore;
	FOOD Food;
	SNAKE Snake;
	HWND hwnd;
} STATE, *LPSTATE;

// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
DWORD ThreadSnakeMove(LPVOID);
void ResetGame(LPSTATE);


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
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hbrBackground = (HBRUSH)GetStockObject(DKGRAY_BRUSH);
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

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
	ShowWindow(hwnd, SW_MAXIMIZE);
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
	static HANDLE hMutex = NULL;

	static COLORS colors = { 0, 0, 0, NULL };

	static STATE State;
	static RECT rcUnit, rcCanvas;
	RECT rc;
	static HDC hdc;
	PAINTSTRUCT ps;
	HBRUSH hBrush;
	// HBRUSH hbrDarkGreen, hbrLightGreen;
	TCHAR lpszScore[255]=TEXT("");
	static int height, width, size, iResult;

	switch (iMsg)
	{
	case WM_CREATE:

		// Initialization 
		srand((unsigned)time(NULL));
		hMutex = CreateMutex(NULL, FALSE, TEXT("SnakeMutex"));

		// Init the STATE
		ResetGame(&State);
		State.hwnd = hwnd;

		// Create Threads
		hThreadMove = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadSnakeMove, (LPVOID)&State, 0, NULL);

		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case 'P':
			// Pause Main Snake Thread
			SuspendThread(hThreadMove);

			iResult = MessageBox(hwnd, TEXT("Press OK to resume"), TEXT("Game Paused"), MB_OK | MB_ICONINFORMATION );
			if (iResult == IDOK)
			{
				// Resume Thread
				ResumeThread(hThreadMove);
			}
			break;

		case 'W':
		case VK_UP:
			if (State.Snake.iDirection != DIR_DW)
				State.Snake.iDirection = DIR_UP;
			break;

		case 'A':
		case VK_LEFT:
			if (State.Snake.iDirection != DIR_RT)
				State.Snake.iDirection = DIR_LF;
			break;

		case 'S':
		case VK_DOWN:
			if (State.Snake.iDirection != DIR_UP)
				State.Snake.iDirection = DIR_DW;
			break;

		case 'D':
		case VK_RIGHT:
			if (State.Snake.iDirection != DIR_LF)
				State.Snake.iDirection = DIR_RT;
			break;

		case VK_ESCAPE:
			// Pause Game
			SuspendThread(hThreadMove);

			// Confirm Exit
			iResult = MessageBox(hwnd, TEXT("Do You Want To Quit?"), TEXT("SnAkE!"), MB_YESNO | MB_ICONQUESTION);
			if (iResult == IDYES) DestroyWindow(hwnd);
			else ResumeThread(hThreadMove);

			break;
		}

		GetClientRect(hwnd, &rc);
		InvalidateRect(hwnd, &rc, TRUE);

		break;

	case WM_PAINT:
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);

		height = rc.bottom - rc.top;
		width = rc.right - rc.left;
		size = width > height ? height : width;

		// Create Canvas
		rcCanvas.left = rc.left + 80;
		rcCanvas.top = rc.top;
		rcCanvas.right = rcCanvas.left + size;
		rcCanvas.bottom = rcCanvas.top + size;

		// hbrLightGreen = CreateSolidBrush(RGB(0, 95, 0));
		// hbrDarkGreen = CreateSolidBrush(RGB(0, 75, 0));

		// Fill canvas in all black
		hBrush = CreateSolidBrush(RGB(0, 0, 0));
		FillRect(hdc, &rcCanvas, hBrush);

		// Draw Snake
		OpenMutex(SYNCHRONIZE, FALSE, TEXT("SnakeMutex"));
		for (int i = 0; i < State.Snake.iLength; i++)
		{
			rcUnit.left = rcCanvas.left + (State.Snake.body[i][0] * (size / SIZE));
			rcUnit.top = rcCanvas.top + (State.Snake.body[i][1] * (size / SIZE));
			rcUnit.right = rcUnit.left + (size / SIZE);
			rcUnit.bottom = rcUnit.top + (size / SIZE);

			hBrush = CreateSolidBrush(RGB(255, 0, 0));  /* White Snake */
			//hBrush = CreateSolidBrush(RGB(255 - (i*2), 255 - (i * 2), 255 - (i * 2))); /* White Gradiant Snake*/
			//hBrush = CreateSolidBrush(RGB(128, 64, 0)); /* Brown Snake*/
			FillRect(hdc, &rcUnit, hBrush);
		}
		ReleaseMutex(hMutex);

		// Draw Food
		/*rcUnit.left = rcCanvas.left + (State.Food.iX * (size / SIZE));
		rcUnit.top = rcCanvas.top + (State.Food.iY * (size / SIZE));
		rcUnit.right = rcUnit.left + (size / SIZE);
		rcUnit.bottom = rcUnit.top + (size / SIZE);

		hBrush = CreateSolidBrush(RGB(0, 255, 128));
		FillRect(hdc, &rcUnit, hBrush);
*/
		// Draw Score
		wsprintf(lpszScore, TEXT("Score: %d\nSnake:\n Length: %d\n Head:\n  X: %d\n  Y: %d\nFood:\n X: %d\n Y: %d"), State.iScore, State.Snake.iLength, State.Snake.body[0][0], State.Snake.body[0][1], State.Food.iX, State.Food.iY);
		SetTextColor(hdc, RGB(0, 0, 0));
		DrawText(hdc, lpszScore, -1, &rc, DT_LEFT);

		// Finish
		//DeleteObject(hBrush);
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
	int iResult;
	TCHAR lpszMsg[255];
	LPSTATE lpState = (LPSTATE)param;
	int iTempX = lpState->Snake.body[lpState->Snake.iLength - 1][0];
	int iTempY = lpState->Snake.body[lpState->Snake.iLength - 1][1];
	HANDLE hMutex = OpenMutex(SYNCHRONIZE, FALSE, TEXT("SnakeMutex"));

	while (TRUE)
	{
		Sleep(100);


		for (int i = lpState->Snake.iLength - 1; i > 0; i--)
		{
			lpState->Snake.body[i][0] = lpState->Snake.body[i - 1][0];
			lpState->Snake.body[i][1] = lpState->Snake.body[i - 1][1];
		}

		switch (lpState->Snake.iDirection)
		{
		case DIR_UP:
			lpState->Snake.body[0][0] = lpState->Snake.body[1][0];

			// Canvas Edge condition
			if ((lpState->Snake.body[1][1] - 1) < 0) lpState->Snake.body[0][1] = SIZE;
			else lpState->Snake.body[0][1] = (lpState->Snake.body[1][1] - 1) % SIZE;

			break;

		case DIR_LF:

			// Canvas Edge condition
			if ((lpState->Snake.body[1][0] - 1) < 0) lpState->Snake.body[0][0] = SIZE;
			else lpState->Snake.body[0][0] = (lpState->Snake.body[1][0] - 1) % SIZE;

			lpState->Snake.body[0][1] = lpState->Snake.body[1][1];

			break;

		case DIR_DW:
			lpState->Snake.body[0][0] = lpState->Snake.body[1][0];
			lpState->Snake.body[0][1] = (lpState->Snake.body[1][1] + 1) % (SIZE + 1);
			break;

		case DIR_RT:
			lpState->Snake.body[0][0] = (lpState->Snake.body[1][0] + 1) % (SIZE + 1);
			lpState->Snake.body[0][1] = lpState->Snake.body[1][1];
			break;
		}

		// If Snake Head is on the Snake Body
		for (int i = 1; i < lpState->Snake.iLength; i++)
		{
			if (lpState->Snake.body[0][0] == lpState->Snake.body[i][0] &&
				lpState->Snake.body[0][1] == lpState->Snake.body[i][1])
			{
				wsprintf(lpszMsg, TEXT("Score: %d \nTry Again?"), lpState->iScore);
				iResult = MessageBox(lpState->hwnd, lpszMsg, TEXT("SnAkE!"), MB_YESNO );
				if (iResult == IDYES) {
					ResetGame(lpState);
				}
				else {
					SendMessage(lpState->hwnd, WM_CLOSE, 0, 0);
				}
			}
		}

		// If Snake head is on the food
		if (lpState->Food.iX == lpState->Snake.body[0][0] && lpState->Food.iY == lpState->Snake.body[0][1])
		{
			lpState->Snake.body[lpState->Snake.iLength][0] = iTempX;
			lpState->Snake.body[lpState->Snake.iLength][1] = iTempY;
			lpState->Snake.iLength++;
			lpState->iScore += 10;
			lpState->Food.iX = (double)rand() / (RAND_MAX + 1) * (SIZE);
			lpState->Food.iY = (double)rand() / (RAND_MAX + 1) * (SIZE);
		}
		ReleaseMutex(hMutex);

		HWND hwnd = lpState->hwnd;
		RECT rc;
		GetClientRect(hwnd, &rc);
		InvalidateRect(hwnd, &rc, TRUE);
	}
	return(0);
}

void ResetGame(LPSTATE State)
{
	State->Food.iX = (SIZE / 3);
	State->Food.iY = (SIZE / 3);

	State->Snake.iLength = 10;

	for (int i = 0; i < State->Snake.iLength; i++)
	{
		State->Snake.body[i][0] = (SIZE / 2) - i;
		State->Snake.body[i][1] = (SIZE / 2);
	}

	State->Snake.iDirection = DIR_RT;
	State->iScore = 0;
}
