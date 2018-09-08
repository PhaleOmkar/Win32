// Headers
#include <Windows.h>
#include <stdio.h>
#include <math.h>

#define PI 3.14159265358979323846

#define XAXIS 1
#define YAXIS 2
#define ZAXIS 3

// Types
typedef struct tagPOINTEX {
	double x;
	double y;
	double z;
} POINTEX, *LPPOINTEX;

typedef struct tagLINE {
	LPPOINTEX ptStart;
	LPPOINTEX ptEnd;
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
	int iAnimate;
	int Offset[2];
	int Origin[2];
	HWND hwnd;
	MODEL Model;
	CAMERA Camera;
	POINTEX Points[100];
} STATE, *LPSTATE;

// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Rotate(LPSTATE, int, UINT);
DWORD ThreadMove(LPVOID);
void Cube(LPSTATE);
void ThreeCubes(LPSTATE);

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
	int iResult;

	RECT rc;
	PAINTSTRUCT ps;
	HDC hdc;
	POINT arrPoints[50];
	HPEN hPen;
	TCHAR lpszDebugInfo[512];
	static HANDLE hThreadMove;

	switch (iMsg)
	{
	case WM_CREATE:

		// Initialize the State
		State.hwnd = hwnd;
		State.Camera = { 1, 1, 1};
		State.iAnimate = 0;
		State.Offset[0] = 0;
		State.Offset[1] = 0;
		State.Origin[0] = 0;
		State.Origin[1] = 0;

		//Cube(&State);
		//ThreeCubes(&State);

		hThreadMove = CreateThread(NULL, 0, (LPTHREAD_START_ROUTINE)ThreadMove, (LPVOID)&State, CREATE_SUSPENDED, NULL);
		break;

	case WM_PAINT:

		// Begin!
		hdc = BeginPaint(hwnd, &ps);
		GetClientRect(hwnd, &rc);

		// Create White Pen
		hPen = CreatePen(PS_SOLID, 1, RGB(255, 255, 255));
		SelectObject(hdc, hPen);

		// Find Origin
		State.Origin[0] = (rc.right / 2) + State.Offset[0];
		State.Origin[1] = (rc.bottom / 2) + State.Offset[1];

		//// x-axis
		//arrPoints[0].x = 0;
		//arrPoints[0].y = (rc.bottom / 2) + State.Offset[1];
		//arrPoints[1].x = rc.right;
		//arrPoints[1].y = (rc.bottom / 2) + State.Offset[1];
		//Polyline(hdc, arrPoints, 2);

		//// y-axis
		//arrPoints[0].x = (rc.right / 2) + State.Offset[0];
		//arrPoints[0].y = 0 ;
		//arrPoints[1].x = (rc.right / 2) + State.Offset[0];
		//arrPoints[1].y = rc.bottom;
		//Polyline(hdc, arrPoints, 2);

		//// z-axis
		//arrPoints[0].x = (rc.right / 2) + Offset[0];
		//arrPoints[0].y = 0;
		//arrPoints[1].x = (rc.right / 2) + Offset[0];
		//arrPoints[1].y = rc.bottom;
		//Polyline(hdc, arrPoints, 2);

		// Draw Object
		for (int i = 0; i < State.Model.iNoOfLines; i++)
		{
			arrPoints[0].x = (75 * State.Model.arrLine[i].ptStart->x) + State.Origin[0];
			arrPoints[0].y = (75 * State.Model.arrLine[i].ptStart->y) + State.Origin[1];
			arrPoints[1].x = (75 * State.Model.arrLine[i].ptEnd->x) + State.Origin[0];
			arrPoints[1].y = (75 * State.Model.arrLine[i].ptEnd->y) + State.Origin[1];
			Polyline(hdc, arrPoints, 2);
		}

		// Draw Debug Info
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		sprintf_s(lpszDebugInfo, "Angles:\n\tX: %d\n\tY: %d\n\tZ: %d\n\nDirections:\n\tX: %d\n\tY: %d\n\tZ: %d", State.Camera.xAngle, State.Camera.yAngle, State.Camera.zAngle);
		DrawText(hdc, lpszDebugInfo, -1, &rc, DT_TOP | DT_LEFT);

		EndPaint(hwnd, &ps);
		break;

	case WM_KEYDOWN:

		switch (wParam)
		{
		case '1':
			Cube(&State);
			break;

		case '2':
			ThreeCubes(&State);
			break;

		case VK_SPACE:
			if (State.iAnimate == 0) {
				State.iAnimate = 1;
				ResumeThread(hThreadMove);
			}
			else
			{
				State.iAnimate = 0;
				SuspendThread(hThreadMove);
			}
			break;
		
		case 'R':
			State.Offset[0] = State.Offset[1] = 0;
			State.Camera.xAngle = 1;
			State.Camera.yAngle = 1;
			State.Camera.zAngle = 1;
			Rotate(&State, 8, XAXIS);
			Rotate(&State, 8, YAXIS);
			Rotate(&State, 8, ZAXIS);
			break;

		case VK_UP:
			// Rotate along X-Axis
			State.Camera.xAngle = 1;
			Rotate(&State, 8, XAXIS);
			break;

		case VK_DOWN:
			// Rotate along X-Axis Negative
			State.Camera.xAngle = -1;
			Rotate(&State, 8, XAXIS);
			break;

		case VK_LEFT:
			// Rotate along Y-Axis 
			State.Camera.yAngle = 1;
			Rotate(&State, 8, YAXIS);
			break;

		case VK_RIGHT:
			// Rotate along Y-Axis Negative
			State.Camera.yAngle = - 1;
			Rotate(&State, 8, YAXIS);
			break;

		case 'Q':
			// Rotate along Z-Axis
			State.Camera.zAngle = 1;
			Rotate(&State, 8, ZAXIS);
			break;

		case 'A':
			// Rotate along Z-Axis Negative
			State.Camera.zAngle = - 1;
			Rotate(&State, 8, ZAXIS);
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

void Rotate(LPSTATE State, int iNoOfPoints, UINT iAxis)
{
	double dRotationMatrix[3][3];
	double dSin = 0;
	double dCos = 0;
	double dTmp[3];

	switch (iAxis)
	{

	case XAXIS:
		dSin = sin(State->Camera.xAngle * PI / 180);
		dCos = cos(State->Camera.xAngle * PI / 180);

		dRotationMatrix[0][0] = 1;
		dRotationMatrix[0][1] = 0;
		dRotationMatrix[0][2] = 0;

		dRotationMatrix[1][0] = 0;
		dRotationMatrix[1][1] = dCos;
		dRotationMatrix[1][2] = -dSin;

		dRotationMatrix[2][0] = 0;
		dRotationMatrix[2][1] = dSin;
		dRotationMatrix[2][2] = dCos;

		break;

	case YAXIS:
		dSin = sin(State->Camera.yAngle * PI / 180);
		dCos = cos(State->Camera.yAngle * PI / 180);

		dRotationMatrix[0][0] = dCos;
		dRotationMatrix[0][1] = 0;
		dRotationMatrix[0][2] = dSin;

		dRotationMatrix[1][0] = 0;
		dRotationMatrix[1][1] = 1;
		dRotationMatrix[1][2] = 0;

		dRotationMatrix[2][0] = -dSin;
		dRotationMatrix[2][1] = 0;
		dRotationMatrix[2][2] = dCos;

		break;

	case ZAXIS:
		dSin = sin(State->Camera.zAngle * PI / 180);
		dCos = cos(State->Camera.zAngle * PI / 180);

		dRotationMatrix[0][0] = dCos;
		dRotationMatrix[0][1] = -dSin;
		dRotationMatrix[0][2] = 0;

		dRotationMatrix[1][0] = dSin;
		dRotationMatrix[1][1] = dCos;
		dRotationMatrix[1][2] = 0;

		dRotationMatrix[2][0] = 0;
		dRotationMatrix[2][1] = 0;
		dRotationMatrix[2][2] = 1;

		break;

	}

	for (int i = 0; i < State->Model.iNoOfLines*2 ; i++)
	{
		dTmp[0] = (dRotationMatrix[0][0] * State->Points[i].x) + (dRotationMatrix[0][1] * State->Points[i].y) + (dRotationMatrix[0][2] * State->Points[i].z);
		dTmp[1] = (dRotationMatrix[1][0] * State->Points[i].x) + (dRotationMatrix[1][1] * State->Points[i].y) + (dRotationMatrix[1][2] * State->Points[i].z);
		dTmp[2] = (dRotationMatrix[2][0] * State->Points[i].x) + (dRotationMatrix[2][1] * State->Points[i].y) + (dRotationMatrix[2][2] * State->Points[i].z);
		
		State->Points[i].x = dTmp[0];
		State->Points[i].y = dTmp[1];
		State->Points[i].z = dTmp[2];
	}

}

DWORD ThreadMove(LPVOID State)
{
	RECT rc;
	LPSTATE lpState = (LPSTATE)State;

	while (TRUE)
	{
		Rotate(lpState, 8, XAXIS);
		Rotate(lpState, 8, YAXIS);
		Rotate(lpState, 8, ZAXIS);
		GetClientRect(lpState->hwnd, &rc);
		InvalidateRect(lpState->hwnd, &rc, TRUE);
		Sleep(100);
	}

	return 0;
}

void Cube(LPSTATE State)
{
	POINTEX ptA = { -1, -1, -1 };
	POINTEX ptB = { -1, -1,  1 };
	POINTEX ptC = { -1,  1, -1 };
	POINTEX ptD = { -1,  1,  1 };
	POINTEX ptE = { 1, -1, -1 };
	POINTEX ptF = { 1, -1,  1 };
	POINTEX ptG = { 1,  1, -1 };
	POINTEX ptH = { 1,  1,  1 };

	State->Points[0] = ptA;
	State->Points[1] = ptB;
	State->Points[2] = ptC;
	State->Points[3] = ptD;
	State->Points[4] = ptE;
	State->Points[5] = ptF;
	State->Points[6] = ptG;
	State->Points[7] = ptH;

	State->Model.arrLine[0].ptStart = &State->Points[0];
	State->Model.arrLine[0].ptEnd = &State->Points[1];

	State->Model.arrLine[1].ptStart = &State->Points[1];
	State->Model.arrLine[1].ptEnd = &State->Points[5];

	State->Model.arrLine[2].ptStart = &State->Points[5];
	State->Model.arrLine[2].ptEnd = &State->Points[4];

	State->Model.arrLine[3].ptStart = &State->Points[4];
	State->Model.arrLine[3].ptEnd = &State->Points[0];

	State->Model.arrLine[4].ptStart = &State->Points[2];
	State->Model.arrLine[4].ptEnd = &State->Points[3];

	State->Model.arrLine[5].ptStart = &State->Points[3];
	State->Model.arrLine[5].ptEnd = &State->Points[7];

	State->Model.arrLine[6].ptStart = &State->Points[7];
	State->Model.arrLine[6].ptEnd = &State->Points[6];

	State->Model.arrLine[7].ptStart = &State->Points[6];
	State->Model.arrLine[7].ptEnd = &State->Points[2];

	State->Model.arrLine[8].ptStart = &State->Points[2];
	State->Model.arrLine[8].ptEnd = &State->Points[0];

	State->Model.arrLine[9].ptStart = &State->Points[3];
	State->Model.arrLine[9].ptEnd = &State->Points[1];

	State->Model.arrLine[10].ptStart = &State->Points[7];
	State->Model.arrLine[10].ptEnd = &State->Points[5];

	State->Model.arrLine[11].ptStart = &State->Points[6];
	State->Model.arrLine[11].ptEnd = &State->Points[4];

	State->Model.iNoOfLines = 12;
}

void ThreeCubes(LPSTATE State)
{
	POINTEX ptA, ptB, ptC, ptD, ptE, ptF, ptG, ptH;

	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			ptA = { -1, -1, -1 - 2};
			ptB = { -1, -1,  1 - 2};
			ptC = { -1,  1, -1 - 2};
			ptD = { -1,  1,  1 - 2};
			ptE = {  1, -1, -1 - 2};
			ptF = {  1, -1,  1 - 2};
			ptG = {  1,  1, -1 - 2};
			ptH = {  1,  1,  1 - 2};
			break;

		case 1:
			ptA = { -1 +2, -1 +2, -1 };
			ptB = { -1 +2, -1 +2,  1 };
			ptC = { -1 +2,  1 +2, -1 };
			ptD = { -1 +2,  1 +2,  1 };
			ptE = {  1 +2, -1 +2, -1 };
			ptF = {  1 +2, -1 +2,  1 };
			ptG = {  1 +2,  1 +2, -1 };
			ptH = {  1 +2,  1 +2,  1 };
			break;

		case 2:
			ptA = { -1 -2, -1 +2, -1 };
			ptB = { -1 -2, -1 +2,  1 };
			ptC = { -1 -2,  1 +2, -1 };
			ptD = { -1 -2,  1 +2,  1 };
			ptE = {  1 -2, -1 +2, -1 };
			ptF = {  1 -2, -1 +2,  1 };
			ptG = {  1 -2,  1 +2, -1 };
			ptH = {  1 -2,  1 +2,  1 };
			break;
		}
		

		State->Points[0 + (i * 8)] = ptA;
		State->Points[1 + (i * 8)] = ptB;
		State->Points[2 + (i * 8)] = ptC;
		State->Points[3 + (i * 8)] = ptD;
		State->Points[4 + (i * 8)] = ptE;
		State->Points[5 + (i * 8)] = ptF;
		State->Points[6 + (i * 8)] = ptG;
		State->Points[7 + (i * 8)] = ptH;

		State->Model.arrLine[0 + (i * 12)].ptStart = &State->Points[0 + (i *8)];
		State->Model.arrLine[0 + (i * 12)].ptEnd = &State->Points[1 + (i *8)];

		State->Model.arrLine[1 + (i * 12)].ptStart = &State->Points[1 + (i *8)];
		State->Model.arrLine[1 + (i * 12)].ptEnd = &State->Points[5 + (i *8)];
		
		State->Model.arrLine[2 + (i * 12)].ptStart = &State->Points[5 + (i *8)];
		State->Model.arrLine[2 + (i * 12)].ptEnd = &State->Points[4 + (i *8)];
		
		State->Model.arrLine[3 + (i * 12)].ptStart = &State->Points[4 + (i *8)];
		State->Model.arrLine[3 + (i * 12)].ptEnd = &State->Points[0 + (i *8)];
		
		State->Model.arrLine[4 + (i * 12)].ptStart = &State->Points[2 + (i *8)];
		State->Model.arrLine[4 + (i * 12)].ptEnd = &State->Points[3 + (i *8)];
		
		State->Model.arrLine[5 + (i * 12)].ptStart = &State->Points[3 + (i *8)];
		State->Model.arrLine[5 + (i * 12)].ptEnd = &State->Points[7 + (i *8)];
		
		State->Model.arrLine[6 + (i * 12)].ptStart = &State->Points[7 + (i *8)];
		State->Model.arrLine[6 + (i * 12)].ptEnd = &State->Points[6 + (i *8)];
		
		State->Model.arrLine[7 + (i * 12)].ptStart = &State->Points[6 + (i *8)];
		State->Model.arrLine[7 + (i * 12)].ptEnd = &State->Points[2 + (i *8)];
		
		State->Model.arrLine[8 + (i * 12)].ptStart = &State->Points[2 + (i *8)];
		State->Model.arrLine[8 + (i * 12)].ptEnd = &State->Points[0 + (i *8)];
		
		State->Model.arrLine[9 + (i * 12)].ptStart = &State->Points[3 + (i *8)];
		State->Model.arrLine[9 + (i * 12)].ptEnd = &State->Points[1 + (i *8)];
		
		State->Model.arrLine[10 + (i * 12)].ptStart = &State->Points[7 + (i *8)];
		State->Model.arrLine[10 + (i * 12)].ptEnd = &State->Points[5 + (i *8)];

		State->Model.arrLine[11 + (i * 12)].ptStart = &State->Points[6 + (i *8)];
		State->Model.arrLine[11 + (i * 12)].ptEnd = &State->Points[4 + (i *8)];
	}
	State->Model.iNoOfLines = 36;
}
