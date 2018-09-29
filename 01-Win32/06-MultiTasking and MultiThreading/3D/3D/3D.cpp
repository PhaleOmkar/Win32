// Headers
#include <Windows.h>
#include <stdio.h>
#include <math.h>
#include "3D.h"

// Prototypes
LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);
void Rotate(LPSTATE, UINT);
DWORD ThreadMove(LPVOID);
void ResetCamera(LPSTATE);

void Cube(LPSTATE);
void ThreeCubes(LPSTATE);
void FiveAngleStar(LPSTATE);
void Pyramid(LPSTATE);
void Diamon(LPSTATE);
void LetterZ(LPSTATE);

void ConvertToLines(int , int *, LPSTATE);

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
	wndclass.hIcon = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));
	wndclass.hInstance = hInstance;
	wndclass.hCursor = LoadCursor(NULL, IDC_ARROW);
	wndclass.cbClsExtra = 0;
	wndclass.cbWndExtra = 0;
	wndclass.lpszClassName = szClassName;
	wndclass.lpszMenuName = NULL;
	wndclass.hIconSm = LoadIcon(hInstance, MAKEINTRESOURCE(MYICON));

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
	int matGraph[8][8] = { 0 };

	switch (iMsg)
	{
	case WM_CREATE:

		// Initialize the State
		State.hwnd = hwnd;
		State.Camera = { 0, 0, 0, 40 };
		State.iAnimate = 0;
		State.Directions[0] = 1;
		State.Directions[1] = 1;
		State.Directions[2] = 1;
		State.Offset[0] = 0;
		State.Offset[1] = 0;
		State.Origin[0] = 0;
		State.Origin[1] = 0;

		//Cube(&State);
		//ThreeCubes(&State);
		//FiveAngleStar(&State);

		// DIRTY IMPLIMENTATION
		matGraph[0][1] = 1;
		matGraph[2][3] = 1;
		matGraph[0][2] = 1;
		matGraph[3][6] = 1;
		matGraph[1][4] = 1;
		matGraph[4][5] = 1;
		matGraph[6][7] = 1;
		matGraph[5][7] = 1;		
		ConvertToLines(8, (int *)&matGraph, &State);

		// DIRTY

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
			arrPoints[0].x = (State.Camera.iScale * State.Model.arrLine[i].ptStart->x) + State.Origin[0];
			arrPoints[0].y = -(State.Camera.iScale * State.Model.arrLine[i].ptStart->y) + State.Origin[1];
			arrPoints[1].x = (State.Camera.iScale * State.Model.arrLine[i].ptEnd->x) + State.Origin[0];
			arrPoints[1].y = -(State.Camera.iScale * State.Model.arrLine[i].ptEnd->y) + State.Origin[1];
			Polyline(hdc, arrPoints, 2);
		}

		// Draw Debug Info
		SetBkColor(hdc, RGB(0, 0, 0));
		SetTextColor(hdc, RGB(255, 255, 255));
		sprintf_s(lpszDebugInfo, "Angles:\n\tX: %d\n\tY: %d\n\tZ: %d\n\nDirections:\n\tX: %d\n\tY: %d\n\tZ: %d", State.Camera.xAngle, State.Camera.yAngle, State.Camera.zAngle,
			State.Directions[0], State.Directions[1], State.Directions[2]);
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

		case '3':
			Pyramid(&State);
			break;

		case '4':
			Diamon(&State);
			break;
				
		case '5':
			FiveAngleStar(&State);
			break;

		case '6':
			LetterZ(&State);
			break;

		case 'I':
			State.Camera.iScale += 2;
			break;

		case 'O':
			if (State.Camera.iScale > 2)
				State.Camera.iScale -= 2;
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
			State.Camera.iScale = 40;

			State.Directions[0] = -State.Camera.xAngle;
			State.Directions[1] = -State.Camera.yAngle;
			State.Directions[2] = -State.Camera.zAngle;

			Rotate(&State, XAXIS);
			Rotate(&State, YAXIS);
			Rotate(&State, ZAXIS);

			ResetCamera(&State);

			break;

		case VK_UP:
			// Rotate along X-Axis
			State.Directions[0] = 1;
			Rotate(&State, XAXIS);
			break;

		case VK_DOWN:
			// Rotate along X-Axis Negative
			State.Directions[0] = -1;
			Rotate(&State, XAXIS);
			break;

		case VK_LEFT:
			// Rotate along Y-Axis 
			State.Directions[1] = 1;
			Rotate(&State, YAXIS);
			break;

		case VK_RIGHT:
			// Rotate along Y-Axis Negative
			State.Directions[1] = -1;
			Rotate(&State, YAXIS);
			break;

		case 'Q':
			// Rotate along Z-Axis
			State.Directions[2] = 1;
			Rotate(&State, ZAXIS);
			break;

		case 'A':
			// Rotate along Z-Axis Negative
			State.Directions[2] = -1;
			Rotate(&State, ZAXIS);
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

void Rotate(LPSTATE State, UINT iAxis)
{
	double dRotationMatrix[3][3];
	double dSin = 0;
	double dCos = 0;
	double dTmp[3];

	switch (iAxis)
	{

	case XAXIS:

		// Adjust Camera Angle
		if (State->Directions[0] == 1)
		{
			if (State->Camera.xAngle < 360)
				State->Camera.xAngle++;
			else State->Camera.xAngle = 0;
		}

		if (State->Directions[0] == -1)
		{
			if (State->Camera.xAngle > 0)
				State->Camera.xAngle--;
			else State->Camera.xAngle = 360;
		}

		// Calculate Rotation Matrix 
		dSin = sin(State->Directions[0] * PI / 180);
		dCos = cos(State->Directions[0] * PI / 180);

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

		// Adjust Camera Angle
		if (State->Directions[1] == 1)
		{
			if (State->Camera.yAngle < 360)
				State->Camera.yAngle++;
			else State->Camera.yAngle = 0;
		}

		if (State->Directions[1] == -1)
		{
			if (State->Camera.yAngle > 0)
				State->Camera.yAngle--;
			else State->Camera.yAngle = 360;
		}

		// Calculate Rotation Matrix 
		dSin = sin(State->Directions[1] * PI / 180);
		dCos = cos(State->Directions[1] * PI / 180);

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

		// Adjust Camera Angle
		if (State->Directions[2] == 1)
		{
			if (State->Camera.zAngle < 360)
				State->Camera.zAngle++;
			else State->Camera.zAngle = 0;
		}

		if (State->Directions[2] == -1)
		{
			if (State->Camera.zAngle > 0)
				State->Camera.zAngle--;
			else State->Camera.zAngle = 360;
		}

		// Calculate Rotation Matrix 
		dSin = sin(State->Directions[2] * PI / 180);
		dCos = cos(State->Directions[2] * PI / 180);

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

	for (int i = 0; i < State->Model.iNoOfPoints; i++)
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
		Rotate(lpState, XAXIS);
		Rotate(lpState, YAXIS);
		Rotate(lpState, ZAXIS);
		GetClientRect(lpState->hwnd, &rc);
		InvalidateRect(lpState->hwnd, &rc, TRUE);
		Sleep(100);
	}

	return 0;
}

void ResetCamera(LPSTATE State)
{
	State->Directions[0] = 1;
	State->Directions[1] = 1;
	State->Directions[2] = 1;

	State->Camera.xAngle = 0;
	State->Camera.yAngle = 0;
	State->Camera.zAngle = 0;
}

void Cube(LPSTATE State)
{
	ResetCamera(State);

	State->Points[0] = POINTEX({ -1, -1, -1 });
	State->Points[1] = POINTEX({ -1, -1,  1 });
	State->Points[2] = POINTEX({ -1,  1, -1 });
	State->Points[3] = POINTEX({ -1,  1,  1 });
	State->Points[4] = POINTEX({ 1, -1, -1 });
	State->Points[5] = POINTEX({ 1, -1,  1 });
	State->Points[6] = POINTEX({ 1,  1, -1 });
	State->Points[7] = POINTEX({ 1,  1,  1 });

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

	State->Model.iNoOfPoints = 8;
	State->Model.iNoOfLines = 12;
}

void ThreeCubes(LPSTATE State)
{
	ResetCamera(State);
	POINTEX ptA, ptB, ptC, ptD, ptE, ptF, ptG, ptH;

	for (int i = 0; i < 3; i++)
	{
		switch (i)
		{
		case 0:
			ptA = { -1, -1, -1 };
			ptB = { -1, -1,  1 };
			ptC = { -1,  1, -1 };
			ptD = { -1,  1,  1 };
			ptE = { 1, -1, -1 };
			ptF = { 1, -1,  1 };
			ptG = { 1,  1, -1 };
			ptH = { 1,  1,  1 };
			break;

		case 1:
			ptA = { -1 + 2, -1 + 2, -1 + 2 };
			ptB = { -1 + 2, -1 + 2,  1 + 2 };
			ptC = { -1 + 2,  1 + 2, -1 + 2 };
			ptD = { -1 + 2,  1 + 2,  1 + 2 };
			ptE = { 1 + 2, -1 + 2, -1 + 2 };
			ptF = { 1 + 2, -1 + 2,  1 + 2 };
			ptG = { 1 + 2,  1 + 2, -1 + 2 };
			ptH = { 1 + 2,  1 + 2,  1 + 2 };
			break;

		case 2:
			ptA = { -1 - 2, -1 - 2, -1 - 2 };
			ptB = { -1 - 2, -1 - 2,  1 - 2 };
			ptC = { -1 - 2,  1 - 2, -1 - 2 };
			ptD = { -1 - 2,  1 - 2,  1 - 2 };
			ptE = { 1 - 2, -1 - 2, -1 - 2 };
			ptF = { 1 - 2, -1 - 2,  1 - 2 };
			ptG = { 1 - 2,  1 - 2, -1 - 2 };
			ptH = { 1 - 2,  1 - 2,  1 - 2 };
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

		State->Model.arrLine[0 + (i * 12)].ptStart = &State->Points[0 + (i * 8)];
		State->Model.arrLine[0 + (i * 12)].ptEnd = &State->Points[1 + (i * 8)];

		State->Model.arrLine[1 + (i * 12)].ptStart = &State->Points[1 + (i * 8)];
		State->Model.arrLine[1 + (i * 12)].ptEnd = &State->Points[5 + (i * 8)];

		State->Model.arrLine[2 + (i * 12)].ptStart = &State->Points[5 + (i * 8)];
		State->Model.arrLine[2 + (i * 12)].ptEnd = &State->Points[4 + (i * 8)];

		State->Model.arrLine[3 + (i * 12)].ptStart = &State->Points[4 + (i * 8)];
		State->Model.arrLine[3 + (i * 12)].ptEnd = &State->Points[0 + (i * 8)];

		State->Model.arrLine[4 + (i * 12)].ptStart = &State->Points[2 + (i * 8)];
		State->Model.arrLine[4 + (i * 12)].ptEnd = &State->Points[3 + (i * 8)];

		State->Model.arrLine[5 + (i * 12)].ptStart = &State->Points[3 + (i * 8)];
		State->Model.arrLine[5 + (i * 12)].ptEnd = &State->Points[7 + (i * 8)];

		State->Model.arrLine[6 + (i * 12)].ptStart = &State->Points[7 + (i * 8)];
		State->Model.arrLine[6 + (i * 12)].ptEnd = &State->Points[6 + (i * 8)];

		State->Model.arrLine[7 + (i * 12)].ptStart = &State->Points[6 + (i * 8)];
		State->Model.arrLine[7 + (i * 12)].ptEnd = &State->Points[2 + (i * 8)];

		State->Model.arrLine[8 + (i * 12)].ptStart = &State->Points[2 + (i * 8)];
		State->Model.arrLine[8 + (i * 12)].ptEnd = &State->Points[0 + (i * 8)];

		State->Model.arrLine[9 + (i * 12)].ptStart = &State->Points[3 + (i * 8)];
		State->Model.arrLine[9 + (i * 12)].ptEnd = &State->Points[1 + (i * 8)];

		State->Model.arrLine[10 + (i * 12)].ptStart = &State->Points[7 + (i * 8)];
		State->Model.arrLine[10 + (i * 12)].ptEnd = &State->Points[5 + (i * 8)];

		State->Model.arrLine[11 + (i * 12)].ptStart = &State->Points[6 + (i * 8)];
		State->Model.arrLine[11 + (i * 12)].ptEnd = &State->Points[4 + (i * 8)];
	}

	State->Model.iNoOfPoints = 24;
	State->Model.iNoOfLines = 36;
}

void FiveAngleStar(LPSTATE State)
{
	ResetCamera(State);

	State->Points[0] = POINTEX({ 0, 15, 0 });
	State->Points[1] = POINTEX({ 15, 5, 0 });
	State->Points[2] = POINTEX({ 8, -12, 0 });
	State->Points[3] = POINTEX({ -8, -12, 0 });
	State->Points[4] = POINTEX({ -15, 5, 0 });

	State->Points[5] = POINTEX({ 3,5, 3 });
	State->Points[6] = POINTEX({ 3,5, -3 });

	State->Points[7] = POINTEX({ 5,-3, 3 });
	State->Points[8] = POINTEX({ 5,-3, -3 });

	State->Points[9] = POINTEX({ 0,-6, 3 });
	State->Points[10] = POINTEX({ 0,-6, -3 });

	State->Points[11] = POINTEX({ -5,-3, 3 });
	State->Points[12] = POINTEX({ -5,-3, -3 });

	State->Points[13] = POINTEX({ -3,5, 3 });
	State->Points[14] = POINTEX({ -3,5, -3 });

	// Lines From A Point
	State->Model.arrLine[0].ptStart = &State->Points[0];
	State->Model.arrLine[0].ptEnd = &State->Points[5];

	State->Model.arrLine[1].ptStart = &State->Points[0];
	State->Model.arrLine[1].ptEnd = &State->Points[6];

	State->Model.arrLine[2].ptStart = &State->Points[0];
	State->Model.arrLine[2].ptEnd = &State->Points[13];

	State->Model.arrLine[3].ptStart = &State->Points[0];
	State->Model.arrLine[3].ptEnd = &State->Points[14];

	// Lines From B Point
	State->Model.arrLine[4].ptStart = &State->Points[1];
	State->Model.arrLine[4].ptEnd = &State->Points[5];

	State->Model.arrLine[5].ptStart = &State->Points[1];
	State->Model.arrLine[5].ptEnd = &State->Points[6];

	State->Model.arrLine[6].ptStart = &State->Points[1];
	State->Model.arrLine[6].ptEnd = &State->Points[7];

	State->Model.arrLine[7].ptStart = &State->Points[1];
	State->Model.arrLine[7].ptEnd = &State->Points[8];

	// Lines From C Point
	State->Model.arrLine[8].ptStart = &State->Points[2];
	State->Model.arrLine[8].ptEnd = &State->Points[7];

	State->Model.arrLine[9].ptStart = &State->Points[2];
	State->Model.arrLine[9].ptEnd = &State->Points[8];

	State->Model.arrLine[10].ptStart = &State->Points[2];
	State->Model.arrLine[10].ptEnd = &State->Points[9];

	State->Model.arrLine[11].ptStart = &State->Points[2];
	State->Model.arrLine[11].ptEnd = &State->Points[10];

	// Lines From D Point
	State->Model.arrLine[12].ptStart = &State->Points[3];
	State->Model.arrLine[12].ptEnd = &State->Points[9];

	State->Model.arrLine[13].ptStart = &State->Points[3];
	State->Model.arrLine[13].ptEnd = &State->Points[10];

	State->Model.arrLine[14].ptStart = &State->Points[3];
	State->Model.arrLine[14].ptEnd = &State->Points[11];

	State->Model.arrLine[15].ptStart = &State->Points[3];
	State->Model.arrLine[15].ptEnd = &State->Points[12];

	// Lines From E Point
	State->Model.arrLine[16].ptStart = &State->Points[4];
	State->Model.arrLine[16].ptEnd = &State->Points[11];

	State->Model.arrLine[17].ptStart = &State->Points[4];
	State->Model.arrLine[17].ptEnd = &State->Points[12];

	State->Model.arrLine[18].ptStart = &State->Points[4];
	State->Model.arrLine[18].ptEnd = &State->Points[13];

	State->Model.arrLine[19].ptStart = &State->Points[4];
	State->Model.arrLine[19].ptEnd = &State->Points[14];

	// Upper Face
	State->Model.arrLine[20].ptStart = &State->Points[13];
	State->Model.arrLine[20].ptEnd = &State->Points[5];

	State->Model.arrLine[21].ptStart = &State->Points[5];
	State->Model.arrLine[21].ptEnd = &State->Points[7];

	State->Model.arrLine[22].ptStart = &State->Points[7];
	State->Model.arrLine[22].ptEnd = &State->Points[9];

	State->Model.arrLine[23].ptStart = &State->Points[9];
	State->Model.arrLine[23].ptEnd = &State->Points[11];

	State->Model.arrLine[24].ptStart = &State->Points[11];
	State->Model.arrLine[24].ptEnd = &State->Points[13];

	// Lower Face
	State->Model.arrLine[25].ptStart = &State->Points[14];
	State->Model.arrLine[25].ptEnd = &State->Points[6];

	State->Model.arrLine[26].ptStart = &State->Points[6];
	State->Model.arrLine[26].ptEnd = &State->Points[8];

	State->Model.arrLine[27].ptStart = &State->Points[8];
	State->Model.arrLine[27].ptEnd = &State->Points[10];

	State->Model.arrLine[28].ptStart = &State->Points[10];
	State->Model.arrLine[28].ptEnd = &State->Points[12];

	State->Model.arrLine[29].ptStart = &State->Points[12];
	State->Model.arrLine[29].ptEnd = &State->Points[14];

	// Two Face Connectors
	State->Model.arrLine[30].ptStart = &State->Points[5];
	State->Model.arrLine[30].ptEnd = &State->Points[6];

	State->Model.arrLine[31].ptStart = &State->Points[7];
	State->Model.arrLine[31].ptEnd = &State->Points[8];

	State->Model.arrLine[32].ptStart = &State->Points[9];
	State->Model.arrLine[32].ptEnd = &State->Points[10];

	State->Model.arrLine[33].ptStart = &State->Points[11];
	State->Model.arrLine[33].ptEnd = &State->Points[12];

	State->Model.arrLine[34].ptStart = &State->Points[13];
	State->Model.arrLine[34].ptEnd = &State->Points[14];

	State->Model.iNoOfPoints = 15;
	State->Model.iNoOfLines = 35;

}

void Pyramid(LPSTATE State)
{
	ResetCamera(State);

	State->Points[0] = POINTEX({ 0, 0, 5 });
	State->Points[1] = POINTEX({ 2, 2, 0 });
	State->Points[2] = POINTEX({ 2, -2, 0 });
	State->Points[3] = POINTEX({ -2, -2, 0 });
	State->Points[4] = POINTEX({ -2, 2, 0 });

	State->Model.arrLine[0].ptStart = &State->Points[0];
	State->Model.arrLine[0].ptEnd = &State->Points[1];

	State->Model.arrLine[1].ptStart = &State->Points[0];
	State->Model.arrLine[1].ptEnd = &State->Points[2];

	State->Model.arrLine[2].ptStart = &State->Points[0];
	State->Model.arrLine[2].ptEnd = &State->Points[3];

	State->Model.arrLine[3].ptStart = &State->Points[0];
	State->Model.arrLine[3].ptEnd = &State->Points[4];

	State->Model.arrLine[4].ptStart = &State->Points[1];
	State->Model.arrLine[4].ptEnd = &State->Points[2];

	State->Model.arrLine[5].ptStart = &State->Points[2];
	State->Model.arrLine[5].ptEnd = &State->Points[3];

	State->Model.arrLine[6].ptStart = &State->Points[3];
	State->Model.arrLine[6].ptEnd = &State->Points[4];

	State->Model.arrLine[7].ptStart = &State->Points[4];
	State->Model.arrLine[7].ptEnd = &State->Points[1];

	State->Model.iNoOfPoints = 5;
	State->Model.iNoOfLines = 8;

}

void Diamon(LPSTATE State)
{
	ResetCamera(State);

	State->Points[0] = POINTEX({ 0, 0, 5 });
	State->Points[1] = POINTEX({ 2, 2, 0 });
	State->Points[2] = POINTEX({ 2, -2, 0 });
	State->Points[3] = POINTEX({ -2, -2, 0 });
	State->Points[4] = POINTEX({ -2, 2, 0 });
	State->Points[5] = POINTEX({ 0, 0, -5 });

	State->Model.arrLine[0].ptStart = &State->Points[0];
	State->Model.arrLine[0].ptEnd = &State->Points[1];

	State->Model.arrLine[1].ptStart = &State->Points[0];
	State->Model.arrLine[1].ptEnd = &State->Points[2];

	State->Model.arrLine[2].ptStart = &State->Points[0];
	State->Model.arrLine[2].ptEnd = &State->Points[3];

	State->Model.arrLine[3].ptStart = &State->Points[0];
	State->Model.arrLine[3].ptEnd = &State->Points[4];

	State->Model.arrLine[4].ptStart = &State->Points[1];
	State->Model.arrLine[4].ptEnd = &State->Points[2];

	State->Model.arrLine[5].ptStart = &State->Points[2];
	State->Model.arrLine[5].ptEnd = &State->Points[3];

	State->Model.arrLine[6].ptStart = &State->Points[3];
	State->Model.arrLine[6].ptEnd = &State->Points[4];

	State->Model.arrLine[7].ptStart = &State->Points[4];
	State->Model.arrLine[7].ptEnd = &State->Points[1];

	State->Model.arrLine[8].ptStart = &State->Points[5];
	State->Model.arrLine[8].ptEnd = &State->Points[1];

	State->Model.arrLine[9].ptStart = &State->Points[5];
	State->Model.arrLine[9].ptEnd = &State->Points[2];

	State->Model.arrLine[10].ptStart = &State->Points[5];
	State->Model.arrLine[10].ptEnd = &State->Points[3];

	State->Model.arrLine[11].ptStart = &State->Points[5];
	State->Model.arrLine[11].ptEnd = &State->Points[4];

	State->Model.iNoOfPoints = 6;
	State->Model.iNoOfLines = 12;
}

void LetterZ(LPSTATE State)
{
	ResetCamera(State);

	State->Points[0] = POINTEX({ 0,10,-2 });
	State->Points[1] = POINTEX({ 10,10,-2 });
	State->Points[2] = POINTEX({ 0,8,-2 });
	State->Points[3] = POINTEX({ 6,8,-2 });
	State->Points[4] = POINTEX({ 6,2,-2 });
	State->Points[5] = POINTEX({ 10,2,-2 });
	State->Points[6] = POINTEX({ 0,0,-2 });
	State->Points[7] = POINTEX({ 10,0,-2 });

	State->Points[8] = POINTEX({ 0,10,2 });
	State->Points[9] = POINTEX({ 10,10,2 });
	State->Points[10] = POINTEX({ 0,8,2 });
	State->Points[11] = POINTEX({ 6,8,2 });
	State->Points[12] = POINTEX({ 6,2,2 });
	State->Points[13] = POINTEX({ 10,2,2 });
	State->Points[14] = POINTEX({ 0,0,2 });
	State->Points[15] = POINTEX({ 10,0,2 });

	State->Model.arrLine[0].ptStart = &State->Points[0];
	State->Model.arrLine[0].ptEnd = &State->Points[1];
	State->Model.arrLine[1].ptStart = &State->Points[0+8];
	State->Model.arrLine[1].ptEnd = &State->Points[1+8];
	State->Model.arrLine[2].ptStart = &State->Points[0];
	State->Model.arrLine[2].ptEnd = &State->Points[0+8];
	State->Model.arrLine[3].ptStart = &State->Points[1];
	State->Model.arrLine[3].ptEnd = &State->Points[1+8];

	State->Model.arrLine[4].ptStart = &State->Points[2];
	State->Model.arrLine[4].ptEnd = &State->Points[3];
	State->Model.arrLine[5].ptStart = &State->Points[2 + 8];
	State->Model.arrLine[5].ptEnd = &State->Points[3 + 8];
	State->Model.arrLine[6].ptStart = &State->Points[2];
	State->Model.arrLine[6].ptEnd = &State->Points[2 + 8];
	State->Model.arrLine[7].ptStart = &State->Points[3];
	State->Model.arrLine[7].ptEnd = &State->Points[3 + 8];

	State->Model.arrLine[8].ptStart = &State->Points[0];
	State->Model.arrLine[8].ptEnd = &State->Points[2];
	State->Model.arrLine[9].ptStart = &State->Points[0 + 8];
	State->Model.arrLine[9].ptEnd = &State->Points[2 + 8];
	State->Model.arrLine[10].ptStart = &State->Points[0];
	State->Model.arrLine[10].ptEnd = &State->Points[0 + 8];
	State->Model.arrLine[11].ptStart = &State->Points[2];
	State->Model.arrLine[11].ptEnd = &State->Points[2 + 8];

	State->Model.arrLine[12].ptStart = &State->Points[3];
	State->Model.arrLine[12].ptEnd = &State->Points[6];
	State->Model.arrLine[13].ptStart = &State->Points[3 + 8];
	State->Model.arrLine[13].ptEnd = &State->Points[6 + 8];
	State->Model.arrLine[14].ptStart = &State->Points[3];
	State->Model.arrLine[14].ptEnd = &State->Points[3 + 8];
	State->Model.arrLine[15].ptStart = &State->Points[6];
	State->Model.arrLine[15].ptEnd = &State->Points[6 + 8];

	State->Model.arrLine[16].ptStart = &State->Points[1];
	State->Model.arrLine[16].ptEnd = &State->Points[4];
	State->Model.arrLine[17].ptStart = &State->Points[1 + 8];
	State->Model.arrLine[17].ptEnd = &State->Points[4 + 8];
	State->Model.arrLine[18].ptStart = &State->Points[1];
	State->Model.arrLine[18].ptEnd = &State->Points[1 + 8];
	State->Model.arrLine[19].ptStart = &State->Points[4];
	State->Model.arrLine[19].ptEnd = &State->Points[4 + 8];

	State->Model.arrLine[20].ptStart = &State->Points[4];
	State->Model.arrLine[20].ptEnd = &State->Points[5];
	State->Model.arrLine[21].ptStart = &State->Points[4 + 8];
	State->Model.arrLine[21].ptEnd = &State->Points[5 + 8];
	State->Model.arrLine[22].ptStart = &State->Points[4];
	State->Model.arrLine[22].ptEnd = &State->Points[4 + 8];
	State->Model.arrLine[23].ptStart = &State->Points[5];
	State->Model.arrLine[23].ptEnd = &State->Points[5 + 8];

	State->Model.arrLine[24].ptStart = &State->Points[6];
	State->Model.arrLine[24].ptEnd = &State->Points[7];
	State->Model.arrLine[25].ptStart = &State->Points[6 + 8];
	State->Model.arrLine[25].ptEnd = &State->Points[7 + 8];
	State->Model.arrLine[26].ptStart = &State->Points[6];
	State->Model.arrLine[26].ptEnd = &State->Points[6 + 8];
	State->Model.arrLine[27].ptStart = &State->Points[7];
	State->Model.arrLine[27].ptEnd = &State->Points[7 + 8];

	State->Model.arrLine[28].ptStart = &State->Points[5];
	State->Model.arrLine[28].ptEnd = &State->Points[7];
	State->Model.arrLine[29].ptStart = &State->Points[5 + 8];
	State->Model.arrLine[29].ptEnd = &State->Points[7 + 8];
	State->Model.arrLine[30].ptStart = &State->Points[5];
	State->Model.arrLine[30].ptEnd = &State->Points[5 + 8];
	State->Model.arrLine[31].ptStart = &State->Points[7];
	State->Model.arrLine[31].ptEnd = &State->Points[7 + 8];

	State->Model.iNoOfLines = 32;
	State->Model.iNoOfPoints = 16;
}

void ConvertToLines(int iNoOfPoints, int *matGraph, LPSTATE State)
{

	// dirty fix
	State->Points[0] = POINTEX({ 0,10,-2 });
	State->Points[1] = POINTEX({ 10,10,-2 });
	State->Points[2] = POINTEX({ 0,8,-2 });
	State->Points[3] = POINTEX({ 6,8,-2 });
	State->Points[4] = POINTEX({ 6,2,-2 });
	State->Points[5] = POINTEX({ 10,2,-2 });
	State->Points[6] = POINTEX({ 0,0,-2 });
	State->Points[7] = POINTEX({ 10,0,-2 });

	State->Points[8] = POINTEX({ 0,10,2 });
	State->Points[9] = POINTEX({ 10,10,2 });
	State->Points[10] = POINTEX({ 0,8,2 });
	State->Points[11] = POINTEX({ 6,8,2 });
	State->Points[12] = POINTEX({ 6,2,2 });
	State->Points[13] = POINTEX({ 10,2,2 });
	State->Points[14] = POINTEX({ 0,0,2 });
	State->Points[15] = POINTEX({ 10,0,2 });

	int iLineNo = 0;

	// for uppper and lower face
	for(int i = 0 ; i < iNoOfPoints; i++)
	{
		for (int j = i+1; j < iNoOfPoints; j++)
		{
			if (matGraph[(i*iNoOfPoints) + j] == 1)
			{
				// upper face
				State->Model.arrLine[iLineNo].ptStart = &State->Points[i];
				State->Model.arrLine[iLineNo++].ptEnd = &State->Points[j];

				// lower face
				State->Model.arrLine[iLineNo].ptStart = &State->Points[i + iNoOfPoints];
				State->Model.arrLine[iLineNo++].ptEnd = &State->Points[j + iNoOfPoints];
			}
		}
	}

	// for upper to lower connections
	for (int i = 0; i < iNoOfPoints; i++)
	{
		State->Model.arrLine[iLineNo].ptStart = &State->Points[i];
		State->Model.arrLine[iLineNo++].ptEnd = &State->Points[i + iNoOfPoints];
	}

	State->Model.iNoOfLines = iLineNo;
	State->Model.iNoOfPoints = iNoOfPoints * 2;
}


