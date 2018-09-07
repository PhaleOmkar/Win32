// Headers
#include <Windows.h>

// Types
typedef struct tagPOINTEX {
	double x;
	double y;
	double z;
} POINTEX, *LPPOINTEX;

typedef struct tagLINE {
	POINTEX ptStart;
	POINTEX ptEnd;
} LINE, *LPLINE;

typedef struct tagCAMERA {
	int xAngle;
	int yAngle;
	int zAngle;
} CAMERA, *LPCAMERA;

typedef struct tagMODEL {
	int iNoOfLines;
	LINE arrLine[100];
} MODEL, *LPMODEL;

typedef struct tagSTATE {
	HWND hwnd;
	MODEL Model;
	CAMERA Camera;
} STATE, *LPSTATE;

// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

// WinMain
int WINAPI WinMain(HINSTANCE hInstance, HINSTANCE hPrevInstance, LPSTR lpszCmdLine, int iCmdShow)
{
	// variables
	WNDCLASSEX wndclass;
	HWND hwnd;
	MSG msg;
	TCHAR szClassName[] = TEXT("MyClass");

	// code
	wndclass.cbSize = sizeof(WNDCLASSEX);
	wndclass.style = CS_HREDRAW | CS_VREDRAW;
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpfnWndProc = WndProc;
	wndclass.hbrBackground = (HBRUSH)GetStockObject(BLACK_BRUSH);
	wndclass.hIcon = LoadIcon(NULL, IDI_APPLICATION);
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(NULL, IDI_APPLICATION);

	// Register Class
	RegisterClassEx(&wndclass);

	hwnd = CreateWindow(szClassName,
		TEXT("3D Visualizer"),
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

	// Message Loop
	while (TRUE)
	{
		if (PeekMessage(&msg, NULL, 0, 0, PM_REMOVE))
		{
			if (msg.message == WM_QUIT)
				break;

			TranslateMessage(&msg);
			DispatchMessage(&msg);
		}

	}
	return msg.wParam;

}

LRESULT CALLBACK WndProc(HWND hwnd, UINT iMsg, WPARAM wParam, LPARAM lParam)
{
	static STATE State;
	static int Offset[2] = { 0, 0 };
	static int Origin[2] = { 0, 0 };

	int iResult;

	RECT rc;
	PAINTSTRUCT ps;
	HDC hdc;
	POINT arrPoints[50];
	HPEN hPen;
	POINTEX ptA, ptB, ptC, ptD, ptE, ptF, ptG, ptH;


	switch (iMsg)
	{
	case WM_CREATE:

		// Initialize the State
		State.hwnd = hwnd;

		State.Camera = { 0, 0, 0 };

		ptA = { 0, 0, 0 };
		ptB = { 0, 0, 1 };
		ptC = { 0, 1, 0 };
		ptD = { 0, 1, 1 };
		ptE = { 1, 0, 0 };
		ptF = { 1, 0, 1 };
		ptG = { 1, 1, 0 };
		ptH = { 1, 1, 1 };

		State.Model.arrLine[0].ptStart = ptA;
		State.Model.arrLine[0].ptEnd = ptB;

		State.Model.arrLine[1].ptStart = ptB;
		State.Model.arrLine[1].ptEnd = ptF;

		State.Model.arrLine[2].ptStart = ptF;
		State.Model.arrLine[2].ptEnd = ptE;

		State.Model.arrLine[3].ptStart = ptE;
		State.Model.arrLine[3].ptEnd = ptA;

		State.Model.arrLine[4].ptStart = ptC;
		State.Model.arrLine[4].ptEnd = ptD;

		State.Model.arrLine[5].ptStart = ptD;
		State.Model.arrLine[5].ptEnd = ptH;

		State.Model.arrLine[6].ptStart = ptH;
		State.Model.arrLine[6].ptEnd = ptG;

		State.Model.arrLine[7].ptStart = ptG;
		State.Model.arrLine[7].ptEnd = ptC;

		State.Model.arrLine[8].ptStart = ptC;
		State.Model.arrLine[8].ptEnd = ptA;

		State.Model.arrLine[9].ptStart = ptD;
		State.Model.arrLine[9].ptEnd = ptB;

		State.Model.arrLine[10].ptStart = ptH;
		State.Model.arrLine[10].ptEnd = ptF;

		State.Model.arrLine[11].ptStart = ptG;
		State.Model.arrLine[11].ptEnd = ptE;

		State.Model.iNoOfLines = 12;
		break;

	case WM_PAINT:

		// Begin!
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);

		// Create White Pen
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		SelectObject(hdc, hPen);

		// Find Origin
		Origin[0] = (rc.right / 2) + Offset[0];
		Origin[1] = (rc.bottom / 2) + Offset[1];

		// x-axis
		arrPoints[0].x = 0;
		arrPoints[0].y = (rc.bottom / 2) + Offset[1];
		arrPoints[1].x = rc.right;
		arrPoints[1].y = (rc.bottom / 2) + Offset[1];
		Polyline(hdc, arrPoints, 2);

		// y-axis
		arrPoints[0].x = (rc.right / 2) + Offset[0];
		arrPoints[0].y = 0 ;
		arrPoints[1].x = (rc.right / 2) + Offset[0];
		arrPoints[1].y = rc.bottom;
		Polyline(hdc, arrPoints, 2);

		//// z-axis
		//arrPoints[0].x = (rc.right / 2) + Offset[0];
		//arrPoints[0].y = 0;
		//arrPoints[1].x = (rc.right / 2) + Offset[0];
		//arrPoints[1].y = rc.bottom;
		//Polyline(hdc, arrPoints, 2);

		// Draw Object
		for (int i = 0; i < State.Model.iNoOfLines; i++)
		{
			arrPoints[0].x = (100 * State.Model.arrLine[i].ptStart.x) + Origin[0];
			arrPoints[0].y = (100 * State.Model.arrLine[i].ptStart.z) + Origin[1];
			arrPoints[1].x = (100 * State.Model.arrLine[i].ptEnd.x) + Origin[0];
			arrPoints[1].y = (100 * State.Model.arrLine[i].ptEnd.z) + Origin[1];
			Polyline(hdc, arrPoints, 2);
		}

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case 'X':
			ptA = { 0, 0, 0 };
			ptB = { 0.853553391, -0.146446609, 0.5 };
			ptC = { -0.146446609, 0.85355, 0.5 };
			ptD = { 0.70711, 0.70711, 1 };
			ptE = { 0.5, 0.5, -0.7071 };
			ptF = { 1.35355,0.35355,-0.2071 };
			ptG = { 0.35355, 1.35355,-0.2071 };
			ptH = { 1.20711, 1.20711,0.29289 };

			State.Model.arrLine[0].ptStart = ptA;
			State.Model.arrLine[0].ptEnd = ptB;

			State.Model.arrLine[1].ptStart = ptB;
			State.Model.arrLine[1].ptEnd = ptF;

			State.Model.arrLine[2].ptStart = ptF;
			State.Model.arrLine[2].ptEnd = ptE;

			State.Model.arrLine[3].ptStart = ptE;
			State.Model.arrLine[3].ptEnd = ptA;

			State.Model.arrLine[4].ptStart = ptC;
			State.Model.arrLine[4].ptEnd = ptD;

			State.Model.arrLine[5].ptStart = ptD;
			State.Model.arrLine[5].ptEnd = ptH;

			State.Model.arrLine[6].ptStart = ptH;
			State.Model.arrLine[6].ptEnd = ptG;

			State.Model.arrLine[7].ptStart = ptG;
			State.Model.arrLine[7].ptEnd = ptC;

			State.Model.arrLine[8].ptStart = ptC;
			State.Model.arrLine[8].ptEnd = ptA;

			State.Model.arrLine[9].ptStart = ptD;
			State.Model.arrLine[9].ptEnd = ptB;

			State.Model.arrLine[10].ptStart = ptH;
			State.Model.arrLine[10].ptEnd = ptF;

			State.Model.arrLine[11].ptStart = ptG;
			State.Model.arrLine[11].ptEnd = ptE;
			break;

		case 'R':
			Offset[0] = Offset[1] = 0;
			break;

		case VK_UP:
			Offset[1]--;
			break;

		case VK_DOWN:
			Offset[1]++;
			break;

		case VK_LEFT:
			Offset[0]--;
			break;

		case VK_RIGHT:
			Offset[0]++;
			break;

		case VK_ESCAPE:
			iResult = MessageBox(hwnd, TEXT("Do you want to Quit?"), TEXT("3D Visualilzer"), MB_YESNO | MB_ICONQUESTION);
			if (iResult == IDYES) DestroyWindow(hwnd);
			break;
		}

		GetClientRect(hwnd, &rc);
		InvalidateRect(hwnd, &rc, TRUE);
		break;

	case WM_DESTROY:
		PostQuitMessage(0);
		break;
	}

	return(DefWindowProc(hwnd, iMsg, wParam, lParam));
}